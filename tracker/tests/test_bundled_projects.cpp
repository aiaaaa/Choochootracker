#include "doctest.h"
#include "chipnomad_lib.h"
#include "project_utils.h"
#include <memory>
#include <filesystem>
#include "../platforms/vita/stdio_check.h"

TEST_CASE("Vita startup stdio probe checks byte widths and numeric conversions") {
  CHECK(vitaStdioCompatible()); // Host libc here; also runs on Vita before project I/O.
}

TEST_CASE("Bundled native projects parse into standalone project data") {
  // These files already ship with the app; no network fixtures are acquired.
  unsigned count=0;
  for(const auto& entry: std::filesystem::directory_iterator("packaging/common/projects")) {
    if(entry.path().extension()!=".cct") continue;
    INFO(entry.path().string());
    auto project=std::make_unique<Project>();
    projectInitAY(project.get());
    const auto result=projectLoad(project.get(),entry.path().string().c_str());
    INFO(projectFileError);
    CHECK(result==0);
    if(result==0) CHECK(project->tracksCount>0);
    projectFree(project.get());
    ++count;
  }
  CHECK(count>=7);
}
