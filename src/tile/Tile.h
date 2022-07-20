#ifndef TILE_H
#define TILE_H

#include <memory>
#include <vector>

#include "../lib/assets/Bitmap.h"
#include "../lib/util/BoundingBox.h"
#include "./TileType.h"

class Tile {
 public:
  // Automatic tile creator
  Tile(std::shared_ptr<TileType> type, int x = 0, int y = 0);

  // Destroy tile
  ~Tile() = default;

  // Get coordinates
  int getX();
  int getY();

  // Get size
  int getWidth();
  int getHeight();

  // Get bounds
  BoundingBox getBounds();

  // Set new coordinates
  void setX(int newX);
  void setY(int newY);

  // Get tile type
  int getType();

  // Returns true if the tile contains the given attribute (e.g. solid, gas)
  bool hasAttribute(const TileAttribute newAttribute) const;

  // Set tile type
  void setType(std::shared_ptr<TileType> type);

  // Get images
  std::vector<Bitmap> getFrames();

  // Draws tile. If no images are assigned it prints out the type number instead
  void draw(int xOffset, int yOffset, int newFrame);

 private:
  // Tile Type
  std::shared_ptr<TileType> type;

  // Position
  BoundingBox bounds{0, 0, 0, 0};

  // Sets dimensions to frame[0]'s dimensions
  void setDimensions();
};

#endif
