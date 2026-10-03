#include "seed_assets.h"
#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int vitaCopySeedIfMissing(const char* source, const char* destination) {
  // Avoid std::filesystem::copy_file: the pinned Vita runtime fails after
  // creating its destination. Basic newlib read/write do not require chmod,
  // inode equivalence, or filebuf support from that copy implementation.
  const int input = open(source, O_RDONLY);
  if (input < 0) return errno;
  const int output = open(destination, O_WRONLY | O_CREAT | O_EXCL, 0666);
  if (output < 0) {
    int error = errno;
    if (error == EEXIST) {
      struct stat existing{};
      if (stat(destination, &existing) == 0 && S_ISREG(existing.st_mode)) error = 0;
    }
    close(input);
    return error;
  }
  int error = 0;
  char buffer[16 * 1024];
  for (;;) {
    const auto count = read(input, buffer, sizeof(buffer));
    if (count < 0) {
      if (errno == EINTR) continue;
      error = errno;
      break;
    }
    if (count == 0) break;
    ssize_t offset = 0;
    while (offset < count) {
      const auto written = write(output, buffer + offset, count - offset);
      if (written < 0 && errno == EINTR) continue;
      if (written <= 0) { error = written < 0 ? errno : EIO; break; }
      offset += written;
    }
    if (error) break;
  }
  if (close(input) < 0 && !error) error = errno;
  if (close(output) < 0 && !error) error = errno;
  // Only this invocation's O_EXCL-created file is eligible for cleanup.
  if (error) unlink(destination);
  return error;
}
