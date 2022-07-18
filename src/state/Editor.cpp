#include "Editor.h"

#include <allegro5/allegro_primitives.h>
#include <fstream>

#include "../util/DisplayMode.h"
#include "../util/KeyListener.h"
#include "../util/Logger.h"
#include "../util/MouseListener.h"

Editor::Editor()
    : selectedTileType(0), layer(0), saving(false), opening(false) {
  layer = 1;

  // Create map
  tile_map = new TileMap("data/templates/blank64x48");

  // Create example tile
  exampleTile = new Tile(0, tile_map->getIndex());
  exampleTile->setX(0);
  exampleTile->setY(0);

  // Load font
  font = al_load_ttf_font("fonts/opensans.ttf", 24, 0);

  // Sets Font
  if (!font) {
    Logger::fatal(
        "Cannot find font fonts/opensans.ttf \n Please check your files and "
        "try again");
  }

  edittext = ".txt";
  iter = edittext.begin();

  opening = false;
  saving = false;
}

void Editor::save() {
  // Name input
  if (KeyListener::anyKeyJustPressed) {
    int newkey = KeyListener::lastKeyPressed;
    char ASCII = newkey & 0xff;
    char scancode = newkey >> 8;

    // a character key was pressed; add it to the string
    if (ASCII >= 32 && ASCII <= 126 && edittext.length() < 25 &&
        scancode != ALLEGRO_KEY_SPACE) {
      // add the new char
      iter = edittext.insert(iter, ASCII);
      // increment both the caret and the iterator
      iter++;
    }
    // some other, "special" key was pressed; handle it here
    else {
      if (scancode == ALLEGRO_KEY_DELETE) {
        if (iter != edittext.end()) {
          iter = edittext.erase(iter);
        }
      }
      if (scancode == ALLEGRO_KEY_BACKSPACE) {
        if (iter != edittext.begin()) {
          iter--;
          iter = edittext.erase(iter);
        }
      }
      if (scancode == ALLEGRO_KEY_RIGHT) {
        if (iter != edittext.end()) {
          iter++;
        }
      }
      if (scancode == ALLEGRO_KEY_LEFT) {
        if (iter != edittext.begin()) {
          iter--;
        }
      }
      if (scancode == ALLEGRO_KEY_ENTER) {
        // Save fronts
        int widthCounter;
        std::string finalFile = "data/" + edittext;
        std::ofstream saveRaw1(finalFile.c_str());

        widthCounter = 0;
        for (uint32_t i = 0; i < tile_map->mapTiles.size(); i++) {
          widthCounter++;
          if (widthCounter == tile_map->width) {
            saveRaw1 << tile_map->mapTiles.at(i).getType() << "\n";
            widthCounter = 0;
          } else {
            saveRaw1 << tile_map->mapTiles.at(i).getType() << " ";
          }
        }
        saveRaw1.close();

        // Save backs
        finalFile =
            "data/" + edittext.substr(0, edittext.size() - 4) + "_back.txt";
        std::ofstream saveRaw2(finalFile.c_str());

        widthCounter = 0;
        for (uint32_t i = 0; i < tile_map->mapTilesBack.size(); i++) {
          widthCounter++;
          if (widthCounter == tile_map->width) {
            saveRaw2 << tile_map->mapTilesBack.at(i).getType() << "\n";
            widthCounter = 0;
          } else {
            saveRaw2 << tile_map->mapTilesBack.at(i).getType() << " ";
          }
        }
        saveRaw2.close();
        saving = false;
      }
    }
  }
}

void Editor::open() {
  tile_map->mapTiles.clear();
  tile_map->mapTilesBack.clear();

  // Name input
  if (KeyListener::anyKeyJustPressed) {
    int newkey = KeyListener::lastKeyPressed;
    char ASCII = newkey & 0xff;
    char scancode = newkey >> 8;

    Logger::log(ASCII);

    // a character key was pressed; add it to the string
    if (ASCII >= 32 && ASCII <= 126 && edittext.length() < 25 &&
        scancode != ALLEGRO_KEY_SPACE) {
      // add the new char
      iter = edittext.insert(iter, ASCII);
      // increment both the caret and the iterator
      iter++;
    }
    // some other, "special" key was pressed; handle it here
    else {
      if (scancode == ALLEGRO_KEY_DELETE) {
        if (iter != edittext.end()) {
          iter = edittext.erase(iter);
        }
      }
      if (scancode == ALLEGRO_KEY_BACKSPACE) {
        if (iter != edittext.begin()) {
          iter--;
          iter = edittext.erase(iter);
        }
      }
      if (scancode == ALLEGRO_KEY_RIGHT) {
        if (iter != edittext.end()) {
          iter++;
        }
      }
      if (scancode == ALLEGRO_KEY_LEFT) {
        if (iter != edittext.begin()) {
          iter--;
        }
      }
      if (scancode == ALLEGRO_KEY_ENTER) {
        std::string finalFile =
            "data/" + edittext.substr(0, edittext.size() - 4);
        tile_map->load(finalFile);
        opening = false;
      }
    }
  }
}

