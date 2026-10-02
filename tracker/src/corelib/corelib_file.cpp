#include "corelib_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#endif

#if defined(ANDROID_BUILD) || defined(MACOS_BUILD)
// Helper: Create directory recursively
static void createDirectoryRecursive(const char* path) {
  char tmp[4096];
  char* p = NULL;
  size_t len;

  snprintf(tmp, sizeof(tmp), "%s", path);
  len = strlen(tmp);
  if (tmp[len - 1] == '/') tmp[len - 1] = 0;

  for (p = tmp + 1; *p; p++) {
    if (*p == '/') {
      *p = 0;
      #ifdef _WIN32
      mkdir(tmp);
      #else
      mkdir(tmp, 0755);
      #endif
      *p = '/';
    }
  }
  #ifdef _WIN32
  mkdir(tmp);
  #else
  mkdir(tmp, 0755);
  #endif
}
#endif

int fileCreateDirectoryRecursive(const char* path) {
  if (path == NULL || path[0] == 0) return -1;
  if (fileDirectoryExists(path)) return 0;

  char tmp[4096];
  snprintf(tmp, sizeof(tmp), "%s", path);
  size_t len = strlen(tmp);
  if (len == 0) return -1;
  if (tmp[len - 1] == '/' || tmp[len - 1] == '\\') tmp[len - 1] = 0;
  if (tmp[0] == 0) return -1;

  // Create each path level in turn; existing levels report EEXIST and are fine
  for (char* p = tmp + 1; *p; p++) {
    if (*p == '/' || *p == '\\') {
      *p = 0;
      if (!fileDirectoryExists(tmp)) {
        if (fileCreateDirectory(tmp) != 0) return -1;
      }
      *p = '/';
    }
  }
  if (!fileDirectoryExists(tmp)) {
    if (fileCreateDirectory(tmp) != 0) return -1;
  }
  return 0;
}

int fileRename(const char* oldPath, const char* newPath) {
  if (oldPath == NULL || newPath == NULL || oldPath[0] == 0 || newPath[0] == 0) return -1;
#ifdef WEB_BUILD
  // The virtual FS has no rename; callers must treat failure as non-fatal
  (void)oldPath;
  (void)newPath;
  return -1;
#elif defined(_WIN32)
  return MoveFileA(oldPath, newPath) ? 0 : -1;
#else
  return rename(oldPath, newPath) == 0 ? 0 : -1;
#endif
}

int fileGetDefaultDirectory(char* buffer, int bufferSize) {
#ifdef ANDROID_BUILD
  extern int androidGetWorkspacePath(char*, int);
  return androidGetWorkspacePath(buffer, bufferSize);
#elif defined(MACOS_BUILD)
  const char* home = getenv("HOME");
  if (home) {
    snprintf(buffer, bufferSize, "%s/Library/Application Support/ChipNomad", home);
    createDirectoryRecursive(buffer);
  } else {
    snprintf(buffer, bufferSize, ".");
  }
  return 0;
#else
  return getcwd(buffer, bufferSize) ? 0 : -1;
#endif
}

void fileImportDocument(const char* mimeType, const char* relativeDirectory) {
#ifdef ANDROID_BUILD
  extern void androidOpenDocument(const char*, const char*);
  androidOpenDocument(mimeType, relativeDirectory);
#else
  (void)mimeType;
  (void)relativeDirectory;
#endif
}

void fileExportDocument(const char* path, const char* mimeType) {
#ifdef ANDROID_BUILD
  extern void androidSaveDocument(const char*, const char*);
  androidSaveDocument(path, mimeType);
#else
  (void)path;
  (void)mimeType;
#endif
}

int fileDirectoryExists(const char* path) {
  struct stat statBuf;
  return (stat(path, &statBuf) == 0 && S_ISDIR(statBuf.st_mode)) ? 1 : 0;
}

int fileCreateDirectory(const char* path) {
  #ifdef _WIN32
  return mkdir(path) == 0 ? 0 : -1;
  #else
  return mkdir(path, 0755) == 0 ? 0 : -1;
  #endif
}

int fileDelete(const char* path) {
  return remove(path) == 0 ? 0 : -1;
}

FileEntry* fileListDirectory(const char* path, const char* extension, int* entryCount) {
  DIR* dir = opendir(path);
  if (!dir) {
    *entryCount = 0;
    return NULL;
  }

  struct dirent* entry;
  int capacity = 100;
  int count = 0;
  FileEntry* entries = (FileEntry*)malloc(capacity * sizeof(FileEntry));

  if (!entries) {
    closedir(dir);
    *entryCount = 0;
    return NULL;
  }

  // Check first entry - if it's neither "." nor "..", insert ".." (unless at root)
  entry = readdir(dir);
  if (entry && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(path, "/") != 0) {
    strcpy(entries[0].name, "..");
    entries[0].isDirectory = 1;
    count = 1;
  }

  // Process entries using do-while to handle first entry
  if (entry) {
    do {
      // Skip hidden files and current directory
      if (entry->d_name[0] == '.') {
        // Allow ".." for parent directory
        if (strcmp(entry->d_name, "..") != 0) continue;
      }

      char fullPath[2048];
      snprintf(fullPath, sizeof(fullPath), "%s%s%s", path, PATH_SEPARATOR_STR, entry->d_name);

      struct stat statBuf;
      if (stat(fullPath, &statBuf) != 0) continue;

      int isDir = S_ISDIR(statBuf.st_mode);

      if (!isDir && extension && extension[0] != '\0') {
        char* dot = strrchr(entry->d_name, '.');
        if (!dot) continue;

        int match = 0;
        const char* pos = extension;
        while (pos) {
          if (strcasecmp(pos, dot) == 0 ||
          (strncasecmp(pos, dot, strlen(dot)) == 0 && pos[strlen(dot)] == ',')) {
            match = 1;
            break;
          }
          pos = strchr(pos, ',');
          if (pos) pos++;
        }
        if (!match) continue;
      }

      // Resize array if needed
      if (count >= capacity) {
        capacity *= 2;
        FileEntry* newEntries = (FileEntry*)realloc(entries, capacity * sizeof(FileEntry));
        if (!newEntries) {
          free(entries);
          closedir(dir);
          *entryCount = 0;
          return NULL;
        }
        entries = newEntries;
      }

      strncpy(entries[count].name, entry->d_name, 255);
      entries[count].name[255] = 0;
      entries[count].isDirectory = isDir;
      count++;
    } while ((entry = readdir(dir)));
  }

  closedir(dir);
  *entryCount = count;
  return entries;
}
