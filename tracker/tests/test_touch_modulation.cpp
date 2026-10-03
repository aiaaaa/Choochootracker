#include "doctest.h"
#include "chipnomad_lib.h"
#include "touch_modulation.h"
#include "playback_modulation.h"
#include "project_utils.h"
#include "app.h"
#include <limits>
#include <memory>
#include <filesystem>
#include <chrono>

namespace {
using E=TouchModInput::Event;
struct TouchFixture {
  TouchModInput input;
  TouchFixture() { touchModReset(); chipnomadSetLiveStickEnabled(1); }
  ~TouchFixture() { touchModReset(); chipnomadSetLiveStickEnabled(0); }
};
static_assert(int(ModulationType::ADSR)==0 && int(ModulationType::StickLinear)==5 &&
  int(ModulationType::StickVelocity)==6 && int(ModulationType::StickRate)==7 &&
  int(ModulationType::FrontTouch)==8 && int(ModulationType::RearTouch)==9,
  "Serialized MOD source collision: reconcile upstream IDs, never renumber songs");
}
TEST_CASE_FIXTURE(TouchFixture,"Touch panels separate overlapping IDs and normalize active axes") {
  REQUIRE(input.event(0,7,E::down,-2,2,true));
  REQUIRE(input.event(1,7,E::down,2,-2,true));
  CHECK(touchModOutput(0,0,127)==-32385);
  CHECK(touchModOutput(0,1,127)==-32385);
  CHECK(touchModOutput(1,0,127)==32385);
  CHECK(touchModOutput(1,1,127)==32385);
  CHECK(touchModOutput(0,2,127)==32385);
  CHECK(touchModOutput(1,2,-127)==-32385);
  input.event(0,7,E::up,0,0,true);
  for(int axis=0;axis<3;++axis) CHECK(touchModOutput(0,axis,127)==0);
  CHECK(touchModOutput(1,2,127)==32385);
  input.event(1,7,E::motion,0.5f,0.5f,true);
  CHECK(touchModOutput(1,0,127)==0);
  CHECK(touchModOutput(1,1,127)==0);
  input.event(1,7,E::motion,std::numeric_limits<float>::quiet_NaN(),0,true);
  CHECK(touchModOutput(1,2,127)==0);
}
TEST_CASE_FIXTURE(TouchFixture,"Touch primary cannot jump to a resting finger and rear remains independent") {
  input.event(0,1,E::down,1,0,true);
  CHECK_FALSE(input.event(0,2,E::down,0,1,true));
  input.event(0,1,E::up,1,0,true);
  CHECK_FALSE(input.event(0,2,E::motion,0,1,true));
  CHECK_FALSE(input.event(0,3,E::down,0,1,true));
  CHECK(touchModOutput(0,2,127)==0);
  CHECK(input.event(1,2,E::down,0,1,true));
  input.event(0,2,E::up,0,0,true); input.event(0,3,E::up,0,0,true);
  CHECK(input.event(0,4,E::down,1,0,true));
}
TEST_CASE_FIXTURE(TouchFixture,"Touch gesture ownership is fixed at down and resets require lift") {
  CHECK_FALSE(input.event(0,1,E::down,1,0,false));
  CHECK_FALSE(input.event(0,1,E::motion,1,0,true));
  CHECK(touchModOutput(0,2,127)==0);
  input.event(0,1,E::up,1,0,true);
  CHECK(input.event(0,1,E::down,1,0,true));
  appResetInputState(); // Same invalidation used by suspend/focus/project replacement.
  CHECK(touchModOutput(0,0,127)==0);
  CHECK_FALSE(input.event(0,1,E::motion,1,0,true));
  input.event(0,1,E::up,1,0,true);
  REQUIRE(input.event(0,2,E::down,1,0,true));
  chipnomadSetLiveStickEnabled(0); chipnomadSetLiveStickEnabled(1);
  CHECK(touchModOutput(0,2,127)==0);
  CHECK_FALSE(input.event(0,2,E::motion,1,0,true));
}
TEST_CASE_FIXTURE(TouchFixture,"Touch uses ordinary MOD scaling including Insert FX destinations") {
  input.event(0,1,E::down,1,0,true);
  for(auto type:{ModulationType::FrontTouch,ModulationType::RearTouch}) {
    Modulation mod{}; mod.type=type; mod.amount=64; mod.p1=0;
    PlaybackModState state{}; playbackModInit(&state,&mod); playbackModNext(&state);
    CHECK(state.outValue==(type==ModulationType::FrontTouch?16320:0));
    CHECK(modulationIsAdditive(type));
    CHECK(playbackModScaleToRange(state.outValue,255)==(type==ModulationType::FrontTouch?129:0));
  }
  CHECK(touchModOutput(2,0,127)==0);
  CHECK(touchModOutput(0,255,127)==0);
}
TEST_CASE_FIXTURE(TouchFixture,"Touch assignments survive project persistence without a device") {
  auto project=std::make_unique<Project>(); projectInitAY(project.get());
  auto& inst=project->instruments[0]; getInstrumentFunctions(InstrumentType::Sample).init(&inst);
  for(int i=0;i<2;++i) {
    auto& mod=inst.modulation[i]; mod.type=i?ModulationType::RearTouch:ModulationType::FrontTouch;
    mod.p1=i?2:1; mod.amount=63;
    mod.destination=getInstrumentFunctions(inst.type).modDestinationsCount+1+genericModFirstInsert;
  }
  auto path=std::filesystem::temp_directory_path()/("cct-touch-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count())+".cct");
  REQUIRE(projectSave(project.get(),path.c_str())==0); projectFree(project.get());
  REQUIRE(projectLoad(project.get(),path.c_str())==0);
  touchModReset();
  for(int i=0;i<2;++i) {
    auto& mod=project->instruments[0].modulation[i];
    CHECK(mod.type==(i?ModulationType::RearTouch:ModulationType::FrontTouch));
    CHECK(mod.p1==(i?2:1)); CHECK(mod.amount==63);
    CHECK(instrumentGenericModDestination(InstrumentType::Sample,mod.destination)==genericModFirstInsert);
    PlaybackModState state{}; playbackModInit(&state,&mod); playbackModNext(&state); CHECK(state.outValue==0);
  }
  projectFree(project.get()); std::filesystem::remove(path);
}
