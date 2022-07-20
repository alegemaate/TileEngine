#include "Tile.h"

#include <algorithm>

// Automatic tile creator, when in doubt, use this one
Tile::Tile(std::shared_ptr<TileType> type, int x, int y) : type(type) {
  bounds.x = x;
  bounds.y = y;
  setDimensions();
}

// Get coordinates
int Tile::getX() {
  return bounds.x;
}

int Tile::getY() {
  return bounds.y;
}

// Get size
int Tile::getWidth() {
  return bounds.width;
}

int Tile::getHeight() {
  return bounds.height;
}

BoundingBox Tile::getBounds() {
  return bounds;
}

// Set new coordinates
void Tile::setX(int newX) {
  bounds.x = newX;
}

void Tile::setY(int newY) {
  bounds.y = newY;
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

  type->frames[frame % type->frames.size()].draw(bounds.x - xOffset,
                                                 bounds.y - yOffset);
}

// Sets dimensions to images[0]'s dimensions
void Tile::setDimensions() {
  if (type->frames.size() == 0) {
    return;
  }

  bounds.width = type->frames[0].getWidth();
  bounds.height = type->frames[0].getHeight();

  if (hasAttribute(TileAttribute::HALF_BLOCK_TOP)) {
    bounds.height = 32;
  } else if (hasAttribute(TileAttribute::HALF_BLOCK_BOTTOM)) {
    bounds.height = 32;
    bounds.y += bounds.height;
  } else if (hasAttribute(TileAttribute::QUARTER_BLOCK_TOP)) {
    bounds.height = 16;
  } else if (hasAttribute(TileAttribute::QUARTER_BLOCK_BOTTOM)) {
    bounds.height = 16;
    bounds.y += 48;
  }
}
