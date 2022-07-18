#include "TileMap.h"

#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_print.hpp"

#include "../util/DisplayMode.h"
#include "../util/Logger.h"

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
  rapidxml::xml_document<> doc;
  ifstream file;

  // Check exist
  if (fexists("data/tiles.xml")) {
    file.open("data/tiles.xml");
  } else {
    Logger::fatal(
        "Cannot find file /data/tiles.xml \n Please check your files and try "
        "again");
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content(buffer.str());
  doc.parse<0>(&content[0]);

  rapidxml::xml_node<>* loadingTiles = doc.first_node();

  // Load blocks
  for (rapidxml::xml_node<>* cTile = loadingTiles->first_node("tile"); cTile;
       cTile = cTile->next_sibling()) {
    // Read xml variables
    string tileName = cTile->first_node("name")->value();
    int tileId = atoi(cTile->first_node("id")->value());

    bool tileParticlesEnabled =
        cTile->first_node("particles")->value() == "true" ? true : false;
    bool tileLightingEnabled =
        cTile->first_node("lighting")->value() == "true" ? true : false;

    Tile newTile(tileId, tileParticlesEnabled, tileLightingEnabled, tileName);

    // Attributes
    for (rapidxml::xml_node<>* bTile =
             cTile->first_node("types")->first_node("type");
         bTile; bTile = bTile->next_sibling()) {
      string tileType = bTile->value();
      int tileTypeInt = 0;
      // Type
      if (tileType == "gas")
        tileTypeInt = 0;
      else if (tileType == "solid")
        tileTypeInt = 1;
      else if (tileType == "liquid")
        tileTypeInt = 2;
      else if (tileType == "climb")
        tileTypeInt = 3;
      else if (tileType == "harmful")
        tileTypeInt = 4;
      else if (tileType == "interactive")
        tileTypeInt = 5;
      else if (tileType == "item")
        tileTypeInt = 6;
      else if (tileType == "spawn")
        tileTypeInt = 7;
      else if (tileType == "half_block_top")
        tileTypeInt = 8;
      else if (tileType == "half_block_bottom")
        tileTypeInt = 9;
      else if (tileType == "quarter_block_top")
        tileTypeInt = 10;
      else if (tileType == "quarter_block_bottom")
        tileTypeInt = 11;
      else if (tileType == "light")
        tileTypeInt = 12;
      else if (tileType == "finish")
        tileTypeInt = 13;
      else
        Logger::fatal(("Unknown type (" + tileType +
                       ") in node: type for tile: " + tileName)
                          .c_str());

      newTile.addAttribute(tileTypeInt);
    }

    // Images
    int counter = 0;
    string images[8];
    for (rapidxml::xml_node<>* bTile =
             cTile->first_node("images")->first_node("image");
         bTile; bTile = bTile->next_sibling()) {
      images[counter] = bTile->value();
      counter++;
    }

    // Set images
    if (images[0].length() > 0) {
      if (images[1].length() > 0) {
        if (images[4].length() > 0) {
          newTile.setImages(al_load_bitmap(images[0].c_str()),
                            al_load_bitmap(images[1].c_str()),
                            al_load_bitmap(images[2].c_str()),
                            al_load_bitmap(images[3].c_str()),
                            al_load_bitmap(images[4].c_str()),
                            al_load_bitmap(images[5].c_str()),
                            al_load_bitmap(images[6].c_str()),
                            al_load_bitmap(images[7].c_str()));
        } else {
          newTile.setImages(al_load_bitmap(images[0].c_str()),
                            al_load_bitmap(images[1].c_str()),
                            al_load_bitmap(images[2].c_str()),
                            al_load_bitmap(images[3].c_str()));
        }
      } else {
        newTile.setImages(al_load_bitmap(images[0].c_str()));
      }
    } else {
      Logger::fatal(("Need images for tile: " + tileName).c_str());
    }

    // Index for other tiles to reference
    tileIndex.push_back(newTile);
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
      mapTilesBack.at(i).draw_tile(x, y, frame);
    }
  }
  for (uint32_t i = 0; i < mapTiles.size(); i++) {
    if ((mapTiles.at(i).getX() >= x - mapTiles.at(i).getWidth()) &&
        (mapTiles.at(i).getX() < x + DisplayMode::getDrawWidth()) &&
        (mapTiles.at(i).getY() >= y - mapTiles.at(i).getHeight()) &&
        (mapTiles.at(i).getY() < y + DisplayMode::getDrawHeight())) {
      if (!mapTiles.at(i).containsAttribute(spawn)) {
        mapTiles.at(i).draw_tile(x, y, frame);
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
      mapTilesBack.at(i).draw_tile(newX, newY, frame);
    }
  }
  for (uint32_t i = 0; i < mapTiles.size(); i++) {
    if ((mapTiles.at(i).getX() >= newX - mapTiles.at(i).getHeight()) &&
        (mapTiles.at(i).getX() < newX + DisplayMode::getDrawWidth()) &&
        (mapTiles.at(i).getY() >= newY - mapTiles.at(i).getHeight()) &&
        (mapTiles.at(i).getY() < newY + DisplayMode::getDrawHeight())) {
      if (!mapTiles.at(i).containsAttribute(spawn)) {
        mapTiles.at(i).draw_tile(newX, newY, frame);
      }
    }
  }
}
