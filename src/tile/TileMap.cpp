#include "TileMap.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "../globals.h"
#include "../util/DisplayMode.h"
#include "../util/Logger.h"

std::map<std::string, int> TileMap::TILE_TYPE_LOOKUP = {
    {"gas", 0},
    {"solid", 1},
    {"liquid", 2},
    {"climb", 3},
    {"harmful", 4},
    {"interactive", 5},
    {"item", 6},
    {"spawn", 7},
    {"half_block_top", 8},
    {"half_block_bottom", 9},
    {"quarter_block_top", 10},
    {"quarter_block_bottom", 11},
    {"light", 12},
    {"finish", 13},
};

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
    Tile tile(tileData["id"].get<int>(), tileData["particles"].get<bool>(),
              tileData["lighting"].get<bool>(),
              tileData["name"].get<std::string>());

    // Attributes
    for (auto& tileDataType : tileData["types"]) {
      std::string tileTypeStr = tileDataType.get<std::string>();

      // Check if type is valid
      if (TILE_TYPE_LOOKUP.find(tileTypeStr) == TILE_TYPE_LOOKUP.end()) {
        Logger::fatal(
            "Unknown type (" + tileTypeStr +
            ") in node: type for tile: " + tileData["name"].get<std::string>());
      }

      // Add type
      tile.addAttribute(TILE_TYPE_LOOKUP[tileTypeStr]);
    }

    // Set images
    std::vector<Bitmap> images;
    for (const auto& tileDataImage : tileData["images"]) {
      images.push_back(Bitmap(tileDataImage.get<std::string>()));
    }

    if (images.size() > 0) {
      tile.setFrames(images);
    }

    // Index for other tiles to reference
    tileIndex.push_back(tile);
  }
}

// Manually load new file
void TileMap::load(std::string fileName) {
  // Change size
  std::string fileLoad = fileName + ".txt";
  std::ifstream findSize(fileLoad.c_str());
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

    fileLoad = fileName + ".txt";
    std::ifstream read(fileLoad.c_str());

    for (int t = 0; t < height; t++) {
      for (int i = 0; i < width; i++) {
        int newTileType;
        read >> newTileType;
        // Set tile type
        Tile newTile(newTileType, &tileIndex);
        newTile.setX(i * 64);
        newTile.setY(t * 64);

        // First time, set tile set
        mapTiles.push_back(newTile);
      }
    }
    read.close();

    fileLoad = fileName + "_back.txt";
    std::ifstream read2(fileLoad.c_str());

    for (int t = 0; t < height; t++) {
      for (int i = 0; i < width; i++) {
        int newTileType;
        read2 >> newTileType;
        // Set tile type
        Tile newTile(newTileType, &tileIndex);
        newTile.setX(i * 64);
        newTile.setY(t * 64);

        // First time, set tile set
        mapTilesBack.push_back(newTile);
      }
    }
    read2.close();
  }
}

// Return current animation frame
long TileMap::getFrame() {
  return static_cast<long>(runningTime * (1.0 / FRAME_SECONDS));
}

// Get tileIndex generated from xml. If it has not been loaded it will
// Return NULL!
std::vector<Tile>* TileMap::getIndex() {
  return &tileIndex;
}

// Draw tile map to its x and y
void TileMap::draw_map() {
  auto frame = getFrame();

  for (uint32_t i = 0; i < mapTilesBack.size(); i++) {
    if ((mapTilesBack.at(i).getX() >= x - mapTilesBack.at(i).getWidth()) &&
        (mapTilesBack.at(i).getX() < x + DisplayMode::getDrawWidth()) &&
        (mapTilesBack.at(i).getY() >= y - mapTilesBack.at(i).getHeight()) &&
        (mapTilesBack.at(i).getY() < y + DisplayMode::getDrawHeight())) {
      mapTilesBack.at(i).draw(x, y, frame);
    }
  }
  for (uint32_t i = 0; i < mapTiles.size(); i++) {
    if ((mapTiles.at(i).getX() >= x - mapTiles.at(i).getWidth()) &&
        (mapTiles.at(i).getX() < x + DisplayMode::getDrawWidth()) &&
        (mapTiles.at(i).getY() >= y - mapTiles.at(i).getHeight()) &&
        (mapTiles.at(i).getY() < y + DisplayMode::getDrawHeight())) {
      if (!mapTiles.at(i).containsAttribute(spawn)) {
        mapTiles.at(i).draw(x, y, frame);
      }
    }
  }
}
