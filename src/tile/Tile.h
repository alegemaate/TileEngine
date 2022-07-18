#ifndef TILE_H
#define TILE_H

#include <allegro5/allegro.h>
#include <vector>

#include "../util/Bitmap.h"

class Tile {
 public:
  // Automatic tile creator, when in doubt, use this one
  Tile(int newType, std::vector<Tile>* newTileSet);

  // Manual tile creator, must be used when no tileSet has been created (for
  // example before it is loaded) In order to work fully, it must be called
  // alongside setImages(...); and addAttribute(...); If this is used, it is not
  // possible to change types using setType unless setTileSet(...) is called
  // first
  Tile(int newType, bool newParticles, bool newLighting, std::string newName);

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

  // Gets a vector containing any attributes of the tile
  // (e.g. may return a vector containing 2 attributes, gas and harmful)
  std::vector<int> getAttribute();

  // Returns true if the tile contains the given attribute (e.g. solid, gas)
  bool containsAttribute(int newAttribute);

  // Add an attribute to the tile (e.g. solid, harmful)
  void addAttribute(int newAttribute);

  // Set tile type and automatically assigns attributes/images/ect...
  // Warning! tileSet must have been initialized else this will not work!
  void setType(int newType);

  // Set tile to an index number forward or back
  bool changeType(int changeValue);

  // Set images
  void setFrames(std::vector<Bitmap> frames);

  // Get images
  std::vector<Bitmap> getFrames();

  // Draws tile. If no images are assigned it prints out the type number instead
  void draw(int xOffset, int yOffset, int newFrame);

  // Gives the tile an index of all tiles in the tile map, used when assigning
  // type
  void setTileSet(std::vector<Tile>* newTileSet);

 private:
  // Position
  int x{0};
  int y{0};
  // Starting position
  int initialX{0};
  int initialY{0};

  // Size
  int width{0};
  int height{0};

  // Type
  int type{0};

  // Particles enabled (fire/water effects)
  bool particlesEnabled{false};

  // Lighting enabled (if it emits light)
  bool lightingEnabled{false};

  // Name of tile
  std::string name;

  // Attributes
  std::vector<int> attribute;

  // Pointer to all tile types used as reference
  std::vector<Tile>* tileSet;

  // Images
  std::vector<Bitmap> frames;

  // Sets dimensions to images[0]'s dimensions
  void setDimensions();
};

#endif
