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
#include "../util/JoystickListener.h"
#include "../util/KeyListener.h"
#include "../util/MouseListener.h"

// Main game screen
class Game : public State {
 public:
  /**
   * @brief Construct a new Game object
   *
   */
  Game(KeyListener& keyboardListener,
       MouseListener& mouseListener,
       JoystickListener& joystickListener);

  void init();
  void update(double delta);
  void draw();

 private:
  void initGame();

  Player* player1{nullptr};
  std::vector<Enemy> badGuy{};

  TileMap* tile_map{nullptr};

  KeyListener& keyboardListener;
  MouseListener& mouseListener;
  JoystickListener& joystickListener;
};

#endif
