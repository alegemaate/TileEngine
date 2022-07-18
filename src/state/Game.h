#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <string>
#include <vector>

#include "./engine/State.h"

#include "../entities/Enemy.h"
#include "../entities/Player.h"
#include "../globals.h"
#include "../tile/TileMap.h"
#include "../tools.h"

using namespace std;

// Main game screen
class Game : public State {
 public:
  // Main loop functions
  Game();
  ~Game();

  void update(StateEngine* engine);
  void init();
  void draw();

 private:
  // Variables
  int animationFrame;
  static volatile int timer1;
  bool gameBegin;
  int totalTime[2];

  int frames_done = 0;

  // Objects
  Player player1;
  vector<Enemy> badGuy;

  TileMap* tile_map;

  static void gameTicker();
};

#endif
