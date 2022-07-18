#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include "./engine/State.h"

#include "../entities/Enemy.h"
#include "../entities/Player.h"
#include "../globals.h"
#include "../tile/TileMap.h"
#include "../tools.h"

// Main game screen
class Game : public State {
 public:
  // Main loop functions
  Game();
  ~Game();

  void update(double delta);
  void init();
  void draw();

 private:
  Player player1{};
  std::vector<Enemy> badGuy{};

  TileMap* tile_map{nullptr};
};

#endif
