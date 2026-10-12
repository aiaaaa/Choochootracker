#include <chipnomad_lib.h>
#include <project_utils.h>
#include "import_lsdj.h"
#include "import_common.h"
#include <lsdj_format.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void copyName(char* dest, size_t destSize, const uint8_t* src, size_t srcSize) {
  size_t len = 0;
  while (len < srcSize && src[len] >= 0x20 && src[len] < 0x7F && len < destSize - 1) {
    dest[len] = (char)src[len];
    len++;
  }
  while (len > 0 && dest[len - 1] == ' ') len--;
  dest[len] = 0;
}

// The envelope byte is the Game Boy NRx2 register: initial volume in the high
// nibble, direction in bit 3 and period in the low 3 bits.
static void convertEnvelope(InstrumentSimpleChip* s, uint8_t env) {
  s->envelopeInitial = env >> 4;
  s->envelopeIncrease = (env >> 3) & 1;
  s->envelopePeriod = env & 7;
  // Only the hardware envelope shapes the sound, like on the Game Boy.
  s->attack = 0;
  s->decay = 0;
  s->sustain = 255;
  s->release = 20;
}

// LSDj panning bits: 0 mute, 1 right, 2 left, 3 both.
static uint8_t convertPan(uint8_t bits) {
  bits &= 3;
  return bits == 1 ? 255 : bits == 2 ? 0 : 128;
}

static void convertInstrument(Instrument* inst, const uint8_t* src) {
  switch (src[0]) {
    case LSDJ_PULSE: {
      getInstrumentFunctions(InstrumentType::GBPulse).init(inst);
      InstrumentSimpleChip* s = &inst->chip.simpleChip;
      s->mode = src[7] >> 6; // 12.5%, 25%, 50%, 75%
      convertEnvelope(s, src[1]);
      // The pulse sweep byte is not imported: read as a Game Boy NR10 it
      // detunes the notes.
      s->sweepPeriod = s->sweepShift = s->sweepNegate = 0;
      inst->pan = convertPan(src[7]);
      break;
    }
    case LSDJ_NOISE: {
      getInstrumentFunctions(InstrumentType::GBNoise).init(inst);
      InstrumentSimpleChip* s = &inst->chip.simpleChip;
      s->mode = src[2] & 1; // stable = 7 bit
      convertEnvelope(s, src[1]);
      inst->pan = convertPan(src[7]);
      break;
    }
    case LSDJ_WAVE: { // the wave frames are not converted: Hollow Lead stands in
      getInstrumentFunctions(InstrumentType::GBPulse).init(inst);
      inst->pan = convertPan(src[7]);
      break;
    }
    default: { // kit: samples are not converted
      getInstrumentFunctions(InstrumentType::GBNoise).init(inst);
      inst->pan = convertPan(src[7]);
      break;
    }
  }
}

