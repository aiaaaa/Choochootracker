#include "doctest.h"

#include <import/import_lsdj.h>
#include <export/export_lsdj.h>
#include <lsdj_format.h>
#include <chipnomad_lib.h>
#include <import/import_common.h>

#include <cstdio>
#include <cstring>
#include <vector>

namespace {

const size_t SONG_SIZE = 0x8000;

std::vector<uint8_t> makeSong() {
  std::vector<uint8_t> s(SONG_SIZE, 0);
  s[0x1E78] = 'r'; s[0x1E79] = 'b';
  s[0x3E80] = 'r'; s[0x3E81] = 'b';
  s[0x7FF0] = 'r'; s[0x7FF1] = 'b';
  memset(&s[0x1290], 0xFF, 256 * 4);
  memset(&s[0x2080], 0xFF, 128 * 16);
  memset(&s[0x7000], 0xFF, 255 * 16);
  s[0x3FB4] = 150; // tempo
  return s;
}

bool writeLsdsng(const char* path, const std::vector<uint8_t>& song, const char* name) {
  FILE* f = fopen(path, "wb");
  if (!f) return false;
  uint8_t header[9] = {0};
  memcpy(header, name, strlen(name));
  header[8] = 18;
  fwrite(header, 1, 9, f);
  std::vector<uint8_t> c(LSDJ_MAX_BLOCKS * LSDJ_BLOCK_SIZE);
  size_t n = lsdjCompress(song.data(), c.data());
  fwrite(c.data(), 1, n, f);
  fclose(f);
  return true;
}

}

