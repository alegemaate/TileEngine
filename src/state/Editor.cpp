#include "Editor.h"

#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <fstream>

#include "../util/DisplayMode.h"
#include "../util/KeyListener.h"
#include "../util/Logger.h"
#include "../util/MouseListener.h"

Editor::Editor(KeyListener& keyboardListener,
               MouseListener& mouseListener,
               JoystickListener& joystickListener)
    : keyboardListener(keyboardListener),
      mouseListener(mouseListener),
      joystickListener(joystickListener) {}

void Editor::init() {
  // Create map
  tile_map = new TileMap("data/templates/blank64x48.txt");

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
}

void Editor::saveMap() {
  auto* chooser = al_create_native_file_dialog(
      "data/", "Save Level", "*.txt;*.*", ALLEGRO_FILECHOOSER_SAVE);

  if (al_show_native_file_dialog(nullptr, chooser)) {
    const char* fileName = al_get_native_file_dialog_path(chooser, 0);
    Logger::log(fileName);

    // You also need to check for cancel Button here too
    if (fileName != nullptr) {
      tile_map->save(fileName);
    }
  }
}

void Editor::openMap() {
  auto* chooser =
      al_create_native_file_dialog("data/", "Select a Level", "*.txt;*.*", 0);

  if (al_show_native_file_dialog(nullptr, chooser)) {
    const char* fileName = al_get_native_file_dialog_path(chooser, 0);
    Logger::log(fileName);

    // You also need to check for cancel Button here too
    if (fileName != nullptr) {
      tile_map->load(fileName);
    }
  }
}

