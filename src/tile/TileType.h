#ifndef TILE_TILE_TYPE_H_
#define TILE_TILE_TYPE_H_

#include <string>
#include <vector>

#include "../util/Bitmap.h"

enum class TileAttribute {
  GAS = 0,
  SOLID = 1,
  LIQUID = 2,
  CLIMB = 3,
  HARMFUL = 4,
  INTERACTIVE = 5,
  ITEM = 6,
  SPAWN = 7,
  HALF_BLOCK_TOP = 8,
  HALF_BLOCK_BOTTOM = 9,
  QUARTER_BLOCK_TOP = 10,
  QUARTER_BLOCK_BOTTOM = 11,
  LIGHT = 12,
  FINISH = 13,
};

struct TileType {
  uint8_t id;

  std::vector<TileAttribute> attributes;
  std::vector<Bitmap> frames;
  std::string name;

  bool particles;
  bool lighting;
};

#endif  // TILE_TILE_TYPE_H_