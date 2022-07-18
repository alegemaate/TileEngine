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
  Editor();
  ~Editor();

  void update(double delta) override;
  void draw() override;

 private:
  int selectedTileType;
  int layer;

  bool saving;
  bool opening;

  TileMap* tile_map;
  Tile* exampleTile;

  // Text input
  std::string edittext;
  std::string::iterator iter;

  void init();
  void save();
  void open();

  ALLEGRO_FONT* font;
};

#endif  // EDITOR_H
