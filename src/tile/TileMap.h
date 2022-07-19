#ifndef TILEMAP_H
#define TILEMAP_H

#define FRAME_SECONDS 0.10

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "./Tile.h"
#include "./TileType.h"

// Tile map is the container of all of the tiles
// It loads the set of tiles from the tiles.xml file on creation
class TileMap {
 public:
  // Creates map and loads level from fileName
  TileMap(std::string fileName);
  // Destroy map
  ~TileMap();

  // Update map
  void update(double delta);

  // Find a tile type by id
  std::shared_ptr<TileType> findTileType(uint8_t id);

  // Holds all map tiles in the colliding layer
  std::vector<Tile> mapTiles;

  // Holds all map tiles in background layer
  std::vector<Tile> mapTilesBack;

  // Position of map (for drawing)
  int x;
  int y;

  // Size of map
  int width;
  int height;

  // Manually load new file
  void load(std::string fileName);

  // Save map to file
  void save(std::string fileName);

  // Frame, for animations
  long getFrame();

  // Draw map
  void draw_map();

 private:
  // Load starting tiles
  void load_tiles();

  const std::map<std::string, TileAttribute> TILE_TYPE_LOOKUP{
      {"gas", TileAttribute::GAS},
      {"solid", TileAttribute::SOLID},
      {"liquid", TileAttribute::LIQUID},
      {"climb", TileAttribute::CLIMB},
      {"harmful", TileAttribute::HARMFUL},
      {"interactive", TileAttribute::INTERACTIVE},
      {"item", TileAttribute::ITEM},
      {"spawn", TileAttribute::SPAWN},
      {"half_block_top", TileAttribute::HALF_BLOCK_TOP},
      {"half_block_bottom", TileAttribute::HALF_BLOCK_BOTTOM},
      {"quarter_block_top", TileAttribute::QUARTER_BLOCK_TOP},
      {"quarter_block_bottom", TileAttribute::QUARTER_BLOCK_BOTTOM},
      {"light", TileAttribute::LIGHT},
      {"finish", TileAttribute::FINISH},
  };

  // Filled on creation of tile map, holds 1 of each type of block
  std::vector<std::shared_ptr<TileType>> tileTypes;

  double runningTime{0.0};
};

#endif
