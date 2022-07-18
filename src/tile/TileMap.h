#ifndef TILEMAP_H
#define TILEMAP_H

#define FRAME_MILASECONDS 100

#include <allegro5/allegro.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../globals.h"
#include "../tools.h"

#include "./Tile.h"

// Tile map is the container of all of the tiles
// It loads the set of tiles from the tiles.xml file on creation
class TileMap {
 public:
  // Creates map and loads level from fileName
  TileMap(string fileName);
  // Destroy map
  ~TileMap();

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
  void load(string fileName);

  // Frame, for animations
  long getFrame();

  // Get tileIndex generated from xml. If it has not been loaded it will
  // Return NULL!
  std::vector<Tile>* getIndex();

  // Draw map
  void draw_map();
  void draw_map(int newX, int newY);

 private:
  // Load starting tiles
  void load_tiles();

  // Animation variables
  static volatile long frame;
  static void change_frame();

  static std::map<std::string, int> TILE_TYPE_LOOKUP;
};

#endif
