#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>

#include "./engine/State.h"

#include "../tile/TileMap.h"
#include "../util/Bitmap.h"
#include "../util/Font.h"
#include "../util/JoystickListener.h"
#include "../util/KeyListener.h"
#include "../util/MouseListener.h"
#include "../util/Sound.h"

class Menu : public State {
 public:
  /**
   * @brief Construct a new Menu object
   *
   */
  Menu(KeyListener& keyboardListener,
       MouseListener& mouseListener,
       JoystickListener& joystickListener);

  void init() override;
  void update(double delta) override;
  void draw() override;

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

  KeyListener& keyboardListener;
  MouseListener& mouseListener;
  JoystickListener& joystickListener;
};

#endif  // MENU_H
