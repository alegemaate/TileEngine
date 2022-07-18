#include "Game.h"

#include <allegro5/allegro_primitives.h>

#include "../util/DisplayMode.h"
#include "../util/KeyListener.h"
#include "../util/Logger.h"
#include "../util/MouseListener.h"

volatile int Game::timer1 = 00;

// Create game state
Game::Game() : frames_done(0) {
  // Creates a random number generator (based on time)
  srand(time(NULL));

  // Player
  player1.load_images(1);
  player1.load_sounds();
  player1.set_keys(ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT,
                   ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_LCTRL, ALLEGRO_KEY_ALT);

  // Init
  init();
}

// Destroy game
Game::~Game() {}

// void Game::gameTicker() {
//   timer1++;
// }
// END_OF_FUNCTION(gameTicker)

// Init game state
void Game::init() {
  // Timer
  // LOCK_VARIABLE(timer1);
  // LOCK_FUNCTION(gameTicker);
  // install_int_ex(gameTicker, BPS_TO_TIMER(100));

  tile_map = new TileMap("blank");

  // Create map
  if (levelOn == 0) {
    tile_map->load("data/levels/level_01");
  } else if (levelOn == 1) {
    tile_map->load("data/levels/level_test");
  } else if (levelOn == 2) {
    tile_map->load("data/saves/danny");
  } else {
    tile_map->load("data/saves/dannyII");
  }

  // Variables
  player1.setDead(false);
  player1.spawncommand(tile_map);

  // Load enemies
  for (uint32_t i = 0; i < tile_map->mapTiles.size(); i++) {
    if (tile_map->mapTiles.at(i).getType() > 199) {
      Enemy newBadGuy(tile_map->mapTiles.at(i).getX(),
                      tile_map->mapTiles.at(i).getY(),
                      tile_map->mapTiles.at(i).getType() - 200);
      newBadGuy.load_images();
      newBadGuy.load_sounds();
      badGuy.push_back(newBadGuy);
      tile_map->mapTiles.at(i).setType(0);
    }
  }
}