int projectLoadLSDJ(Project* project, const char* path) {
  if (!project || !path) return 1;

  FILE* f = fopen(path, "rb");
  if (!f) {
    snprintf(projectFileError, 40, "Cannot open LSDj file");
    return 1;
  }
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);
  if (size < LSDJ_MIN_FILE_SIZE || size > LSDJ_MAX_FILE_SIZE) {
    fclose(f);
    snprintf(projectFileError, 40, "Not a valid LSDj song");
    return 1;
  }
  uint8_t* file = (uint8_t*)malloc(size);
  size_t got = file ? fread(file, 1, size, f) : 0;
  fclose(f);
  if (!file || got != (size_t)size) {
    free(file);
    snprintf(projectFileError, 40, "Cannot read LSDj file");
    return 1;
  }

  uint8_t* data = (uint8_t*)malloc(LSDJ_SONG_SIZE);
  bool ok = data && lsdjDecompress(file + LSDJ_HEADER_SIZE, size - LSDJ_HEADER_SIZE, data);
  // Both memory markers "rb" must be where LSDj puts them.
  ok = ok && data[LSDJ_MARKER1] == 'r' && data[LSDJ_MARKER1 + 1] == 'b' && data[LSDJ_MARKER2] == 'r' && data[LSDJ_MARKER2 + 1] == 'b';
  if (!ok) {
    free(data);
    free(file);
    snprintf(projectFileError, 40, "Not a valid LSDj song");
    return 1;
  }

  Project p;
  projectInitAY(&p);

  // One LSDj row is a 16th note: 6 ticks at T bpm is T / 60 * 4 * 6 ticks per second.
  int bpm = data[LSDJ_TEMPO];
  if (bpm < 40) bpm = 128;
  p.tickRate = (float)(bpm * 0.4);

  copyName(p.title, sizeof(p.title), file, LSDJ_NAME_SIZE);

  for (int row = 0; row < LSDJ_ROWS && row < PROJECT_MAX_LENGTH; row++) {
    for (int t = 0; t < LSDJ_CHANNELS && t < PROJECT_MAX_TRACKS; t++) {
      uint8_t chain = data[LSDJ_SONG_ROWS + row * LSDJ_CHANNELS + t];
      p.song[row][t] = chain < LSDJ_CHAINS ? chain : EMPTY_VALUE_16;
    }
  }

  for (int c = 0; c < LSDJ_CHAINS && c < PROJECT_MAX_CHAINS; c++) {
    chainClear(&p.chains[c]);
    for (int s = 0; s < 16; s++) {
      uint8_t phrase = data[LSDJ_CHAIN_PHRASES + c * 16 + s];
      if (phrase < LSDJ_PHRASES) {
        p.chains[c].rows[s].phrase = phrase;
        p.chains[c].rows[s].transpose = data[LSDJ_CHAIN_TRANSPOSE + c * 16 + s];
      }
    }
  }

  uint8_t instrumentUsed[LSDJ_INSTRUMENTS] = {0};
  for (int ph = 0; ph < LSDJ_PHRASES && ph < PROJECT_MAX_PHRASES; ph++) {
    phraseClear(&p.phrases[ph]);
    for (int s = 0; s < 16; s++) {
      PhraseRow* dest = &p.phrases[ph].rows[s];
      initEmptyPhraseRow(dest);
      uint8_t note = data[LSDJ_PHRASE_NOTES + ph * 16 + s];
      if (note == 0) continue;
      int index = LSDJ_NOTE_INDEX_OFFSET + note;
      if (p.pitchTable.length > 0 && index >= p.pitchTable.length) index = p.pitchTable.length - 1;
      dest->note = (uint8_t)index;
      dest->volume = PHRASE_VOLUME_MAX; // LSDj has no note volume: full level
      uint8_t inst = data[LSDJ_PHRASE_INSTRUMENTS + ph * 16 + s];
      if (inst < LSDJ_INSTRUMENTS && inst < PROJECT_MAX_INSTRUMENTS) {
        dest->instrument = inst;
        instrumentUsed[inst] = 1;
      }
    }
  }

  for (int i = 0; i < LSDJ_INSTRUMENTS && i < PROJECT_MAX_INSTRUMENTS; i++) {
    if (!instrumentUsed[i] && !data[LSDJ_INSTRUMENT_ALLOC + i]) continue;
    Instrument* inst = &p.instruments[i];
    convertInstrument(inst, data + LSDJ_INSTRUMENT_PARAMS + i * LSDJ_INSTRUMENT_SIZE);
    char name[LSDJ_INSTRUMENT_NAME_SIZE + 1];
    copyName(name, sizeof(name), data + LSDJ_INSTRUMENT_NAMES + i * LSDJ_INSTRUMENT_NAME_SIZE, LSDJ_INSTRUMENT_NAME_SIZE);
    if (name[0]) snprintf(inst->name, sizeof(inst->name), "%s", name);
    else snprintf(inst->name, sizeof(inst->name), "LSDJ %02X", i);
  }

  free(data);
  free(file);
  projectFree(project);
  *project = p;
  return 0;
}
