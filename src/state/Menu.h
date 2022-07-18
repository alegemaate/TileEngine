#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <string>
#include <vector>

#include "./engine/State.h"

#include "../tile/TileMap.h"
#include "../util/Bitmap.h"
#include "../util/Sound.h"

#include "../globals.h"
#include "../tools.h"

class Menu : public State {
 public:
  // Main loop functions
  Menu();
  ~Menu() = default;

  void update(StateEngine* engine);
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
  int step;

  int old_mouse_x;
  int old_mouse_y;

  bool mouse_control;

  // Live background
  TileMap* tile_map;
  string scrollDirection;

  // Menu
  int selectorY, selectorX, newSelectorY;
  bool menuOpen;

  ALLEGRO_FONT* font;
};

#endif  // MENU_H
