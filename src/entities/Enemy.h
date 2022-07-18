#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <vector>

#include "../globals.h"
#include "../tile/TileMap.h"
#include "../tools.h"
#include "../util/Bitmap.h"
#include "./Player.h"
#include "./Projectile.h"

#define ANIMATION_SPEED 5
#define JUMPING_HEIGHT 192

class Enemy {
 public:
  Enemy(int newX, int newY, int newType);
  ~Enemy();

  void load_images();
  void load_sounds();

  int getX();
  int getY();

  bool getDead();

  void setDead(bool newDead);

  void update(TileMap* newMap, Player* newPlayer);
  void draw(int tile_map_x, int tile_map_y);

 private:
  int x;
  int y;

  int width;
  int height;

  int speed;
  int type;

  // Cusomize!
  int jumpChance;
  int maxSteps;
  int minSteps;
  int jumpHeight;
  int walkChance;
  int jumpSpeed;

  bool canFall;
  bool jumping;
  bool dead;
  int walking_animation_sequence;
  int jumping_animation_sequence;
  int jump_height;
  int characterDir;
  int yVelocity;
  int walking_counter;

  std::vector<Projectile> bullets;
  TileMap* newMap;

  // 0-3 left, 4-7 right, 8 jump left 9 jump right 10 dead
  Bitmap enemy_images[11];

  // Sounds
  ALLEGRO_SAMPLE* walk1;
  ALLEGRO_SAMPLE* walk2;
  ALLEGRO_SAMPLE* jump;
  ALLEGRO_SAMPLE* die;
  ALLEGRO_SAMPLE* getItem;
  ALLEGRO_SAMPLE* getBonus;
};

#endif
