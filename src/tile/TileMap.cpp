#include "TileMap.h"

#include <nlohmann/json.hpp>

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

volatile long TileMap::frame = 0;

// Creates map and loads level from fileName
TileMap::TileMap(string fileName) {
  x = 0;
  y = 0;

  width = 0;
  height = 0;

  // LOCK_VARIABLE(frame);
  // LOCK_FUNCTION(change_frame);
  // install_int_ex(change_frame, MSEC_TO_TIMER(FRAME_MILASECONDS));

  load_tiles();
  load(fileName);
}

// Destroy map
TileMap::~TileMap() {
  mapTiles.clear();
  mapTilesBack.clear();
}

// Manually load new file
void TileMap::load_tiles() {
  // Read tiles
  ifstream file("data/tiles.json");

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
void TileMap::load(string fileName) {
  // Change size
  string fileLoad = fileName + ".txt";
  ifstream findSize(fileLoad.c_str());
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
    ifstream read(fileLoad.c_str());

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
    ifstream read2(fileLoad.c_str());

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

// // Change animation frame
// void tileMap::change_frame() {
//   if (frame < 7) {
//     frame++;
//   } else {
//     frame = 0;
//   }
// }
// END_OF_FUNCTION(change_frame)

// Return current animation frame
long TileMap::getFrame() {
  return frame;
}

// Get tileIndex generated from xml. If it has not been loaded it will
// Return NULL!
vector<Tile>* TileMap::getIndex() {
  return &tileIndex;
}

// Draw tile map to its x and y
void TileMap::draw_map() {
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

// Draw tile map to certain coordinates
void TileMap::draw_map(int newX, int newY) {
  for (uint32_t i = 0; i < mapTilesBack.size(); i++) {
    if ((mapTilesBack.at(i).getX() >= newX - mapTilesBack.at(i).getWidth()) &&
        (mapTilesBack.at(i).getX() < newX + DisplayMode::getDrawWidth()) &&
        (mapTilesBack.at(i).getY() >= newY - mapTilesBack.at(i).getHeight()) &&
        (mapTilesBack.at(i).getY() < newY + DisplayMode::getDrawHeight())) {
      mapTilesBack.at(i).draw(newX, newY, frame);
    }
  }
  for (uint32_t i = 0; i < mapTiles.size(); i++) {
    if ((mapTiles.at(i).getX() >= newX - mapTiles.at(i).getHeight()) &&
        (mapTiles.at(i).getX() < newX + DisplayMode::getDrawWidth()) &&
        (mapTiles.at(i).getY() >= newY - mapTiles.at(i).getHeight()) &&
        (mapTiles.at(i).getY() < newY + DisplayMode::getDrawHeight())) {
      if (!mapTiles.at(i).containsAttribute(spawn)) {
        mapTiles.at(i).draw(newX, newY, frame);
      }
    }
  }
}
