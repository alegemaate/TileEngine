#include "Menu.h"

#include <allegro5/allegro_primitives.h>
#include <string>

#include "../lib/display/DisplayMode.h"
#include "../lib/input/KeyListener.h"
#include "../lib/input/MouseListener.h"
#include "../lib/util/Logger.h"
#include "../tools.h"
#include "./Game.h"

Menu::Menu(KeyListener& keyboardListener,
           MouseListener& mouseListener,
           JoystickListener& joystickListener)
    : keyboardListener(keyboardListener),
      mouseListener(mouseListener),
      joystickListener(joystickListener) {}

void Menu::init() {
  // Load images
  menu = Bitmap("images/gui/menu.png");
  menuselect = Bitmap("images/gui/menuSelector.png");
  overlayHelp = Bitmap("images/gui/help.png");
  levelSelectLeft = Bitmap("images/gui/levelSelectLeft.png");
  levelSelectRight = Bitmap("images/gui/levelSelectRight.png");
  levelSelectNumber = Bitmap("images/gui/levelSelectNumber.png");
  overlayCopyright = Bitmap("images/gui/copyright.png");
  overlayCredits = Bitmap("images/gui/credits.png");

  // Load sound effects
  click = Sound("sounds/click.wav");

  // Load font
  font = Font("fonts/opensans.ttf", 24);

  // Create map for live background
  tile_map = new TileMap("data/levels/level_01.txt");

  // Set background scroll direction
  scrollDirection = ScrollDirection::Right;
  tile_map->y =
      random(0, (tile_map->height * 64) - DisplayMode::getDrawHeight());
  tile_map->x = 0;

  level = 0;
}

void Menu::update(double delta) {
  tile_map->update(delta);

  // Disable/Enable mouse control on input
  if (keyboardListener.wasAnyPressed()) {
    mouseControl = false;
  } else if (mouseListener.didMove()) {
    mouseControl = true;
  }

  menuOpen = false;

  // Move around live background
  if (scrollDirection == ScrollDirection::Right) {
    if (tile_map->x + 1 <
        (tile_map->width * 64 - DisplayMode::getScaleWidth())) {
      tile_map->x += 1;
    } else {
      scrollDirection = ScrollDirection::Left;
    }
  } else if (scrollDirection == ScrollDirection::Left) {
    if (tile_map->x - 1 > 0) {
      tile_map->x -= 1;
    } else {
      scrollDirection = ScrollDirection::Right;
    }
  }

  // Move selector
  if (selectorY != newSelectorY) {
    int selectorVelocity = 0;
    if (newSelectorY != selectorY) {
      selectorVelocity = (newSelectorY - selectorY) / 6;
      if (newSelectorY - selectorY < 0) {
        selectorVelocity -= 1;
      } else if (newSelectorY - selectorY > 0) {
        selectorVelocity += 1;
      }
    }
    selectorY += selectorVelocity;
  }

  // Keyboard selecting
  if (keyboardListener.wasPressed(Key::DOWN)) {
    switch (selectedButton) {
      case MenuButton::Start:
        selectedButton = MenuButton::Edit;
        break;
      case MenuButton::Edit:
        selectedButton = MenuButton::Help;
        break;
      case MenuButton::Help:
        selectedButton = MenuButton::Exit;
        break;
      case MenuButton::Exit:
        selectedButton = MenuButton::Start;
        break;
    }
  }

  if (keyboardListener.wasPressed(Key::UP)) {
    switch (selectedButton) {
      case MenuButton::Start:
        selectedButton = MenuButton::Exit;
        break;
      case MenuButton::Edit:
        selectedButton = MenuButton::Start;
        break;
      case MenuButton::Help:
        selectedButton = MenuButton::Edit;
        break;
      case MenuButton::Exit:
        selectedButton = MenuButton::Help;
        break;
    }
  }

  // Mouse selecting
  isHoveringButton = false;
  if (buttonStart.collidesPoint(mouseListener.getX(), mouseListener.getY())) {
    selectedButton = MenuButton::Start;
    isHoveringButton = true;
  } else if (buttonEdit.collidesPoint(mouseListener.getX(),
                                      mouseListener.getY())) {
    selectedButton = MenuButton::Edit;
    isHoveringButton = true;
  } else if (buttonHelp.collidesPoint(mouseListener.getX(),
                                      mouseListener.getY())) {
    selectedButton = MenuButton::Help;
    isHoveringButton = true;
  } else if (buttonExit.collidesPoint(mouseListener.getX(),
                                      mouseListener.getY())) {
    selectedButton = MenuButton::Exit;
    isHoveringButton = true;
  }

  // Hover play
  if (selectedButton == MenuButton::Start) {
    if (newSelectorY != buttonStart.y) {
      newSelectorY = buttonStart.y;
      click.play();
    }
  }

  // Hover edit
  else if (selectedButton == MenuButton::Edit) {
    if (newSelectorY != buttonEdit.y) {
      newSelectorY = DisplayMode::getDrawHeight() - 260;
      click.play();
    }
  }

  // Hover help
  else if (selectedButton == MenuButton::Help) {
    if (newSelectorY != buttonHelp.y) {
      newSelectorY = DisplayMode::getDrawHeight() - 197;
      click.play();
    }
    menuOpen = true;
  }

  // Hover exit
  else if (selectedButton == MenuButton::Exit) {
    if (newSelectorY != buttonExit.y) {
      newSelectorY = DisplayMode::getDrawHeight() - 132;
      click.play();
    }
  }

  // Select button
  // level select left
  if ((buttonSelectLeft.collidesPoint(mouseListener.getX(),
                                      mouseListener.getY()) &&
       mouseListener.wasPressed(MouseButton::Left)) ||
      keyboardListener.wasPressed(Key::LEFT)) {
    click.play();
    if (level > 0) {
      level--;
    } else {
      level = 3;
    }
    if (level == 0) {
      tile_map->load("data/levels/level_01.txt");
    } else if (level == 1) {
      tile_map->load("data/levels/level_test.txt");
    } else if (level == 2) {
      tile_map->load("data/saves/danny.txt");
    } else if (level == 3) {
      tile_map->load("data/saves/dannyII.txt");
    }
    tile_map->y =
        random(0, (tile_map->height * 64) - DisplayMode::getDrawHeight());
    tile_map->x = 0;
  }

  if ((buttonSelectRight.collidesPoint(mouseListener.getX(),
                                       mouseListener.getY()) &&
       mouseListener.wasPressed(MouseButton::Left)) ||
      keyboardListener.wasPressed(Key::RIGHT)) {
    // level select right
    click.play();
    if (level < 3) {
      level++;
    } else {
      level = 0;
    }
    if (level == 0) {
      tile_map->load("data/levels/level_01.txt");
    } else if (level == 1) {
      tile_map->load("data/levels/level_test.txt");
    } else if (level == 2) {
      tile_map->load("data/saves/danny.txt");
    } else if (level == 3) {
      tile_map->load("data/saves/dannyII.txt");
    }

    tile_map->y =
        random(0, (tile_map->height * 64) - DisplayMode::getDrawHeight());
    tile_map->x = 0;
  }

  // Choose a button
  if ((mouseListener.wasPressed(MouseButton::Left) && isHoveringButton) ||
      keyboardListener.wasPressed(Key::ENTER)) {
    // Start
    if (selectedButton == MenuButton::Start) {
      Game::level = level;
      setNextState(ProgramState::GAME);
    }
    // Edit
    if (selectedButton == MenuButton::Edit) {
      setNextState(ProgramState::EDIT);
    }
    // Quit
    if (selectedButton == MenuButton::Exit) {
      setNextState(ProgramState::EXIT);
    }
  }
}

