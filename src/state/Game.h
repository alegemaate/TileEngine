#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include "../lib/state/State.h"

#include "../entities/Enemy.h"
#include "../entities/Player.h"
#include "../lib/input/JoystickListener.h"
#include "../lib/input/KeyListener.h"
#include "../lib/input/MouseListener.h"
#include "../tile/TileMap.h"
#include "../tools.h"

// Main game screen
class Game : public State {
 public:
  /**
   * @brief Construct a new Game state
   *
   * @param keyboardListener Keyboard listener
   * @param mouseListener Mouse listener
   * @param joystickListener Joystick listener
   */
  Game(KeyListener& keyboardListener,
       MouseListener& mouseListener,
       JoystickListener& joystickListener);

  /**
   * @brief Initialize the game state
   *
   */
  void init();

  /**
   * @brief Update the game state
   *
   * @param delta Time since last update
   */
  void update(double delta);

  /**
   * @brief Draw the game state
   *
   */
  void draw();

  /// Current level
  static int level;

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
