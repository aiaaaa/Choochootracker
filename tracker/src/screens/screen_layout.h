#pragma once
struct AppScreen;

// Scope occupies rows 0-1. The sidebar and message row remain in physical
// screen coordinates; only the page's content uses the two-row offset.
int screenScopeRows(const AppScreen* screen);
int screenVisibleRows(void);

class ScreenOverlayCoordinates {
 public:
  ScreenOverlayCoordinates();
  ~ScreenOverlayCoordinates();
 private:
  int previous_;
};