void Editor::update(double delta) {
  tile_map->update(delta);

  // Back to menu
  if (KeyListener::keyPressed[ALLEGRO_KEY_M] && !saving && !opening) {
    setNextState(ProgramState::MENU);
  }

  // Close menu
  if (KeyListener::keyPressed[ALLEGRO_KEY_TILDE]) {
    opening = false;
    saving = false;
  }

  if (saving) {
    save();
  } else if (opening) {
    open();
  } else {
    // Scroll Map
    if (MouseListener::mouse_y < 10 && tile_map->y > 0) {
      tile_map->y -= 16;
    }
    if (MouseListener::mouse_y > (DisplayMode::getDrawHeight() - 10) &&
        tile_map->y < tile_map->height * 64 - DisplayMode::getDrawHeight()) {
      tile_map->y += 16;
    }
    if (MouseListener::mouse_x < 10 && tile_map->x > 0) {
      tile_map->x -= 8;
    }
    if (MouseListener::mouse_x > (DisplayMode::getDrawWidth() - 10) &&
        tile_map->x < tile_map->width * 64 - DisplayMode::getDrawWidth()) {
      tile_map->x += 8;
    }

    // Change selected
    if (KeyListener::keyPressed[ALLEGRO_KEY_UP]) {
      exampleTile->setX(0);
      exampleTile->setY(0);
      exampleTile->changeType(1);
    }
    if (KeyListener::keyPressed[ALLEGRO_KEY_DOWN]) {
      exampleTile->setX(0);
      exampleTile->setY(0);
      exampleTile->changeType(-1);
    }

    // Change Layer
    if (KeyListener::keyPressed[ALLEGRO_KEY_TAB]) {
      if (layer == 0) {
        layer = 1;
      } else {
        layer = 0;
      }
    }

    // Place tile
    if (MouseListener::mouse_button & 1) {
      // Check for collision
      if (layer == 1) {
        for (uint32_t i = 0; i < tile_map->mapTiles.size(); i++) {
          if (collisionAny(MouseListener::mouse_x + tile_map->x,
                           MouseListener::mouse_x + tile_map->x,
                           tile_map->mapTiles.at(i).getX(),
                           tile_map->mapTiles.at(i).getX() + 64,
                           MouseListener::mouse_y + tile_map->y,
                           MouseListener::mouse_y + tile_map->y,
                           tile_map->mapTiles.at(i).getY(),
                           tile_map->mapTiles.at(i).getY() + 64)) {
            tile_map->mapTiles.at(i).setType(selectedTileType);
          }
        }
      } else {
        for (uint32_t i = 0; i < tile_map->mapTilesBack.size(); i++) {
          if (collisionAny(MouseListener::mouse_x + tile_map->x,
                           MouseListener::mouse_x + tile_map->x,
                           tile_map->mapTilesBack.at(i).getX(),
                           tile_map->mapTilesBack.at(i).getX() + 64,
                           MouseListener::mouse_y + tile_map->y,
                           MouseListener::mouse_y + tile_map->y,
                           tile_map->mapTilesBack.at(i).getY(),
                           tile_map->mapTilesBack.at(i).getY() + 64)) {
            tile_map->mapTilesBack.at(i).setType(selectedTileType);
          }
        }
      }
    }

    // Get tile type tile
    if (KeyListener::key[ALLEGRO_KEY_K]) {
      // Check for collision
      if (layer == 1) {
        for (uint32_t i = 0; i < tile_map->mapTiles.size(); i++) {
          if (collisionAny(MouseListener::mouse_x + tile_map->x,
                           MouseListener::mouse_x + tile_map->x,
                           tile_map->mapTiles.at(i).getX(),
                           tile_map->mapTiles.at(i).getX() + 64,
                           MouseListener::mouse_y + tile_map->y,
                           MouseListener::mouse_y + tile_map->y,
                           tile_map->mapTiles.at(i).getY(),
                           tile_map->mapTiles.at(i).getY() + 64)) {
            exampleTile->setX(0);
            exampleTile->setY(0);
            selectedTileType = tile_map->mapTiles.at(i).getType();
            exampleTile->setType(selectedTileType);
          }
        }
      } else {
        for (uint32_t i = 0; i < tile_map->mapTilesBack.size(); i++) {
          if (collisionAny(MouseListener::mouse_x + tile_map->x,
                           MouseListener::mouse_x + tile_map->x,
                           tile_map->mapTilesBack.at(i).getX(),
                           tile_map->mapTilesBack.at(i).getX() + 64,
                           MouseListener::mouse_y + tile_map->y,
                           MouseListener::mouse_y + tile_map->y,
                           tile_map->mapTilesBack.at(i).getY(),
                           tile_map->mapTilesBack.at(i).getY() + 64)) {
            exampleTile->setX(0);
            exampleTile->setY(0);
            selectedTileType = tile_map->mapTilesBack.at(i).getType();
            exampleTile->setType(selectedTileType);
          }
        }
      }
    }

    // Erase tile
    if (MouseListener::mouse_button & 2) {
      // Check for collision
      if (layer == 1) {
        for (uint32_t i = 0; i < tile_map->mapTiles.size(); i++) {
          if (collisionAny(MouseListener::mouse_x + tile_map->x,
                           MouseListener::mouse_x + tile_map->x,
                           tile_map->mapTiles.at(i).getX(),
                           tile_map->mapTiles.at(i).getX() + 64,
                           MouseListener::mouse_y + tile_map->y,
                           MouseListener::mouse_y + tile_map->y,
                           tile_map->mapTiles.at(i).getY(),
                           tile_map->mapTiles.at(i).getY() + 64)) {
            tile_map->mapTiles.at(i).setType(0);
          }
        }
      } else {
        for (uint32_t i = 0; i < tile_map->mapTilesBack.size(); i++) {
          if (collisionAny(MouseListener::mouse_x + tile_map->x,
                           MouseListener::mouse_x + tile_map->x,
                           tile_map->mapTilesBack.at(i).getX(),
                           tile_map->mapTilesBack.at(i).getX() + 64,
                           MouseListener::mouse_y + tile_map->y,
                           MouseListener::mouse_y + tile_map->y,
                           tile_map->mapTilesBack.at(i).getY(),
                           tile_map->mapTilesBack.at(i).getY() + 64)) {
            tile_map->mapTilesBack.at(i).setType(0);
          }
        }
      }
    }

    // Save map
    if (KeyListener::keyPressed[ALLEGRO_KEY_S]) {
      saving = true;
    }

    // Open map
    if (KeyListener::keyPressed[ALLEGRO_KEY_O]) {
      opening = true;
    }

    // Fill map
    if (KeyListener::key[ALLEGRO_KEY_F]) {
      for (uint32_t i = 0; i < tile_map->mapTilesBack.size(); i++) {
        tile_map->mapTilesBack.at(i).setType(selectedTileType);
      }
    }
    if (KeyListener::key[ALLEGRO_KEY_G]) {
      for (uint32_t i = 0; i < tile_map->mapTiles.size(); i++) {
        tile_map->mapTiles.at(i).setType(selectedTileType);
      }
    }
  }
}