// Update game
void Game::update(StateEngine* engine) {
  frames_done++;

  // Character movements (runs only every 2nd loop)
  if (frames_done % 2 == 0) {
    player1.update(tile_map);

    for (uint32_t i = 0; i < badGuy.size(); i++) {
      // badGuy.at(i).update(tile_map, &player1);
    }
  }

  // Scroll Map
  if (player1.getY() - tile_map->y < (DisplayMode::getDrawHeight() / 4) &&
      tile_map->y > 0) {
    // UP
    if (tile_map->y - (DisplayMode::getDrawHeight() / 4 -
                       abs(player1.getY() - tile_map->y)) >=
        0) {
      tile_map->y -= (DisplayMode::getDrawHeight() / 4 -
                      abs(player1.getY() - tile_map->y));
    }
    // Close But not quite
    else {
      tile_map->y = 0;
    }
  }
  if (player1.getY() - tile_map->y > (3 * (DisplayMode::getDrawHeight() / 4)) &&
      tile_map->y <
          (tile_map->height * 64 - DisplayMode::getDrawHeight())) {  // DOWN
    if (tile_map->y + (abs(player1.getY() - tile_map->y) -
                       (3 * (DisplayMode::getDrawHeight() / 4))) <=
        (tile_map->height * 64 - DisplayMode::getDrawHeight())) {
      tile_map->y += (abs(player1.getY() - tile_map->y) -
                      (3 * (DisplayMode::getDrawHeight() / 4)));
    }
    // Close But not quite
    else {
      tile_map->y = (tile_map->height * 64 - DisplayMode::getDrawHeight());
    }
  }
  if (player1.getX() - tile_map->x < (DisplayMode::getDrawWidth() / 4) &&
      tile_map->x > 0) {  // LEFT
    if (tile_map->x - (DisplayMode::getDrawWidth() / 4 -
                       abs(player1.getX() - tile_map->x)) >=
        0) {
      tile_map->x -=
          (DisplayMode::getDrawWidth() / 4 - abs(player1.getX() - tile_map->x));
    }
    // Close But not quite
    else {
      tile_map->x = 0;
    }
  }
  if (player1.getX() - tile_map->x > (3 * (DisplayMode::getDrawWidth() / 4)) &&
      tile_map->x <
          (tile_map->width * 64 - DisplayMode::getDrawWidth())) {  // RIGHT
    if (tile_map->x + (abs(player1.getX() - tile_map->x) -
                       (3 * (DisplayMode::getDrawWidth() / 4))) <=
        (tile_map->width * 64 - DisplayMode::getDrawWidth())) {
      tile_map->x += (abs(player1.getX() - tile_map->x) -
                      (3 * (DisplayMode::getDrawWidth() / 4)));
    }

    // Close But not quite
    else {
      tile_map->x = (tile_map->width * 64 - DisplayMode::getDrawWidth());
    }
  }

  // Die
  if (player1.getDead()) {
    init();
  }

  // Respawn
  if (KeyListener::keyPressed[ALLEGRO_KEY_Y]) {
    player1.spawncommand(tile_map);
  }

  // Spawn enemy
  if (KeyListener::key[ALLEGRO_KEY_R]) {
    if (KeyListener::keyPressed[ALLEGRO_KEY_1] ||
        KeyListener::keyPressed[ALLEGRO_KEY_2] ||
        KeyListener::keyPressed[ALLEGRO_KEY_3]) {
      if (KeyListener::keyPressed[ALLEGRO_KEY_1]) {
        Enemy newBadGuy(player1.getX(), player1.getY(), enemy_vorticon);
        newBadGuy.load_images();
        newBadGuy.load_sounds();
        badGuy.push_back(newBadGuy);
      }
      if (KeyListener::keyPressed[ALLEGRO_KEY_2]) {
        Enemy newBadGuy(player1.getX(), player1.getY(), enemy_robot);
        newBadGuy.load_images();
        newBadGuy.load_sounds();
        badGuy.push_back(newBadGuy);
      }
      if (KeyListener::keyPressed[ALLEGRO_KEY_3]) {
        Enemy newBadGuy(player1.getX(), player1.getY(), enemy_vorticon);
        newBadGuy.load_images();
        newBadGuy.load_sounds();
        badGuy.push_back(newBadGuy);
      }
    }
  }

  //  Back to menu
  if (KeyListener::keyPressed[ALLEGRO_KEY_M]) {
    engine->setNextState(StateEngine::STATE_MENU);
  }
}

// Draw game
void Game::draw() {
  // Black background (just in case)
  al_draw_filled_rectangle(0, 0, 1280, 960, al_map_rgb(0, 0, 0));

  // Draw tiles
  tile_map->draw_map();

  // Draw player1
  player1.draw(tile_map->x, tile_map->y);

  // Frame
  al_draw_filled_rectangle(0, 0, DisplayMode::getDrawWidth(), 16,
                           al_map_rgb(0, 0, 0));
  al_draw_filled_rectangle(0, 0, 16, DisplayMode::getDrawHeight(),
                           al_map_rgb(0, 0, 0));
  al_draw_filled_rectangle(DisplayMode::getDrawWidth() - 16, 0,
                           DisplayMode::getDrawWidth(),
                           DisplayMode::getDrawHeight(), al_map_rgb(0, 0, 0));
  al_draw_filled_rectangle(0, DisplayMode::getDrawHeight() - 16,
                           DisplayMode::getDrawWidth(),
                           DisplayMode::getDrawHeight(), al_map_rgb(0, 0, 0));

  // Draw enemies
  // for (uint32_t i = 0; i < badGuy.size(); i++) {
  //   badGuy.at(i).draw(tile_map->x, tile_map->y);
  // }
}
