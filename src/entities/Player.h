#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <vector>

#include "../globals.h"
#include "../tile/TileMap.h"
#include "../tools.h"
#include "../util/Bitmap.h"
#include "./Projectile.h"

#define ANIMATION_SPEED 5
#define JUMPING_HEIGHT 192

class Player {
 public:
  Player();
  ~Player();

  void load_images(int newType);
  void load_sounds();
  void set_keys(int up, int down, int left, int right, int jump, int shoot);

  int getX();
  int getY();
  int getDeathcount();

  bool getFinished();

  bool getDead();

  vector<Projectile> getBullets();

  void setFinished(bool newFinished);
  void setDead(bool newDead);
  void setDeathcount(int newDeathcount);
  void spawncommand(TileMap* fullMap);

  void update(TileMap* fullMap);
  void draw(int tile_map_x, int tile_map_y);

 private:
  int x{0};
  int y{0};

  int deathcount{0};
  int idle_timer{0};

  float sprintSpeed{0.0f};

  bool canFall{false};
  bool jumping{false};
  bool jumping_animation_done{false};
  bool dead{false};
  bool sprinting{false};
  bool finished{false};
  bool shooting{false};
  int shooting_animation_sequence{0};
  int walking_animation_sequence{0};
  int jumping_animation_sequence{0};
  int jump_height{0};
  int characterDir{0};
  int yVelocity{0};

  // Keys
  int upKey{0};
  int downKey{0};
  int leftKey{0};
  int rightKey{0};
  int jumpKey{0};
  int shootKey{0};

  vector<Projectile> bullets{};
  TileMap* newMap{nullptr};

  // 0-3 left, 4-7 right, 8-11 up 12-17 jump left 18-23 jump right 24-27 slide
  // 28-19 is idle
  Bitmap player_images[30]{};

  // Sounds
  ALLEGRO_SAMPLE* walk1{nullptr};
  ALLEGRO_SAMPLE* walk2{nullptr};
  ALLEGRO_SAMPLE* jump{nullptr};
  ALLEGRO_SAMPLE* die{nullptr};
  ALLEGRO_SAMPLE* getItem{nullptr};
  ALLEGRO_SAMPLE* getBonus{nullptr};
  ALLEGRO_SAMPLE* win{nullptr};
};

#endif
