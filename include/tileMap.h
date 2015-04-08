#ifndef TILEMAP_H
#define TILEMAP_H

#define FRAME_MILASECONDS 100

#include <allegro.h>
#include <alpng.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

#include "globals.h"
#include "tools.h"

#include "tile.h"

using namespace std;

// Tile map is the container of all of the tiles
// It loads the set of tiles from the tiles.xml file on creation
class tileMap{
  public:
    // Creates map and loads level from fileName
    tileMap(string fileName);
    // Destroy map
    ~tileMap();

    // Holds all map tiles in the colliding layer
    vector<tile> mapTiles;
    // Holds all map tiles in background layer
    vector<tile> mapTilesBack;
    // Filled on creation of tile map, holds 1 of each type of block
    vector<tile> tileIndex;

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
    vector<tile>* getIndex();

    // Draw map
    void draw_map(BITMAP* tempSprite);
    void draw_map(BITMAP* tempSprite, int newX, int newY);
  private:
    // Load starting tiles
    void load_tiles();

    // Animation variables
    static volatile long frame;
    static void change_frame();
};

#endif
