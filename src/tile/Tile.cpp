#include "Tile.h"

#include <algorithm>

// Automatic tile creator, when in doubt, use this one
Tile::Tile(std::shared_ptr<TileType> type, int x, int y)
    : type(type), x(x), y(y) {
  setDimensions();
}

// Get coordinates
int Tile::getX() {
  return x;
}

int Tile::getY() {
  return y;
}

// Get size
int Tile::getWidth() {
  return width;
}

int Tile::getHeight() {
  return height;
}

// Set new coordinates
void Tile::setX(int newX) {
  x = newX;
}

void Tile::setY(int newY) {
  y = newY;
}

// Get tile type
int Tile::getType() {
  return type->id;
}

// Returns true if the tile contains the given attribute (e.g. solid, gas)
bool Tile::hasAttribute(const TileAttribute newAttribute) const {
  if (std::find(type->attributes.begin(), type->attributes.end(),
                newAttribute) != type->attributes.end()) {
    return true;
  }
  return false;
}

// Set tile type
void Tile::setType(std::shared_ptr<TileType> type) {
  // Reset x and y position
  this->type = type;
}

// Get frames
std::vector<Bitmap> Tile::getFrames() {
  return type->frames;
}

// Draws tile. If no images are assigned it prints out the type number instead
void Tile::draw(int xOffset, int yOffset, int frame) {
  if (type->frames.size() == 0) {
    return;
  }

  type->frames[frame % type->frames.size()].draw(x - xOffset, y - yOffset);
}

// Sets dimensions to images[0]'s dimensions
void Tile::setDimensions() {
  if (type->frames.size() == 0) {
    return;
  }

  width = type->frames[0].getWidth();
  height = type->frames[0].getHeight();

  if (hasAttribute(TileAttribute::HALF_BLOCK_TOP)) {
    height = 32;
  } else if (hasAttribute(TileAttribute::HALF_BLOCK_BOTTOM)) {
    height = 32;
    y += height;
  } else if (hasAttribute(TileAttribute::QUARTER_BLOCK_TOP)) {
    height = 16;
  } else if (hasAttribute(TileAttribute::QUARTER_BLOCK_BOTTOM)) {
    height = 16;
    y += 48;
  }
}
