#ifndef GAME_H
#define GAME_H

#include "GameState.h"

#include <allegro.h>
#include <alpng.h>
#include <string>
#include <vector>

#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

#include "globals.h"
#include "tools.h"

#include "tileMap.h"
#include "player.h"
#include "enemy.h"

using namespace std;

// Main game screen
class Game : public GameState
{
    private:
      BITMAP* buffer;

      // Variables
      int animationFrame;

      // Objects
      player character;
      vector<enemy> badGuy;

      tileMap *tile_map;
      tileMap* newTileMap;

    public:
      //Main loop functions
      Game();
      void init();
      void update();
      void draw();
      ~Game();
};

#endif
