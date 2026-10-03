#include "doctest.h"
#include "screens.h"
#include "common.h"
#include "app_ui_mock.h"
#include <memory>
#include <string>
#include <initializer_list>

// Exercise the real MOD picker/parameter editing while keeping the standard
// test suite's screen boundary mocks for unrelated navigation.
extern const AppScreen testedTouchModulationScreen;
#define screenModulation testedTouchModulationScreen
#include "../src/screens/screen_modulation.cpp"
#undef screenModulation

TEST_CASE("MOD picker exposes independent touch sources and X Y Gate parameters") {
  auto saved=chipnomadState; auto state=std::make_unique<ChipNomadState>();
  chipnomadState=state.get(); projectInitAY(&state->project); initDefaultAppSettings();
  cInstrument=0; getInstrumentFunctions(InstrumentType::Sample).init(&state->project.instruments[0]);
  openSourcePopup(0);
  CHECK(std::string(sourceCategories[3].label)=="TOUCH");
  CHECK(paramCount(&state->project.instruments[0].modulation[0])>0);
  for(auto type:{ModulationType::FrontTouch,ModulationType::RearTouch}) {
    sourceSelected(int(type));
    auto& mod=state->project.instruments[0].modulation[0];
    CHECK(mod.type==type); CHECK(paramCount(&mod)==1); CHECK(mod.p1==0);
    CHECK(onEdit(0,3,CellEditAction::increase)); CHECK(mod.p1==1);
    CHECK(onEdit(0,3,CellEditAction::increase)); CHECK(mod.p1==2);
    CHECK(std::string(touchParameterNames[mod.p1])=="Gate  ");
    CHECK_FALSE(isCellValid(0,4));
  }
  projectFree(&state->project); chipnomadState=saved;
}
