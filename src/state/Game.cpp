#include "Game.h"

#include <allegro5/allegro_primitives.h>

#include "../util/DisplayMode.h"
#include "../util/KeyListener.h"
#include "../util/Logger.h"
#include "../util/MouseListener.h"

// Constructor
Game::Game(KeyListener& keyboardListener,
           MouseListener& mouseListener,
           JoystickListener& joystickListener)
    : keyboardListener(keyboardListener),
      mouseListener(mouseListener),
      joystickListener(joystickListener) {}

// Create game state
void Game::init() {
  // Creates a random number generator (based on time)
  srand(time(NULL));

  // Player
  player1 = new Player(keyboardListener);
  player1->load_images();
  player1->load_sounds();
  player1->set_keys(Key::UP, Key::DOWN, Key::LEFT, Key::RIGHT, Key::LCTRL,
                    Key::ALT);

  // Init
  initGame();
}

// Init game state
void Game::initGame() {
  tile_map = new TileMap("blank");

  // Create map
  if (levelOn == 0) {
    tile_map->load("data/levels/level_01.txt");
  } else if (levelOn == 1) {
    tile_map->load("data/levels/level_test.txt");
  } else if (levelOn == 2) {
    tile_map->load("data/saves/danny.txt");
  } else {
    tile_map->load("data/saves/dannyII.txt");
  }

  // Variables
  player1->setDead(false);
  player1->spawncommand(tile_map);

  // Load enemies
  for (uint32_t i = 0; i < tile_map->mapTiles.size(); i++) {
    if (tile_map->mapTiles.at(i).getType() > 199) {
      Enemy newBadGuy(tile_map->mapTiles.at(i).getX(),
                      tile_map->mapTiles.at(i).getY(),
                      tile_map->mapTiles.at(i).getType() - 200);
      newBadGuy.load_images();
      newBadGuy.load_sounds();
      badGuy.push_back(newBadGuy);
      tile_map->mapTiles.at(i).setType(tile_map->findTileType(0));
    }
  }
}

// Update game
void Game::update(double delta) {
  tile_map->update(delta);

  // Character movements (runs only every 2nd loop)
  player1->update(tile_map, delta);

  for (uint32_t i = 0; i < badGuy.size(); i++) {
    badGuy.at(i).update(tile_map, player1);
  }

  // Scroll Map
  if (player1->getY() - tile_map->y < (DisplayMode::getDrawHeight() / 4) &&
      tile_map->y > 0) {
    // UP
    if (tile_map->y - (DisplayMode::getDrawHeight() / 4 -
                       abs(player1->getY() - tile_map->y)) >=
        0) {
      tile_map->y -= (DisplayMode::getDrawHeight() / 4 -
                      abs(player1->getY() - tile_map->y));
    }
    // Close But not quite
    else {
      tile_map->y = 0;
    }
  }
  if (player1->getY() - tile_map->y >
          (3 * (DisplayMode::getDrawHeight() / 4)) &&
      tile_map->y <
          (tile_map->height * 64 - DisplayMode::getDrawHeight())) {  // DOWN
    if (tile_map->y + (abs(player1->getY() - tile_map->y) -
                       (3 * (DisplayMode::getDrawHeight() / 4))) <=
        (tile_map->height * 64 - DisplayMode::getDrawHeight())) {
      tile_map->y += (abs(player1->getY() - tile_map->y) -
                      (3 * (DisplayMode::getDrawHeight() / 4)));
    }
    // Close But not quite
    else {
      tile_map->y = (tile_map->height * 64 - DisplayMode::getDrawHeight());
    }
  }
  if (player1->getX() - tile_map->x < (DisplayMode::getDrawWidth() / 4) &&
      tile_map->x > 0) {  // LEFT
    if (tile_map->x - (DisplayMode::getDrawWidth() / 4 -
                       abs(player1->getX() - tile_map->x)) >=
        0) {
      tile_map->x -= (DisplayMode::getDrawWidth() / 4 -
                      abs(player1->getX() - tile_map->x));
    }
    // Close But not quite
    else {
      tile_map->x = 0;
    }
  }
  if (player1->getX() - tile_map->x > (3 * (DisplayMode::getDrawWidth() / 4)) &&
      tile_map->x <
          (tile_map->width * 64 - DisplayMode::getDrawWidth())) {  // RIGHT
    if (tile_map->x + (abs(player1->getX() - tile_map->x) -
                       (3 * (DisplayMode::getDrawWidth() / 4))) <=
        (tile_map->width * 64 - DisplayMode::getDrawWidth())) {
      tile_map->x += (abs(player1->getX() - tile_map->x) -
                      (3 * (DisplayMode::getDrawWidth() / 4)));
    }

    // Close But not quite
    else {
      tile_map->x = (tile_map->width * 64 - DisplayMode::getDrawWidth());
    }
  }

  // Die
  if (player1->getDead()) {
    initGame();
  }

  // Respawn
  if (keyboardListener.wasPressed(Key::Y)) {
    player1->spawncommand(tile_map);
  }

  // Spawn enemy
  if (keyboardListener.isDown(Key::R)) {
    if (keyboardListener.wasPressed(Key::NUM_1) ||
        keyboardListener.wasPressed(Key::NUM_2) ||
        keyboardListener.wasPressed(Key::NUM_3)) {
      if (keyboardListener.wasPressed(Key::NUM_1)) {
        Enemy newBadGuy(player1->getX(), player1->getY(), enemy_vorticon);
        newBadGuy.load_images();
        newBadGuy.load_sounds();
        badGuy.push_back(newBadGuy);
      }
      if (keyboardListener.wasPressed(Key::NUM_2)) {
        Enemy newBadGuy(player1->getX(), player1->getY(), enemy_robot);
        newBadGuy.load_images();
        newBadGuy.load_sounds();
        badGuy.push_back(newBadGuy);
      }
      if (keyboardListener.wasPressed(Key::NUM_3)) {
        Enemy newBadGuy(player1->getX(), player1->getY(), enemy_vorticon);
        newBadGuy.load_images();
        newBadGuy.load_sounds();
        badGuy.push_back(newBadGuy);
      }
    }
  }

  //  Back to menu
  if (keyboardListener.wasPressed(Key::M)) {
    setNextState(ProgramState::MENU);
  }
}

// Draw game
void Game::draw() {
  // Black background (just in case)
  al_draw_filled_rectangle(0, 0, 1280, 960, al_map_rgb(0, 0, 0));

  // Draw tiles
  tile_map->draw_map();

  // Draw player1
  player1->draw(tile_map->x, tile_map->y);

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
  for (uint32_t i = 0; i < badGuy.size(); i++) {
    badGuy.at(i).draw(tile_map->x, tile_map->y);
  }
}
