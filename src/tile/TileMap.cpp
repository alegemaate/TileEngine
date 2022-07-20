#include "TileMap.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "../lib/display/DisplayMode.h"
#include "../lib/util/Logger.h"

// Creates map and loads level from fileName
TileMap::TileMap(std::string fileName) {
  x = 0;
  y = 0;

  width = 0;
  height = 0;

  // LOCK_VARIABLE(frame);
  // LOCK_FUNCTION(change_frame);
  // install_int_ex(change_frame, MSEC_TO_TIMER(FRAME_SECONDS));

  load_tiles();
  load(fileName);
}

// Destroy map
TileMap::~TileMap() {
  mapTiles.clear();
  mapTilesBack.clear();
}

// Update map
void TileMap::update(double delta) {
  runningTime += delta;
}

std::shared_ptr<TileType> TileMap::findTileType(uint8_t id) {
  for (auto& tileType : tileTypes) {
    if (tileType->id == id) {
      return tileType;
    }
  }

  // TODO: Fix unsafe return
  return tileTypes.at(0);
}

// Manually load new file
void TileMap::load_tiles() {
  // Read tiles
  std::ifstream file("data/tiles.json");

  // Check exist
  if (!file.is_open()) {
    Logger::fatal(
        "Cannot find file /data/tiles.xml \n Please check your files and try "
        "again");
  }

  nlohmann::json tiles;
  file >> tiles;

  // Load blocks
  for (auto& tileData : tiles) {
    auto tile = std::make_shared<TileType>();
    tile->id = tileData["id"].get<int>();
    tile->name = tileData["name"].get<std::string>();
    tile->lighting = tileData["lighting"].get<bool>();
    tile->particles = tileData["particles"].get<bool>();

    // Attributes
    for (const auto& tileDataType : tileData["types"]) {
      std::string tileTypeStr = tileDataType.get<std::string>();

      // Check if type is valid
      if (TILE_TYPE_LOOKUP.find(tileTypeStr) == TILE_TYPE_LOOKUP.end()) {
        Logger::fatal(
            "Unknown type (" + tileTypeStr +
            ") in node: type for tile: " + tileData["name"].get<std::string>());
      }

      // Add type
      tile->attributes.push_back(TILE_TYPE_LOOKUP.at(tileTypeStr));
    }

    // Set images
    for (const auto& tileDataImage : tileData["images"]) {
      tile->frames.emplace_back(tileDataImage.get<std::string>());
    }

    // Index for other tiles to reference
    tileTypes.push_back(tile);
  }
}

// Manually load new file
void TileMap::load(std::string fileName) {
  // Change size
  std::string fileLoad = fileName;
  std::ifstream findSize(fileLoad.c_str());

  if (findSize.fail()) {
    Logger::warn("Could not open file " + fileName);
    return;
  }

  width = 0;
  height = 0;
  int data;
  while (findSize >> data) {
    if (height == 0) {
      width++;
    }
    if (findSize.peek() == '\n') {
      height++;
    }
  }

  // Setup Map
  if (fileName != "blank") {
    mapTiles.clear();
    mapTilesBack.clear();

    fileLoad = fileName;
    std::ifstream read(fileLoad.c_str());

    for (int t = 0; t < height; t++) {
      for (int i = 0; i < width; i++) {
        int intType;
        read >> intType;

        // Set tile type
        auto newTile =
            std::make_shared<Tile>(findTileType(intType), i * 64, t * 64);

        // First time, set tile set
        mapTiles.push_back(newTile);
      }
    }
    read.close();

    fileLoad = fileName.substr(0, fileName.size() - 4) + "_back.txt";
    std::ifstream read2(fileLoad.c_str());

    for (int t = 0; t < height; t++) {
      for (int i = 0; i < width; i++) {
        // Set tile type
        int intType;
        read2 >> intType;

        auto newTile =
            std::make_shared<Tile>(findTileType(intType), i * 64, t * 64);

        // First time, set tile set
        mapTilesBack.push_back(newTile);
      }
    }
    read2.close();
  }
}

// Manually load new file
void TileMap::save(std::string fileName) {
  // Save fronts
  int widthCounter;
  std::string finalFile = fileName;
  std::ofstream saveRaw1(finalFile.c_str());

  widthCounter = 0;
  for (auto& tile : mapTiles) {
    widthCounter++;
    if (widthCounter == width) {
      saveRaw1 << tile->getType() << "\n";
      widthCounter = 0;
    } else {
      saveRaw1 << tile->getType() << " ";
    }
  }
  saveRaw1.close();

  // Save backs
  finalFile = fileName.substr(0, fileName.size() - 4) + "_back.txt";
  std::ofstream saveRaw2(finalFile.c_str());

  widthCounter = 0;
  for (auto& tile : mapTilesBack) {
    widthCounter++;
    if (widthCounter == width) {
      saveRaw2 << tile->getType() << "\n";
      widthCounter = 0;
    } else {
      saveRaw2 << tile->getType() << " ";
    }
  }
  saveRaw2.close();
}

// Return current animation frame
long TileMap::getFrame() {
  return static_cast<long>(runningTime * (1.0 / FRAME_SECONDS));
}

// Draw tile map to its x and y
void TileMap::draw_map() {
  auto frame = getFrame();

  for (const auto& tile : mapTilesBack) {
    if ((tile->getX() >= x - tile->getWidth()) &&
        (tile->getX() < x + DisplayMode::getDrawWidth()) &&
        (tile->getY() >= y - tile->getHeight()) &&
        (tile->getY() < y + DisplayMode::getDrawHeight())) {
      tile->draw(x, y, frame);
    }
  }
  for (const auto& tile : mapTiles) {
    if ((tile->getX() >= x - tile->getWidth()) &&
        (tile->getX() < x + DisplayMode::getDrawWidth()) &&
        (tile->getY() >= y - tile->getHeight()) &&
        (tile->getY() < y + DisplayMode::getDrawHeight())) {
      tile->draw(x, y, frame);
    }
  }
}

// Check if is tile at
bool TileMap::isTileAt(int x, int y) {
  for (const auto& tile : mapTiles) {
    if (tile->getBounds().collidesPoint(x, y)) {
      return true;
    }
  }
  return false;
}

// Check if is tile at bb
bool TileMap::isTileAt(BoundingBox area) {
  for (const auto& tile : mapTiles) {
    if (tile->getBounds().collides(area)) {
      return true;
    }
  }
  return false;
}

// Get tile at position
std::shared_ptr<Tile> TileMap::getTileAt(int x, int y) {
  for (auto tile : mapTiles) {
    if (tile->getBounds().collidesPoint(x, y)) {
      return tile;
    }
  }

  throw new std::runtime_error("No tile found at " + std::to_string(x) + "," +
                               std::to_string(y));
}

// Get tile at position
std::vector<std::shared_ptr<Tile>> TileMap::getTilesAt(BoundingBox area) {
  std::vector<std::shared_ptr<Tile>> tiles;

  for (auto tile : mapTiles) {
    if (tile->getBounds().collides(area)) {
      tiles.push_back(tile);
    }
  }

  return tiles;
}
