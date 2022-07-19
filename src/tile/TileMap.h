#ifndef TILEMAP_H
#define TILEMAP_H

#define FRAME_SECONDS 0.10

#include <map>
#include <string>
#include <vector>

#include "./Tile.h"

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

  // Holds all map tiles in the colliding layer
  std::vector<Tile> mapTiles;
  // Holds all map tiles in background layer
  std::vector<Tile> mapTilesBack;
  // Filled on creation of tile map, holds 1 of each type of block
  std::vector<Tile> tileIndex;

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

  // Get tileIndex generated from xml. If it has not been loaded it will
  // Return NULL!
  std::vector<Tile>* getIndex();

  // Draw map
  void draw_map();

 private:
  const std::map<std::string, int> TILE_TYPE_LOOKUP{
      {"gas", 0},
      {"solid", 1},
      {"liquid", 2},
      {"climb", 3},
      {"harmful", 4},
      {"interactive", 5},
      {"item", 6},
      {"spawn", 7},
      {"half_block_top", 8},
      {"half_block_bottom", 9},
      {"quarter_block_top", 10},
      {"quarter_block_bottom", 11},
      {"light", 12},
      {"finish", 13},
  };

  // Load starting tiles
  void load_tiles();

  double runningTime{0.0};
};

#endif