void Editor::draw() {
  // Background
  al_draw_filled_rectangle(0, 0, DisplayMode::getDrawWidth(),
                           DisplayMode::getDrawHeight(),
                           al_map_rgb(255, 255, 255));

  if (saving) {
    // Create gui
    al_draw_text(font, al_map_rgb(0, 0, 0), 640, 310, 0, "Save Map Name");

    // Input rectangle
    al_draw_filled_rectangle(400, 408, 892, 452, al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(402, 410, 890, 450, al_map_rgb(255, 255, 255));

    // Output the string to the screen
    al_draw_text(font, al_map_rgb(0, 0, 0), 410, 410, 0, edittext.c_str());

    // Draw the caret
    // vline(text_length(font, edittext.c_str()) + 410 - text_length(font,
    // ".txt"),
    //       412, 448, al_map_rgb(0, 0, 0));
  } else if (opening) {
    // Create gui
    al_draw_text(font, al_map_rgb(0, 0, 0), 640, 310, 0, "Open Map Name");

    // Input rectangle
    al_draw_filled_rectangle(400, 408, 892, 452, al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(402, 410, 890, 450, al_map_rgb(255, 255, 255));

    // Output the string to the screen
    al_draw_text(font, al_map_rgb(0, 0, 0), 410, 410, 0, edittext.c_str());

    // Draw the caret
    // vline(text_length(font, edittext.c_str()) + 410 - text_length(font,
    // ".txt"),
    //       412, 448, al_map_rgb(0, 0, 0));
  } else {
    // Draw tiles
    tile_map->draw_map();
    exampleTile->draw(0, 0, 0);

    // Map info
    al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 80, 0,
                  "height-%i width-%i", tile_map->height, tile_map->width);

    if (layer == 1) {
      al_draw_text(font, al_map_rgb(255, 255, 255), 0, 130, 0,
                   "Editing Mode: Foreground");
    } else if (layer == 0) {
      al_draw_text(font, al_map_rgb(255, 255, 255), 0, 130, 0,
                   "Editing Mode: Background");
    }

    // Cursor
    al_draw_filled_circle(MouseListener::mouse_x, MouseListener::mouse_y, 10,
                          al_map_rgb(0, 0, 0));
    al_draw_filled_circle(MouseListener::mouse_x, MouseListener::mouse_y, 8,
                          al_map_rgb(255, 255, 255));
  }
}

Editor::~Editor() {
  delete tile_map;
  delete exampleTile;
}
