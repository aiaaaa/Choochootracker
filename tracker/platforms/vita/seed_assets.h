#pragma once

// First-run seed only: never overwrite an existing file. Returns an errno value
// on failure (zero for a completed copy or a preserved existing regular file).
int vitaCopySeedIfMissing(const char* source, const char* destination);
