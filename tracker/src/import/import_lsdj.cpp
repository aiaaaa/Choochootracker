#include <chipnomad_lib.h>
#include <project_utils.h>
#include "import_lsdj.h"
#include "import_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Layout of the decompressed 32 KB LSDj song (as documented by liblsdj, MIT).
#define LSDJ_SONG_SIZE 0x8000
#define LSDJ_BLOCK_SIZE 0x200
#define LSDJ_HEADER_SIZE 9 // 8 bytes of name, 1 byte of format version
#define LSDJ_NAME_SIZE 8
#define LSDJ_MAX_FILE_SIZE (LSDJ_HEADER_SIZE + 192 * LSDJ_BLOCK_SIZE)

#define LSDJ_PHRASE_NOTES 0x0000      // 255 phrases x 16 bytes, 0 = empty
#define LSDJ_INSTRUMENT_NAMES 0x1E7A  // 64 x 5 bytes
#define LSDJ_SONG_ROWS 0x1290         // 256 rows x 4 channels, 0xFF = empty
#define LSDJ_INSTRUMENT_ALLOC 0x2040  // 64 flags
#define LSDJ_CHAIN_PHRASES 0x2080     // 128 chains x 16 bytes, 0xFF = empty
#define LSDJ_CHAIN_TRANSPOSE 0x2880   // 128 chains x 16 bytes, signed
#define LSDJ_INSTRUMENT_PARAMS 0x3080 // 64 x 16 bytes
#define LSDJ_TEMPO 0x3FB4
#define LSDJ_PHRASE_INSTRUMENTS 0x7000 // 255 phrases x 16 bytes, 0xFF = none

#define LSDJ_PHRASES 255
#define LSDJ_CHAINS 128
#define LSDJ_INSTRUMENTS 64
#define LSDJ_CHANNELS 4
#define LSDJ_ROWS 256
#define LSDJ_INSTRUMENT_SIZE 16
#define LSDJ_INSTRUMENT_NAME_SIZE 5

enum { LSDJ_PULSE = 0, LSDJ_WAVE = 1, LSDJ_KIT = 2, LSDJ_NOISE = 3 };

// LSDj note 1 is its lowest note, C-3, about 65.4 Hz (MIDI 36). Pitch table
// index N is MIDI note 12+N, as in import_midi.cpp.
#define LSDJ_NOTE_INDEX_OFFSET 23

// LSDj compression: 0xC0 is a run (C0 value count) or a literal C0 (C0 C0);
// 0xE0 is a special code: E0 E0 literal E0, E0 FF end of data, E0 F0 n and
// E0 F1 n repeat the default wave (16 bytes) or instrument (16 bytes) n
// times, any other E0 n jumps to the start of the next 512 byte block.
static const uint8_t DEFAULT_WAVE[16] = {0x8E, 0xCD, 0xEF, 0xFF, 0xFF, 0xEE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x11, 0x10, 0x01, 0x23};
static const uint8_t DEFAULT_INSTRUMENT[16] = {0xA8, 0, 0, 0xFF, 0, 0, 3, 0, 0, 0xD0, 0, 0, 0, 0xF3, 0, 0};

static bool lsdjDecompress(const uint8_t* in, size_t size, uint8_t* out) {
  size_t i = 0, n = 0;
  while (i < size) {
    uint8_t b = in[i++];
    if (b == 0xC0) {
      if (i >= size) return false;
      uint8_t v = in[i++];
      if (v == 0xC0) {
        if (n >= LSDJ_SONG_SIZE) return false;
        out[n++] = 0xC0;
      } else {
        if (i >= size) return false;
        size_t count = in[i++];
        if (n + count > LSDJ_SONG_SIZE) return false;
        memset(out + n, v, count);
        n += count;
      }
    } else if (b == 0xE0) {
      if (i >= size) return false;
      uint8_t v = in[i++];
      if (v == 0xE0) {
        if (n >= LSDJ_SONG_SIZE) return false;
        out[n++] = 0xE0;
      } else if (v == 0xFF) {
        return n == LSDJ_SONG_SIZE;
      } else if (v == 0xF0 || v == 0xF1) {
        if (i >= size) return false;
        size_t count = in[i++];
        if (n + count * 16 > LSDJ_SONG_SIZE) return false;
        for (size_t k = 0; k < count; k++) memcpy(out + n + k * 16, v == 0xF0 ? DEFAULT_WAVE : DEFAULT_INSTRUMENT, 16);
        n += count * 16;
      } else {
        i = (i + LSDJ_BLOCK_SIZE - 1) / LSDJ_BLOCK_SIZE * LSDJ_BLOCK_SIZE;
      }
    } else {
      if (n >= LSDJ_SONG_SIZE) return false;
      out[n++] = b;
    }
  }
  return false;
}

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
      uint8_t sweep = src[4]; // NR10, 0xFF means no sweep
      if (sweep == 0xFF) {
        s->sweepPeriod = s->sweepShift = s->sweepNegate = 0;
      } else {
        s->sweepPeriod = (sweep >> 4) & 7;
        s->sweepNegate = (sweep >> 3) & 1;
        s->sweepShift = sweep & 7;
      }
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
    case LSDJ_WAVE: {
      getInstrumentFunctions(InstrumentType::AChChid).init(inst);
      inst->chip.achchid.wave = AChChidWave::square;
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
  if (size <= LSDJ_HEADER_SIZE || size > LSDJ_MAX_FILE_SIZE) {
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
  ok = ok && data[0x1E78] == 'r' && data[0x1E79] == 'b' && data[0x3E80] == 'r' && data[0x3E81] == 'b';
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