TEST_SUITE("import_lsdj") {

TEST_CASE("imports song, chain, transpose, phrase notes and instruments") {
  auto s = makeSong();
  s[0x1290 + 2 * 4 + 1] = 3;       // song row 2, PU2 -> chain 3
  s[0x2080 + 3 * 16] = 7;          // chain 3 step 0 -> phrase 7
  s[0x2880 + 3 * 16] = 0xFE;       // transpose -2
  s[7 * 16 + 0] = 1;               // lowest note, C-3 = MIDI 36
  s[7 * 16 + 4] = 0x1D;
  s[0x7000 + 7 * 16 + 0] = 2;      // instrument 2
  s[0x7000 + 7 * 16 + 4] = 2;
  s[0x2040 + 2] = 1;               // instrument 2 allocated, pulse
  memcpy(&s[0x1E7A + 2 * 5], "LEAD", 4);
  uint8_t* in = &s[0x3080 + 2 * 16];
  in[0] = 0;                       // pulse
  in[1] = 0xA5;                    // volume A, decreasing, period 5
  in[4] = 0xDE;                    // sweep byte, ignored
  in[7] = 0x83;                    // 50% duty, left and right
  s[0x2040 + 4] = 1;               // instrument 4 allocated, noise, stable
  in = &s[0x3080 + 4 * 16];
  in[0] = 3; in[1] = 0xF3; in[2] = 1; in[7] = 0x02;

  const char* path = "test_import_lsdj_basic.lsdsng";
  REQUIRE(writeLsdsng(path, s, "SONGNAME"));
  Project p;
  projectInit(&p);
  REQUIRE(projectLoadLSDJ(&p, path) == 0);
  remove(path);

  CHECK(strcmp(p.title, "SONGNAME") == 0);
  CHECK(p.song[2][1] == 3);
  CHECK(p.song[0][0] == EMPTY_VALUE_16);
  CHECK(p.chains[3].rows[0].phrase == 7);
  CHECK((int8_t)p.chains[3].rows[0].transpose == -2);
  CHECK(p.chains[3].rows[1].phrase == EMPTY_VALUE_16);
  CHECK(p.phrases[7].rows[0].note == 24); // MIDI 36 = pitch index 24
  CHECK(p.phrases[7].rows[4].note == 24 + 0x1D - 1);
  CHECK(p.phrases[7].rows[0].instrument == 2);
  CHECK(p.phrases[7].rows[1].note == EMPTY_VALUE_8);
  CHECK(p.tickRate == doctest::Approx(60.0f)); // 150 bpm * 0.4

  const Instrument& lead = p.instruments[2];
  CHECK(lead.type == InstrumentType::GBPulse);
  CHECK(strcmp(lead.name, "LEAD") == 0);
  CHECK(lead.chip.simpleChip.mode == 2);
  CHECK(lead.chip.simpleChip.envelopeInitial == 10);
  CHECK(lead.chip.simpleChip.envelopeIncrease == 0);
  CHECK(lead.chip.simpleChip.envelopePeriod == 5);
  CHECK(lead.chip.simpleChip.sweepPeriod == 0);
  CHECK(p.phrases[7].rows[0].volume == PHRASE_VOLUME_MAX);
  CHECK(lead.pan == 128);

  const Instrument& noise = p.instruments[4];
  CHECK(noise.type == InstrumentType::GBNoise);
  CHECK(noise.chip.simpleChip.mode == 1);
  CHECK(noise.chip.simpleChip.envelopeInitial == 15);
  CHECK(noise.pan == 0);
  CHECK(strcmp(noise.name, "LSDJ 04") == 0);
  projectFree(&p);
}

TEST_CASE("rejects files that are not LSDj songs") {
  const char* path = "test_import_lsdj_bad.lsdsng";
  FILE* f = fopen(path, "wb");
  REQUIRE(f);
  uint8_t junk[64] = {1, 2, 3};
  fwrite(junk, 1, sizeof(junk), f);
  fclose(f);
  Project p;
  projectInit(&p);
  CHECK(projectLoadLSDJ(&p, path) != 0);
  remove(path);
  CHECK(projectLoadLSDJ(&p, "does_not_exist.lsdsng") != 0);
  projectFree(&p);
}


TEST_CASE("compression round trip, including escaped bytes and runs") {
  std::vector<uint8_t> song(SONG_SIZE);
  for (size_t i = 0; i < song.size(); i++) song[i] = (uint8_t)((i * 7919) >> 3);
  song[100] = 0xC0; song[101] = 0xE0; song[102] = 0xC0; song[103] = 0xC0;
  memset(&song[5000], 0xAB, 700);
  std::vector<uint8_t> packed(LSDJ_MAX_BLOCKS * LSDJ_BLOCK_SIZE);
  size_t n = lsdjCompress(song.data(), packed.data());
  REQUIRE(n > 0);
  CHECK(n % LSDJ_BLOCK_SIZE == 0);
  std::vector<uint8_t> back(SONG_SIZE);
  REQUIRE(lsdjDecompress(packed.data(), n, back.data()));
  CHECK(back == song);
}

TEST_CASE("export into a template, then import again") {
  auto s = makeSong();
  memcpy(&s[0x6000], "WAVEDATA", 8); // template content outside the song stays
  const char* tpl = "test_export_lsdj_template.lsdsng";
  const char* out = "test_export_lsdj_out.lsdsng";
  REQUIRE(writeLsdsng(tpl, s, "TEMPLATE"));

  Project p;
  projectInit(&p);
  strcpy(p.title, "My song");
  p.tickRate = 60.0f;
  p.song[0][0] = 5;
  p.song[1][3] = 6;
  p.song[2][4] = 7; // fifth track: dropped
  chainClear(&p.chains[5]);
  p.chains[5].rows[0].phrase = 9;
  p.chains[5].rows[0].transpose = 0xFE;
  phraseClear(&p.phrases[9]);
  for (int r = 0; r < 16; r++) initEmptyPhraseRow(&p.phrases[9].rows[r]);
  p.phrases[9].rows[0].note = 30;   // index 30 = LSDj note 7
  p.phrases[9].rows[0].instrument = 3;
  p.phrases[9].rows[2].note = 0;    // below the LSDj range: clamped to 1
  p.phrases[9].rows[2].instrument = 3;
  getInstrumentFunctions(InstrumentType::GBPulse).init(&p.instruments[3]);
  p.instruments[3].chip.simpleChip.mode = 3;
  p.instruments[3].chip.simpleChip.envelopeInitial = 12;
  p.instruments[3].chip.simpleChip.envelopeIncrease = 1;
  p.instruments[3].chip.simpleChip.envelopePeriod = 2;
  snprintf(p.instruments[3].name, sizeof(p.instruments[3].name), "Lead 1");

  REQUIRE(projectExportLSDJ(&p, tpl, out) == 0);

  Project q;
  projectInit(&q);
  REQUIRE(projectLoadLSDJ(&q, out) == 0);
  CHECK(strcmp(q.title, "MYSONG") == 0);
  CHECK(q.tickRate == doctest::Approx(60.0f));
  CHECK(q.song[0][0] == 5);
  CHECK(q.song[1][3] == 6);
  CHECK(q.song[2][4] == EMPTY_VALUE_16);
  CHECK(q.chains[5].rows[0].phrase == 9);
  CHECK((int8_t)q.chains[5].rows[0].transpose == -2);
  CHECK(q.phrases[9].rows[0].note == 30);
  CHECK(q.phrases[9].rows[0].instrument == 3);
  CHECK(q.phrases[9].rows[1].note == EMPTY_VALUE_8);
  CHECK(q.phrases[9].rows[2].note == 24);
  CHECK(q.instruments[3].type == InstrumentType::GBPulse);
  CHECK(strcmp(q.instruments[3].name, "LEAD") == 0);
  CHECK(q.instruments[3].chip.simpleChip.mode == 3);
  CHECK(q.instruments[3].chip.simpleChip.envelopeInitial == 12);
  CHECK(q.instruments[3].chip.simpleChip.envelopeIncrease == 1);
  CHECK(q.instruments[3].chip.simpleChip.envelopePeriod == 2);

  // A chain above LSDj's 128 makes the export fail
  p.song[3][0] = 130;
  CHECK(projectExportLSDJ(&p, tpl, out) != 0);
  CHECK(projectExportLSDJ(&p, "does_not_exist.lsdsng", out) != 0);

  remove(tpl);
  remove(out);
  projectFree(&p);
  projectFree(&q);
}

}
