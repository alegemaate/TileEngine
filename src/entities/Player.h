#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "./CharacterDirection.h"
#include "./Projectile.h"

#include "../lib/assets/Bitmap.h"
#include "../lib/assets/Sound.h"
#include "../lib/input/KeyListener.h"
#include "../tile/TileMap.h"
#include "../tools.h"

#define ANIMATION_SPEED 5
#define JUMPING_HEIGHT 192

class Player {
 public:
  Player(KeyListener& keyboardListener);
  ~Player() = default;

  void load_images();
  void load_sounds();
  void setKeys(Key up, Key down, Key left, Key right, Key jump, Key shoot);

  int getX();
  int getY();

  bool getFinished();

  bool getDead();

  std::vector<Projectile> getBullets();

  void setFinished(bool newFinished);
  void setDead(bool newDead);
  void setDeathcount(int newDeathcount);
  void spawncommand(TileMap* fullMap);

  void update(TileMap* fullMap, double delta);
  void draw(int tile_map_x, int tile_map_y);

 private:
  int x{0};
  int y{0};

  float sprintSpeed{0.0f};

  bool canFall{false};
  bool jumping{false};
  bool jumping_animation_done{true};
  bool dead{false};
  bool sprinting{false};
  bool finished{false};
  bool shooting{false};
  int shooting_animation_sequence{0};
  int walking_animation_sequence{0};
  int jumping_animation_sequence{0};
  int jump_height{0};
  int yVelocity{0};
  CharacterDirection characterDir{CharacterDirection::RIGHT};

  // Keys
  Key upKey{Key::UNKNOWN};
  Key downKey{Key::UNKNOWN};
  Key leftKey{Key::UNKNOWN};
  Key rightKey{Key::UNKNOWN};
  Key jumpKey{Key::UNKNOWN};
  Key shootKey{Key::UNKNOWN};

  std::vector<Projectile> bullets{};

  // 0-3 left, 4-7 right, 8-11 up 12-17 jump left 18-23 jump right 24-27 slide
  // 28-19 is idle
  Bitmap player_images[30]{};

  // Sounds
  Sound walk1{};
  Sound walk2{};
  Sound jump{};
  Sound die{};
  Sound getItem{};
  Sound getBonus{};
  Sound win{};

  KeyListener& keyboardListener;
};

#endif