void Editor::update(double delta) {
  tile_map->update(delta);

  Logger::log(selectedTileType);

  // Back to menu
  if (keyboardListener.wasPressed(Key::M)) {
    setNextState(ProgramState::MENU);
  }

  // Scroll Map
  if (mouseListener.getY() < 10 && tile_map->y > 0) {
    tile_map->y -= 16;
  }
  if (mouseListener.getY() > (DisplayMode::getDrawHeight() - 10) &&
      tile_map->y < tile_map->height * 64 - DisplayMode::getDrawHeight()) {
    tile_map->y += 16;
  }
  if (mouseListener.getX() < 10 && tile_map->x > 0) {
    tile_map->x -= 8;
  }
  if (mouseListener.getX() > (DisplayMode::getDrawWidth() - 10) &&
      tile_map->x < tile_map->width * 64 - DisplayMode::getDrawWidth()) {
    tile_map->x += 8;
  }

  // Change selected
  if (keyboardListener.wasPressed(Key::UP)) {
    exampleTile->changeType(1);
    selectedTileType = exampleTile->getType();
  }
  if (keyboardListener.wasPressed(Key::DOWN)) {
    exampleTile->changeType(-1);
    selectedTileType = exampleTile->getType();
  }

  // Change Layer
  if (keyboardListener.wasPressed(Key::TAB)) {
    layer = layer == 0 ? 1 : 0;
  }

  // Place tile
  if (mouseListener.isDown(MouseButton::Left)) {
    // Check for collision
    if (layer == 1) {
      for (uint32_t i = 0; i < tile_map->mapTiles.size(); i++) {
        if (collisionAny(mouseListener.getX() + tile_map->x,
                         mouseListener.getX() + tile_map->x,
                         tile_map->mapTiles.at(i).getX(),
                         tile_map->mapTiles.at(i).getX() + 64,
                         mouseListener.getY() + tile_map->y,
                         mouseListener.getY() + tile_map->y,
                         tile_map->mapTiles.at(i).getY(),
                         tile_map->mapTiles.at(i).getY() + 64)) {
          tile_map->mapTiles.at(i).setType(selectedTileType);
        }
      }
    } else {
      for (uint32_t i = 0; i < tile_map->mapTilesBack.size(); i++) {
        if (collisionAny(mouseListener.getX() + tile_map->x,
                         mouseListener.getX() + tile_map->x,
                         tile_map->mapTilesBack.at(i).getX(),
                         tile_map->mapTilesBack.at(i).getX() + 64,
                         mouseListener.getY() + tile_map->y,
                         mouseListener.getY() + tile_map->y,
                         tile_map->mapTilesBack.at(i).getY(),
                         tile_map->mapTilesBack.at(i).getY() + 64)) {
          tile_map->mapTilesBack.at(i).setType(selectedTileType);
        }
      }
    }
  }

  // Get tile type tile
  if (keyboardListener.isDown(Key::K)) {
    // Check for collision
    if (layer == 1) {
      for (uint32_t i = 0; i < tile_map->mapTiles.size(); i++) {
        if (collisionAny(mouseListener.getX() + tile_map->x,
                         mouseListener.getX() + tile_map->x,
                         tile_map->mapTiles.at(i).getX(),
                         tile_map->mapTiles.at(i).getX() + 64,
                         mouseListener.getY() + tile_map->y,
                         mouseListener.getY() + tile_map->y,
                         tile_map->mapTiles.at(i).getY(),
                         tile_map->mapTiles.at(i).getY() + 64)) {
          exampleTile->setX(0);
          exampleTile->setY(0);
          exampleTile->setType(selectedTileType);
        }
      }
    } else {
      for (uint32_t i = 0; i < tile_map->mapTilesBack.size(); i++) {
        if (collisionAny(mouseListener.getX() + tile_map->x,
                         mouseListener.getX() + tile_map->x,
                         tile_map->mapTilesBack.at(i).getX(),
                         tile_map->mapTilesBack.at(i).getX() + 64,
                         mouseListener.getY() + tile_map->y,
                         mouseListener.getY() + tile_map->y,
                         tile_map->mapTilesBack.at(i).getY(),
                         tile_map->mapTilesBack.at(i).getY() + 64)) {
          exampleTile->setX(0);
          exampleTile->setY(0);
          exampleTile->setType(selectedTileType);
        }
      }
    }
  }

  // Erase tile
  if (mouseListener.isDown(MouseButton::Right)) {
    // Check for collision
    if (layer == 1) {
      for (uint32_t i = 0; i < tile_map->mapTiles.size(); i++) {
        if (collisionAny(mouseListener.getX() + tile_map->x,
                         mouseListener.getX() + tile_map->x,
                         tile_map->mapTiles.at(i).getX(),
                         tile_map->mapTiles.at(i).getX() + 64,
                         mouseListener.getY() + tile_map->y,
                         mouseListener.getY() + tile_map->y,
                         tile_map->mapTiles.at(i).getY(),
                         tile_map->mapTiles.at(i).getY() + 64)) {
          tile_map->mapTiles.at(i).setType(0);
        }
      }
    } else {
      for (uint32_t i = 0; i < tile_map->mapTilesBack.size(); i++) {
        if (collisionAny(mouseListener.getX() + tile_map->x,
                         mouseListener.getX() + tile_map->x,
                         tile_map->mapTilesBack.at(i).getX(),
                         tile_map->mapTilesBack.at(i).getX() + 64,
                         mouseListener.getY() + tile_map->y,
                         mouseListener.getY() + tile_map->y,
                         tile_map->mapTilesBack.at(i).getY(),
                         tile_map->mapTilesBack.at(i).getY() + 64)) {
          tile_map->mapTilesBack.at(i).setType(0);
        }
      }
    }
  }

  // Save map
  if (keyboardListener.wasPressed(Key::S)) {
    saveMap();
  }

  // Open map
  if (keyboardListener.wasPressed(Key::O)) {
    openMap();
  }

  // Fill map
  if (keyboardListener.isDown(Key::F)) {
    for (uint32_t i = 0; i < tile_map->mapTilesBack.size(); i++) {
      tile_map->mapTilesBack.at(i).setType(selectedTileType);
    }
  }
  if (keyboardListener.isDown(Key::G)) {
    for (uint32_t i = 0; i < tile_map->mapTiles.size(); i++) {
      tile_map->mapTiles.at(i).setType(selectedTileType);
    }
  }
}

void Editor::draw() {
  // Background
  al_draw_filled_rectangle(0, 0, DisplayMode::getDrawWidth(),
                           DisplayMode::getDrawHeight(),
                           al_map_rgb(255, 255, 255));

  // Draw tiles
  tile_map->draw_map();
  exampleTile->draw(0, 0, 0);

  // Map info
  al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 80, 0,
                "height: %i width: %i", tile_map->height, tile_map->width);

  if (layer == 1) {
    al_draw_text(font, al_map_rgb(255, 255, 255), 0, 130, 0,
                 "Layer: Foreground");
  } else if (layer == 0) {
    al_draw_text(font, al_map_rgb(255, 255, 255), 0, 130, 0,
                 "Layer: Background");
  }

  // Cursor
  al_draw_filled_circle(mouseListener.getX(), mouseListener.getY(), 10,
                        al_map_rgb(0, 0, 0));
  al_draw_filled_circle(mouseListener.getX(), mouseListener.getY(), 8,
                        al_map_rgb(255, 255, 255));
}
