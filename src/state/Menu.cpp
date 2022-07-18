#include "Menu.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <string>

#include "../util/DisplayMode.h"
#include "../util/KeyListener.h"
#include "../util/Logger.h"
#include "../util/MouseListener.h"

Menu::Menu() {
  // Load images
  menu = Bitmap("images/gui/menu.png");
  menuselect = Bitmap("images/gui/menuSelector.png");
  help = Bitmap("images/gui/help.png");
  cursor[0] = Bitmap("images/gui/cursor1.png");
  cursor[1] = Bitmap("images/gui/cursor2.png");
  levelSelectLeft = Bitmap("images/gui/levelSelectLeft.png");
  levelSelectRight = Bitmap("images/gui/levelSelectRight.png");
  levelSelectNumber = Bitmap("images/gui/levelSelectNumber.png");
  copyright = Bitmap("images/gui/copyright.png");
  credits = Bitmap("images/gui/credits.png");

  // Load sound effects
  click = Sound("sounds/click.wav");

  // Load font
  font = al_load_ttf_font("fonts/opensans.ttf", 24, 0);

  // Sets Font
  if (!font) {
    Logger::fatal(
        "Cannot find font fonts/opensans.ttf \n Please check your files and "
        "try again");
  }

  // Variables
  newSelectorY = DisplayMode::getDrawHeight() - 323;
  selectorY = DisplayMode::getDrawHeight() - 323;
  selectorX = 60;
  mouse_control = false;
  selectorHovering = 0;

  // Create map for live background
  tile_map = new TileMap("data/levels/level_01");

  // Set background scroll direction
  scrollDirection = "right";
  tile_map->y =
      random(0, (tile_map->height * 64) - DisplayMode::getDrawHeight());
  tile_map->x = 0;

  levelOn = 0;
}

