#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>

#include "./engine/State.h"

#include "../tile/TileMap.h"
#include "../util/Bitmap.h"
#include "../util/Font.h"
#include "../util/Sound.h"

class Menu : public State {
 public:
  // Main loop functions
  Menu();
  ~Menu() = default;

  void update(double delta);
  void draw();

 private:
  // Menu/GUI
  Bitmap levelSelectLeft;
  Bitmap levelSelectRight;
  Bitmap levelSelectNumber;
  Bitmap cursor[2];
  Bitmap menuselect;
  Bitmap menu;
  Bitmap help;
  Bitmap copyright;
  Bitmap credits;

  Sound click;

  int selectorHovering;

  bool mouseControl;

  // Live background
  TileMap* tile_map;
  std::string scrollDirection;

  // Menu
  int selectorY, selectorX, newSelectorY;
  bool menuOpen;

  Font font;
};

#endif  // MENU_H
