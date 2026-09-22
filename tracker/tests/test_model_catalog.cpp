#include "doctest.h"
#include "screens/model_catalog.h"
#include <string.h>

TEST_CASE("every Braids and Plaits model belongs to exactly one category") {
  CHECK(modelCatalogsValid());
}

TEST_CASE("Plaits-Alt contains exactly its 24 supplemental engines") {
  int count = 0;
  bool seen[24] = {};
  for (int category = 0; category < plaitsAltCategoryCount; ++category) {
    for (int item = 0; item < plaitsAltCategories[category].childCount; ++item) {
      int value = plaitsAltCategories[category].children[item].value;
      CHECK(value >= 0);
      CHECK(value < 24);
      CHECK_FALSE(seen[value]);
      seen[value] = true;
      ++count;
    }
  }
  CHECK(count == 24);
}

TEST_CASE("model names come from the catalog") {
  CHECK(strcmp(modelCatalogName(InstrumentType::Braids, 46), "DIGI-MOD") == 0);
  CHECK(strcmp(modelCatalogName(InstrumentType::Plaits, 5), "WAVE TERRAIN") == 0);
  CHECK(strcmp(modelCatalogName(InstrumentType::PlaitsAlt, 23), "PHASE FLOCK") == 0);
  CHECK(strcmp(modelCatalogName(InstrumentType::Sintered, 5), "MELT") == 0);
  CHECK(strcmp(modelCatalogName(InstrumentType::Plaits, 24), "UNKNOWN") == 0);
}

TEST_CASE("model-specific modulation labels and MME stick defaults") {
  Instrument instrument = {};
  getInstrumentFunctions(InstrumentType::Sintered).init(&instrument);
  instrument.chip.sintered.model = SinteredModel::melt;
  CHECK(strcmp(instrumentModDestinationNameForInstrument(&instrument, 5), "Ratio") == 0);
  CHECK(strcmp(instrumentModDestinationNameForInstrument(&instrument, 6), "Chaos") == 0);
  CHECK(strcmp(instrumentModDestinationNameForInstrument(&instrument, 8), "Drive") == 0);
  const int sinteredDestinations[] = {3, 4, 5, 6};
  for (int slot = 0; slot < 4; ++slot) {
    CHECK(instrument.modulation[slot].type == ModulationType::StickLinear);
    CHECK(instrument.modulation[slot].destination == sinteredDestinations[slot]);
    CHECK(instrument.modulation[slot].amount == 80);
    CHECK(instrument.modulation[slot].p1 == slot);
  }

  getInstrumentFunctions(InstrumentType::MME).init(&instrument);
  instrument.chip.mme.model = MMEModel::sync;
  CHECK(instrument.chip.mme.waves == 80);
  CHECK(strcmp(instrumentModDestinationNameForInstrument(&instrument, 5), "SyncAmt") == 0);
  instrument.chip.mme.model = MMEModel::ring;
  CHECK(strcmp(instrumentModDestinationNameForInstrument(&instrument, 6), "RingType") == 0);
  const int mmeDestinations[] = {3, 4, 5, 6};
  for (int slot = 0; slot < 4; ++slot) {
    CHECK(instrument.modulation[slot].type == ModulationType::StickLinear);
    CHECK(instrument.modulation[slot].destination == mmeDestinations[slot]);
    CHECK(instrument.modulation[slot].amount == 80);
    CHECK(instrument.modulation[slot].p1 == slot);
  }

  getInstrumentFunctions(InstrumentType::Sample).init(&instrument);
  const int sampleDestinations[] = {3, 4, 6, 5};
  for (int slot = 0; slot < 4; ++slot)
    CHECK(instrument.modulation[slot].destination == sampleDestinations[slot]);

  getInstrumentFunctions(InstrumentType::AChChid).init(&instrument);
  const int achchidDestinations[] = {3, 4, 5, 6};
  for (int slot = 0; slot < 4; ++slot)
    CHECK(instrument.modulation[slot].destination == achchidDestinations[slot]);
}
