#include "Tile.h"

#include "../globals.h"

// Automatic tile creator, when in doubt, use this one
Tile::Tile(int newType, std::vector<Tile>* newTileSet) {
  setTileSet(newTileSet);
  setType(newType);
}

// Manual tile creator, must be used when no tileSet has been created (for
// example before it is loaded) In order to work fully, it must be called
// alongside setImages(...); and addAttribute(...); If this is used, it is not
// possible to change types using setType unless setTileSet(...) is called first
Tile::Tile(int newType,
           bool newParticles,
           bool newLighting,
           std::string newName) {
  type = newType;
  particlesEnabled = newParticles;
  lightingEnabled = newLighting;
  name = newName;
}

// Destroy tile
Tile::~Tile() {}

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
  initialX = x;
}
void Tile::setY(int newY) {
  y = newY;
  initialY = y;
}

// Get tile type
int Tile::getType() {
  return type;
}

// Gets a vector containing any attributes of the tile
// (e.g. may return a vector containing 2 attributes, gas and harmful)
std::vector<int> Tile::getAttribute() {
  return attribute;
}

// Returns true if the tile contains the given attribute (e.g. solid, gas)
bool Tile::containsAttribute(int newAttribute) {
  if (find(attribute.begin(), attribute.end(), newAttribute) !=
      attribute.end()) {
    return true;
  }
  return false;
}

// Add an attribute to the tile (e.g. solid, harmful)
void Tile::addAttribute(int newAttribute) {
  attribute.push_back(newAttribute);
}

// Set tile type and automatically assigns attributes/images/ect...
// Warning! tileSet must have been initialized else this will not work!
void Tile::setType(int newType) {
  // Reset x and y position
  x = initialX;
  y = initialY;

  // Set type to defined type
  type = newType;

  // Tile copying
  Tile* tileToCopy;

  // Remove all attributes
  attribute.clear();

  // Find tile in tileSet

  // Find matching index tile
  for (uint32_t i = 0; i < tileSet->size(); i++) {
    if (tileSet->at(i).getType() == type) {
      tileToCopy = &(tileSet->at(i));

      // Set attributes to ones found in the index
      for (uint32_t t = 0; t < tileToCopy->getAttribute().size(); t++) {
        attribute.push_back(tileToCopy->getAttribute().at(t));
      }

      // Sets images
      setFrames(tileToCopy->getFrames());
    }
  }
}

// Set tile to an index number forward or back
bool Tile::changeType(int changeValue) {
  // Match found
  bool matchFound = false;

  // Index number of tile
  int matchIndex = -1;

  // Find matching index tile
  for (uint32_t i = 0; i < tileSet->size(); i++) {
    if (tileSet->at(i).getType() == type) {
      matchFound = true;
      matchIndex = i;
      break;
    }
  }

  // Match is found at current location
  if (matchFound && (matchIndex + changeValue) > 0 &&
      (matchIndex + changeValue) < tileSet->size()) {
    // Set type to found type
    setType(tileSet->at(matchIndex + changeValue).getType());

    // Worked yay!
    return true;
  } else
    return false;
}

// Set images (and automatically changes animation to 0, 1 or 2)
void Tile::setFrames(std::vector<Bitmap> frames) {
  this->frames = frames;
  setDimensions();
}

// Get frames
std::vector<Bitmap> Tile::getFrames() {
  return frames;
}

// Draws tile. If no images are assigned it prints out the type number instead
void Tile::draw(int xOffset, int yOffset, int frame) {
  if (frames.size() == 0) {
    return;
  }

  frames[frame % frames.size()].draw(x - xOffset, y - yOffset);
}

// Gives the tile an index of all tiles in the tile map, used when assigning
// type
void Tile::setTileSet(std::vector<Tile>* newTileSet) {
  tileSet = newTileSet;
}

// Sets dimensions to images[0]'s dimensions
void Tile::setDimensions() {
  if (frames.size() == 0) {
    return;
  }

  width = frames[0].getWidth();
  height = frames[0].getHeight();

  if (containsAttribute(half_block_top)) {
    height = 32;
  } else if (containsAttribute(half_block_bottom)) {
    height = 32;
    y += height;
  } else if (containsAttribute(quarter_block_top)) {
    height = 16;
  } else if (containsAttribute(quarter_block_bottom)) {
    height = 16;
    y += 48;
  }
}
