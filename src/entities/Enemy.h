#ifndef ENEMY_H
#define ENEMY_H

#include <vector>

#include "./CharacterDirection.h"
#include "./EnemyType.h"
#include "./Player.h"
#include "./Projectile.h"

#include "../lib/assets/Bitmap.h"
#include "../lib/assets/Sound.h"
#include "../tile/TileMap.h"
#include "../tools.h"

#define ANIMATION_SPEED 5
#define JUMPING_HEIGHT 192

class Enemy {
 public:
  Enemy(int newX, int newY, EnemyType newType);

  int getX();
  int getY();

  bool getDead();

  void setDead(bool newDead);

  void update(TileMap* fullMap, Player* newPlayer);
  void draw(int tile_map_x, int tile_map_y);

 private:
  int x{0};
  int y{0};

  int width{0};
  int height{0};

  int speed{0};
  EnemyType type{EnemyType::VORTICON};

  // Cusomize!
  int jumpChance{0};
  int maxSteps{0};
  int minSteps{0};
  int jumpHeight{0};
  int walkChance{0};
  int jumpSpeed{0};

  bool canFall{false};
  bool jumping{false};
  bool dead{false};
  int walking_animation_sequence{0};
  int jumping_animation_sequence{0};
  int jump_height{0};
  int yVelocity{0};
  int walking_counter{0};

  CharacterDirection characterDir{CharacterDirection::RIGHT};

  std::vector<Projectile> bullets{};

  // 0-3 left, 4-7 right, 8 jump left 9 jump right 10 dead
  Bitmap enemy_images[11]{};

  // Sounds
  Sound walk1;
  Sound walk2;
  Sound jump;
  Sound die;
  Sound getItem;
  Sound getBonus;
};

#endif
