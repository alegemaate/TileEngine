#ifndef TILE_H
#define TILE_H

#include <allegro5/allegro.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include "../globals.h"
#include "../tools.h"

class Tile {
 public:
  // Automatic tile creator, when in doubt, use this one
  Tile(int newType, vector<Tile>* newTileSet);

  // Manual tile creator, must be used when no tileSet has been created (for
  // example before it is loaded) In order to work fully, it must be called
  // alongside setImages(...); and addAttribute(...); If this is used, it is not
  // possible to change types using setType unless setTileSet(...) is called
  // first
  Tile(int newType, bool newParticles, bool newLighting, string newName);

  // Destroy tile
  ~Tile();

  // Get coordinates
  int getX();
  int getY();

  // Get size
  int getWidth();
  int getHeight();

  // Set new coordinates
  void setX(int newX);
  void setY(int newY);

  // Get tile type
  int getType();

  // Get animation type (0 = 1 frame, 1 = 4 frame, 8 = 8 frame)
  int getAnimated();

  // Gets a vector containing any attributes of the tile
  // (e.g. may return a vector containing 2 attributes, gas and harmful)
  vector<int> getAttribute();

  // Returns true if the tile contains the given attribute (e.g. solid, gas)
  bool containsAttribute(int newAttribute);

  // Add an attribute to the tile (e.g. solid, harmful)
  void addAttribute(int newAttribute);

  // Returns images[0], even if it is NOT loaded in which case it returns NULL
  ALLEGRO_BITMAP* getImage();

  // Set tile type and automatically assigns attributes/images/ect...
  // Warning! tileSet must have been initialized else this will not work!
  void setType(int newType);

  // Set tile to an index number forward or back
  bool changeType(int changeValue);

  // Set images (and automatically changes animation to 0, 1 or 2)
  void setImages(ALLEGRO_BITMAP* image1);
  void setImages(ALLEGRO_BITMAP* image1,
                 ALLEGRO_BITMAP* image2,
                 ALLEGRO_BITMAP* image3,
                 ALLEGRO_BITMAP* image4);
  void setImages(ALLEGRO_BITMAP* image1,
                 ALLEGRO_BITMAP* image2,
                 ALLEGRO_BITMAP* image3,
                 ALLEGRO_BITMAP* image4,
                 ALLEGRO_BITMAP* image5,
                 ALLEGRO_BITMAP* image6,
                 ALLEGRO_BITMAP* image7,
                 ALLEGRO_BITMAP* image8);

  // Draws tile. If no images are assigned it prints out the type number instead
  void draw_tile(int xOffset, int yOffset, int newFrame);

  // Gives the tile an index of all tiles in the tile map, used when assigning
  // type
  void setTileSet(vector<Tile>* newTileSet);

 private:
  // Position
  int x, y;
  // Starting position
  int initialX, initialY;

  // Size
  int width;
  int height;

  // Type
  int type;

  // Particles enabled (fire/water effects)
  bool particlesEnabled;

  // Lighting enabled (if it emits light)
  bool lightingEnabled;

  // Name of tile
  string name;

  // Attributes
  vector<int> attribute;

  // Pointer to all tile types used as reference
  vector<Tile>* tileSet;

  // Images
  ALLEGRO_BITMAP* images[8];

  // Type of animation 0 = 1 frame, 1 = 4 frame, 2 = 8 frame
  int animated;

  // Sets dimensions to images[0]'s dimensions
  void setDimensions();
};

#endif
