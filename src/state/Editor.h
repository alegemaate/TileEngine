#ifndef EDITOR_H
#define EDITOR_H

#include "./engine/State.h"

#include <allegro5/allegro_ttf.h>
#include <string>
#include <vector>

#include "../globals.h"
#include "../tile/Tile.h"
#include "../tile/TileMap.h"
#include "../tools.h"

class Editor : public State {
 public:
  Editor(KeyListener& keyboardListener,
         MouseListener& mouseListener,
         JoystickListener& joystickListener);

  void init() override;
  void update(double delta) override;
  void draw() override;

 private:
  void initMap();
  void saveMap();
  void openMap();

  int selectedTileType{0};
  int layer{1};

  TileMap* tile_map{nullptr};
  Tile* exampleTile{nullptr};

  ALLEGRO_FONT* font{nullptr};

  KeyListener& keyboardListener;
  MouseListener& mouseListener;
  JoystickListener& joystickListener;
};

#endif  // EDITOR_H
