#include "../../platforms/vita/seed_assets.h"
#include <cassert>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

static std::string contents(const char* path) {
  std::ifstream file(path, std::ios::binary);
  return {std::istreambuf_iterator<char>(file), {}};
}
int main() {
  char directory[] = "/tmp/cct-vita-seed-XXXXXX";
  assert(mkdtemp(directory));
  assert(chdir(directory) == 0);
  std::string data(65539, '\0');
  for (size_t i = 0; i < data.size(); ++i) data[i] = char(i % 251);
  std::ofstream("source", std::ios::binary).write(data.data(), data.size());
  assert(vitaCopySeedIfMissing("source", "copied") == 0);
  assert(contents("copied") == data);
  assert(vitaCopySeedIfMissing("source", "copied") == 0);
  assert(contents("copied") == data);
  std::ofstream("custom") << "user settings";
  std::ofstream("empty");
  assert(vitaCopySeedIfMissing("source", "custom") == 0);
  assert(contents("custom") == "user settings");
  assert(vitaCopySeedIfMissing("source", "empty") == 0);
  assert(contents("empty").empty());
  assert(vitaCopySeedIfMissing("absent", "missing-source") == ENOENT);
  assert(!std::filesystem::exists("missing-source"));
  std::filesystem::create_directory("occupied-directory");
  assert(vitaCopySeedIfMissing("source", "occupied-directory") != 0);
  assert(std::filesystem::is_directory("occupied-directory"));
  assert(vitaCopySeedIfMissing("empty", "empty-copy") == 0);
  assert(std::filesystem::is_regular_file("empty-copy"));
  // Force a real partial write, without depending on root permissions or /dev/full.
  const auto child = fork();
  assert(child >= 0);
  if (child == 0) {
    signal(SIGXFSZ, SIG_IGN);
    const rlimit limit{256, 256};
    assert(setrlimit(RLIMIT_FSIZE, &limit) == 0);
    assert(vitaCopySeedIfMissing("source", "failed-copy") == EFBIG);
    assert(!std::filesystem::exists("failed-copy"));
    _exit(0);
  }
  int status = 0;
  assert(waitpid(child, &status, 0) == child);
  assert(WIFEXITED(status) && WEXITSTATUS(status) == 0);
  assert(contents("source") == data);
  assert(chdir("/") == 0);
  std::filesystem::remove_all(directory);
  puts("Vita seed assets: binary copy, existing files, repeat and failed-write cleanup passed");
}