void Menu::update(StateEngine* engine) {
  // Disable/Enable mouse control on input
  if (KeyListener::key[ALLEGRO_KEY_UP] || KeyListener::key[ALLEGRO_KEY_DOWN] ||
      KeyListener::key[ALLEGRO_KEY_LEFT] ||
      KeyListener::key[ALLEGRO_KEY_RIGHT] || KeyListener::key[ALLEGRO_KEY_W] ||
      KeyListener::key[ALLEGRO_KEY_A] || KeyListener::key[ALLEGRO_KEY_S] ||
      KeyListener::key[ALLEGRO_KEY_D]) {
    mouse_control = false;
  } else if (MouseListener::mouse_x != old_mouse_x ||
             MouseListener::mouse_y != old_mouse_y) {
    mouse_control = true;
  }

  old_mouse_x = MouseListener::mouse_x;
  old_mouse_y = MouseListener::mouse_y;

  menuOpen = false;

  // Move around live background
  if (scrollDirection == "right") {
    if (tile_map->x + 1 <
        (tile_map->width * 64 - DisplayMode::getScaleWidth())) {
      tile_map->x += 1;
    } else {
      scrollDirection = "left";
    }
  } else if (scrollDirection == "left") {
    if (tile_map->x - 1 > 0) {
      tile_map->x -= 1;
    } else {
      scrollDirection = "right";
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
  if ((KeyListener::key[ALLEGRO_KEY_W] || KeyListener::key[ALLEGRO_KEY_UP]) &&
      step > 10) {
    if (selectorHovering != 0)
      selectorHovering--;
    else
      selectorHovering = 3;
    step = 0;
  }
  if ((KeyListener::key[ALLEGRO_KEY_S] || KeyListener::key[ALLEGRO_KEY_DOWN]) &&
      step > 10) {
    if (selectorHovering != 3)
      selectorHovering++;
    else
      selectorHovering = 0;
    step = 0;
  }

  // Hover play
  if ((mouse_control &&
       collisionAny(MouseListener::mouse_x, MouseListener::mouse_x, 60, 270,
                    MouseListener::mouse_y, MouseListener::mouse_y,
                    DisplayMode::getDrawHeight() - 323,
                    DisplayMode::getDrawHeight() - 278)) ||
      (!mouse_control && (selectorHovering == 0))) {
    if (newSelectorY != DisplayMode::getDrawHeight() - 323) {
      newSelectorY = DisplayMode::getDrawHeight() - 323;
      selectorX = 60;
      click.play();
    }
  }

  // Hover edit
  else if ((mouse_control &&
            collisionAny(MouseListener::mouse_x, MouseListener::mouse_x, 60,
                         270, MouseListener::mouse_y, MouseListener::mouse_y,
                         DisplayMode::getDrawHeight() - 260,
                         DisplayMode::getDrawHeight() - 215)) ||
           (!mouse_control && (selectorHovering == 1))) {
    if (newSelectorY != DisplayMode::getDrawHeight() - 260) {
      newSelectorY = DisplayMode::getDrawHeight() - 260;
      selectorX = 60;
      click.play();
    }
  }

  // Hover help
  else if ((mouse_control &&
            collisionAny(MouseListener::mouse_x, MouseListener::mouse_x, 60,
                         270, MouseListener::mouse_y, MouseListener::mouse_y,
                         DisplayMode::getDrawHeight() - 197,
                         DisplayMode::getDrawHeight() - 152)) ||
           (!mouse_control && (selectorHovering == 2))) {
    if (newSelectorY != DisplayMode::getDrawHeight() - 197) {
      newSelectorY = DisplayMode::getDrawHeight() - 197;
      selectorX = 60;
      click.play();
    }
    menuOpen = true;
  }

  // Hover exit
  else if ((mouse_control &&
            collisionAny(MouseListener::mouse_x, MouseListener::mouse_x, 60,
                         270, MouseListener::mouse_y, MouseListener::mouse_y,
                         DisplayMode::getDrawHeight() - 132,
                         DisplayMode::getDrawHeight() - 87)) ||
           (!mouse_control && (selectorHovering == 3))) {
    if (newSelectorY != DisplayMode::getDrawHeight() - 132) {
      newSelectorY = DisplayMode::getDrawHeight() - 132;
      selectorX = 60;
      click.play();
    }
  }

  // Select button
  // level select left
  if (((collisionAny(MouseListener::mouse_x, MouseListener::mouse_x,
                     DisplayMode::getDrawWidth() - 180,
                     DisplayMode::getDrawWidth() - 140, MouseListener::mouse_y,
                     MouseListener::mouse_y, 80, 120) &&
        MouseListener::mouse_button & 1) ||
       (KeyListener::key[ALLEGRO_KEY_A] ||
        KeyListener::key[ALLEGRO_KEY_LEFT])) &&
      step > 10) {
    click.play();
    if (levelOn > 0) {
      levelOn--;
    } else {
      levelOn = 3;
    }
    if (levelOn == 0) {
      tile_map->load("data/levels/level_01");
    } else if (levelOn == 1) {
      tile_map->load("data/levels/level_test");
    } else if (levelOn == 2) {
      tile_map->load("data/saves/danny");
    } else if (levelOn == 3) {
      tile_map->load("data/saves/dannyII");
    }
    tile_map->y =
        random(0, (tile_map->height * 64) - DisplayMode::getDrawHeight());
    tile_map->x = 0;
    step = 0;
  }

  if (((collisionAny(MouseListener::mouse_x, MouseListener::mouse_x,
                     DisplayMode::getDrawWidth() - 80,
                     DisplayMode::getDrawWidth() - 40, MouseListener::mouse_y,
                     MouseListener::mouse_y, 80, 120) &&
        MouseListener::mouse_button & 1) ||
       (KeyListener::key[ALLEGRO_KEY_D] ||
        KeyListener::key[ALLEGRO_KEY_RIGHT])) &&
      step > 10) {
    // level select right
    click.play();
    if (levelOn < 3) {
      levelOn++;
    } else {
      levelOn = 0;
    }
    if (levelOn == 0) {
      tile_map->load("data/levels/level_01");
    } else if (levelOn == 1) {
      tile_map->load("data/levels/level_test");
    } else if (levelOn == 2) {
      tile_map->load("data/saves/danny");
    } else if (levelOn == 3) {
      tile_map->load("data/saves/dannyII");
    }
    tile_map->y =
        random(0, (tile_map->height * 64) - DisplayMode::getDrawHeight());
    tile_map->x = 0;
    step = 0;
  }
  // Start
  if (((collisionAny(MouseListener::mouse_x, MouseListener::mouse_x, 60, 270,
                     MouseListener::mouse_y, MouseListener::mouse_y,
                     DisplayMode::getDrawHeight() - 323,
                     DisplayMode::getDrawHeight() - 278) &&
        MouseListener::mouse_button & 1) ||
       ((KeyListener::key[ALLEGRO_KEY_ENTER]) && selectorHovering == 0)) &&
      step > 10) {
    engine->setNextState(StateEngine::STATE_GAME);
  }
  // Edit
  if (((collisionAny(MouseListener::mouse_x, MouseListener::mouse_x, 60, 270,
                     MouseListener::mouse_y, MouseListener::mouse_y,
                     DisplayMode::getDrawHeight() - 260,
                     DisplayMode::getDrawHeight() - 215) &&
        MouseListener::mouse_button & 1) ||
       ((KeyListener::key[ALLEGRO_KEY_ENTER]) && selectorHovering == 1)) &&
      step > 10) {
    engine->setNextState(StateEngine::STATE_EDIT);
  }
  // Quit
  if (((collisionAny(MouseListener::mouse_x, MouseListener::mouse_x, 60, 270,
                     MouseListener::mouse_y, MouseListener::mouse_y,
                     DisplayMode::getDrawHeight() - 132,
                     DisplayMode::getDrawHeight() - 87) &&
        MouseListener::mouse_button & 1) ||
       ((KeyListener::key[ALLEGRO_KEY_ENTER]) && selectorHovering == 3)) &&
      step > 10) {
    engine->setNextState(StateEngine::STATE_EXIT);
  }
  step++;
}

void Menu::draw() {
  // Draw background to screen
  al_draw_filled_rectangle(0, 0, DisplayMode::getDrawWidth(),
                           DisplayMode::getDrawHeight(),
                           al_map_rgb(255, 255, 255));

  // // Draw live background
  tile_map->draw_map();

  // Overlay
  credits.draw(0, 0);
  menu.draw(0, DisplayMode::getDrawHeight() - 461);
  menuselect.draw(selectorX, selectorY);

  // Level selection
  levelSelectLeft.draw(DisplayMode::getDrawWidth() - 180, 80);
  levelSelectNumber.draw(DisplayMode::getDrawWidth() - 160, 80);
  al_draw_text(font, al_map_rgb(0, 0, 0), DisplayMode::getDrawWidth() - 112, 73,
               0, std::to_string(levelOn + 1).c_str());
  levelSelectRight.draw(DisplayMode::getDrawWidth() - 80, 80);

  // Hover select left
  if (collisionAny(MouseListener::mouse_x, MouseListener::mouse_x,
                   DisplayMode::getDrawWidth() - 180,
                   DisplayMode::getDrawWidth() - 140, MouseListener::mouse_y,
                   MouseListener::mouse_y, 80, 120)) {
    levelSelectLeft.draw(DisplayMode::getDrawWidth() - 180, 80);
  }
  // Hover select right
  if (collisionAny(MouseListener::mouse_x, MouseListener::mouse_x,
                   DisplayMode::getDrawWidth() - 80,
                   DisplayMode::getDrawWidth() - 40, MouseListener::mouse_y,
                   MouseListener::mouse_y, 80, 120)) {
    levelSelectRight.draw(DisplayMode::getDrawWidth() - 80, 80);
  }

  // Cursor
  cursor[0].draw(MouseListener::mouse_x, MouseListener::mouse_y);

  // Select button
  if (MouseListener::mouse_button & 1 || KeyListener::key[ALLEGRO_KEY_ENTER]) {
    if (selectorY == 610) {
      do {
        menu.draw(0, 0);
        help.draw(0, 0);
      } while (!KeyListener::key[ALLEGRO_KEY_ESCAPE] &&
               !(MouseListener::mouse_button & 1));
    }
  }
  if (menuOpen) {
    help.draw(0, 0);
  }
  copyright.draw(DisplayMode::getDrawWidth() - 350,
                 DisplayMode::getDrawHeight() - 40);
  cursor[0].draw(MouseListener::mouse_x, MouseListener::mouse_y);
}