void Menu::draw() {
  // Draw background to screen
  al_draw_filled_rectangle(0, 0, DisplayMode::getDrawWidth(),
                           DisplayMode::getDrawHeight(),
                           al_map_rgb(255, 255, 255));

  // // Draw live background
  tile_map->draw_map();

  // Overlay
  overlayCredits.draw(0, 0);
  menu.draw(0, DisplayMode::getDrawHeight() - 461);
  menuselect.draw(selectorX, selectorY);

  // Level selection
  levelSelectLeft.draw(DisplayMode::getDrawWidth() - 180, 80);
  levelSelectNumber.draw(DisplayMode::getDrawWidth() - 160, 80);
  font.draw(DisplayMode::getDrawWidth() - 112, 73, al_map_rgb(0, 0, 0),
            std::to_string(level + 1));
  levelSelectRight.draw(DisplayMode::getDrawWidth() - 80, 80);

  // Hover select left
  if (collisionAny(mouseListener.getX(), mouseListener.getX(),
                   DisplayMode::getDrawWidth() - 180,
                   DisplayMode::getDrawWidth() - 140, mouseListener.getY(),
                   mouseListener.getY(), 80, 120)) {
    levelSelectLeft.draw(DisplayMode::getDrawWidth() - 180, 80);
  }
  // Hover select right
  if (collisionAny(mouseListener.getX(), mouseListener.getX(),
                   DisplayMode::getDrawWidth() - 80,
                   DisplayMode::getDrawWidth() - 40, mouseListener.getY(),
                   mouseListener.getY(), 80, 120)) {
    levelSelectRight.draw(DisplayMode::getDrawWidth() - 80, 80);
  }

  if (menuOpen) {
    overlayHelp.draw(0, 0);
  }

  overlayCopyright.draw(DisplayMode::getDrawWidth() - 350,
                        DisplayMode::getDrawHeight() - 40);
}
