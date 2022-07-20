#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>

#include "../lib/assets/Bitmap.h"
#include "../lib/assets/Font.h"
#include "../lib/assets/Sound.h"
#include "../lib/input/JoystickListener.h"
#include "../lib/input/KeyListener.h"
#include "../lib/input/MouseListener.h"
#include "../lib/state/State.h"
#include "../lib/util/BoundingBox.h"
#include "../tile/TileMap.h"

enum class ScrollDirection { Left, Right };

enum class MenuButton { Start, Edit, Help, Exit };

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
  Bitmap levelSelectLeft{};
  Bitmap levelSelectRight{};
  Bitmap levelSelectNumber{};
  Bitmap menuselect{};
  Bitmap menu{};

  Bitmap overlayCopyright{};
  Bitmap overlayHelp{};
  Bitmap overlayCredits{};

  Sound click{};

  MenuButton selectedButton{MenuButton::Start};

  bool mouseControl{false};

  // Live background
  TileMap* tile_map{nullptr};
  ScrollDirection scrollDirection{ScrollDirection::Right};

  // Menu
  int selectorY{0};
  int selectorX{60};
  int newSelectorY{0};
  bool menuOpen{false};
  bool isHoveringButton{false};
  int level{0};

  Font font{};

  BoundingBox buttonStart{60, 637, 210, 45};
  BoundingBox buttonEdit{60, 700, 210, 45};
  BoundingBox buttonHelp{60, 763, 210, 45};
  BoundingBox buttonExit{60, 828, 210, 45};

  BoundingBox buttonSelectLeft{1100, 80, 40, 40};
  BoundingBox buttonSelectRight{1200, 80, 40, 40};

  KeyListener& keyboardListener;
  MouseListener& mouseListener;
  JoystickListener& joystickListener;
};

#endif  // MENU_H
