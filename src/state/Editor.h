#ifndef EDITOR_H
#define EDITOR_H

#include "../lib/state/State.h"

#include <string>
#include <vector>

#include "../lib/assets/Font.h"
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

  Font font{};

  KeyListener& keyboardListener;
  MouseListener& mouseListener;
  JoystickListener& joystickListener;
};

#endif  // EDITOR_H
