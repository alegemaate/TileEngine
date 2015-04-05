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
    static volatile int timer1;
    bool gameBegin;
    int totalTime[2];

    // Lighting effects
    COLOR_MAP light_table;
    PALLETE pal;
    bool lightingEnabled;
    BITMAP *darkness, *darkness_old, *lightBuffer, *spotlight;

    // Objects
    player player1;
    vector<enemy> badGuy;

    tileMap *tile_map;

    static void gameTicker();

  public:
    //Main loop functions
    Game();
    void init();
    void update();
    void draw();
    ~Game();
};

#endif
