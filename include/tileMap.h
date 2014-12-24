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

using namespace std;

#include "globals.h"
#include "tools.h"

#include "tile.h"

class tileMap{
  public:
    tileMap(string fileName);

    vector<tile> mapTiles;
    vector<tile> mapTilesBack;

    int x;
    int y;

    int width;
    int height;

    void load_images();
    void draw_map(BITMAP* tempSprite);

    void load(string fileName);

    ~tileMap();
  private:
    static volatile long frame;
    static void change_frame();
};

#endif
