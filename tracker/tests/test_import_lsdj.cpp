#include "doctest.h"

#include <import/import_lsdj.h>
#include <chipnomad_lib.h>

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

// Compression: C0 runs, escaped C0/E0 and a jump to the next 512 byte block
// every time a block is nearly full, like LSDj does.
std::vector<uint8_t> compress(const std::vector<uint8_t>& song) {
  std::vector<uint8_t> out;
  size_t blockStart = 0, i = 0;
  uint8_t block = 1;
  while (i < song.size()) {
    if (out.size() - blockStart > 500) {
      out.push_back(0xE0);
      out.push_back(block++);
      while ((out.size() % 512) != 0) out.push_back(0xFF);
      blockStart = out.size();
    }
    size_t run = 1;
    while (i + run < song.size() && song[i + run] == song[i] && run < 255) run++;
    if (run >= 4) {
      out.insert(out.end(), {0xC0, song[i], (uint8_t)run});
      i += run;
    } else {
      if (song[i] == 0xC0) out.push_back(0xC0);
      if (song[i] == 0xE0) out.push_back(0xE0);
      out.push_back(song[i]);
      i++;
    }
  }
  out.push_back(0xE0);
  out.push_back(0xFF);
  return out;
}

bool writeLsdsng(const char* path, const std::vector<uint8_t>& song, const char* name) {
  FILE* f = fopen(path, "wb");
  if (!f) return false;
  uint8_t header[9] = {0};
  memcpy(header, name, strlen(name));
  header[8] = 18;
  fwrite(header, 1, 9, f);
  auto c = compress(song);
  fwrite(c.data(), 1, c.size(), f);
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
  in[4] = 0xFF;                    // no sweep
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

}
