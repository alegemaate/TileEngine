#include "Editor.h"

#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <fstream>

#include "../lib/display/DisplayMode.h"
#include "../lib/input/KeyListener.h"
#include "../lib/input/MouseListener.h"
#include "../lib/util/Logger.h"

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
  auto type = tile_map->findTileType(selectedTileType);
  exampleTile = new Tile(type);
  exampleTile->setX(0);
  exampleTile->setY(0);

  // Load font
  font = Font("fonts/opensans.ttf", 24);
}

void Editor::saveMap() {
  auto* chooser = al_create_native_file_dialog(
      "data/", "Save Level", "*.txt;*.*", ALLEGRO_FILECHOOSER_SAVE);

  if (al_show_native_file_dialog(nullptr, chooser)) {
    const char* fileName = al_get_native_file_dialog_path(chooser, 0);

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

    // You also need to check for cancel Button here too
    if (fileName != nullptr) {
      tile_map->load(fileName);
    }
  }
}

void Editor::update(double delta) {
  tile_map->update(delta);

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
    // exampleTile->changeType(1);
    selectedTileType = exampleTile->getType();
  }
  if (keyboardListener.wasPressed(Key::DOWN)) {
    // exampleTile->changeType(-1);
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
      for (auto& tile : tile_map->mapTiles) {
        if (collisionAny(mouseListener.getX() + tile_map->x,
                         mouseListener.getX() + tile_map->x, tile->getX(),
                         tile->getX() + 64, mouseListener.getY() + tile_map->y,
                         mouseListener.getY() + tile_map->y, tile->getY(),
                         tile->getY() + 64)) {
          auto type = tile_map->findTileType(selectedTileType);
          tile->setType(type);
        }
      }
    } else {
      for (auto& tile : tile_map->mapTilesBack) {
        if (collisionAny(mouseListener.getX() + tile_map->x,
                         mouseListener.getX() + tile_map->x, tile->getX(),
                         tile->getX() + 64, mouseListener.getY() + tile_map->y,
                         mouseListener.getY() + tile_map->y, tile->getY(),
                         tile->getY() + 64)) {
          auto type = tile_map->findTileType(selectedTileType);
          tile->setType(type);
        }
      }
    }
  }

  // Get tile type tile
  if (keyboardListener.isDown(Key::K)) {
    // Check for collision
    if (layer == 1) {
      for (auto& tile : tile_map->mapTiles) {
        if (collisionAny(mouseListener.getX() + tile_map->x,
                         mouseListener.getX() + tile_map->x, tile->getX(),
                         tile->getX() + 64, mouseListener.getY() + tile_map->y,
                         mouseListener.getY() + tile_map->y, tile->getY(),
                         tile->getY() + 64)) {
          auto type = tile_map->findTileType(selectedTileType);
          exampleTile->setType(type);
          exampleTile->setX(0);
          exampleTile->setY(0);
        }
      }
    } else {
      for (auto& tile : tile_map->mapTilesBack) {
        if (collisionAny(mouseListener.getX() + tile_map->x,
                         mouseListener.getX() + tile_map->x, tile->getX(),
                         tile->getX() + 64, mouseListener.getY() + tile_map->y,
                         mouseListener.getY() + tile_map->y, tile->getY(),
                         tile->getY() + 64)) {
          auto type = tile_map->findTileType(selectedTileType);
          exampleTile->setType(type);
          exampleTile->setX(0);
          exampleTile->setY(0);
        }
      }
    }
  }

  // Erase tile
  if (mouseListener.isDown(MouseButton::Right)) {
    // Check for collision
    if (layer == 1) {
      for (auto& tile : tile_map->mapTiles) {
        if (collisionAny(mouseListener.getX() + tile_map->x,
                         mouseListener.getX() + tile_map->x, tile->getX(),
                         tile->getX() + 64, mouseListener.getY() + tile_map->y,
                         mouseListener.getY() + tile_map->y, tile->getY(),
                         tile->getY() + 64)) {
          auto type = tile_map->findTileType(0);
          tile->setType(type);
        }
      }
    } else {
      for (auto& tile : tile_map->mapTilesBack) {
        if (collisionAny(mouseListener.getX() + tile_map->x,
                         mouseListener.getX() + tile_map->x, tile->getX(),
                         tile->getX() + 64, mouseListener.getY() + tile_map->y,
                         mouseListener.getY() + tile_map->y, tile->getY(),
                         tile->getY() + 64)) {
          auto type = tile_map->findTileType(0);
          tile->setType(type);
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
    auto type = tile_map->findTileType(selectedTileType);
    for (auto& tile : tile_map->mapTilesBack) {
      tile->setType(type);
    }
  }
  if (keyboardListener.isDown(Key::G)) {
    auto type = tile_map->findTileType(selectedTileType);
    for (auto& tile : tile_map->mapTiles) {
      tile->setType(type);
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
  font.draw(10, 10, al_map_rgb(255, 255, 255),
            "height:" + std::to_string(tile_map->height) +
                " width:" + std::to_string(tile_map->width));

  if (layer == 1) {
    font.draw(10, 40, al_map_rgb(255, 255, 255), "Layer: Foreground");
  } else if (layer == 0) {
    font.draw(10, 40, al_map_rgb(255, 255, 255), "Layer: Background");
  }

  // Cursor
  al_draw_filled_circle(mouseListener.getX(), mouseListener.getY(), 10,
                        al_map_rgb(0, 0, 0));
  al_draw_filled_circle(mouseListener.getX(), mouseListener.getY(), 8,
                        al_map_rgb(255, 255, 255));
}
