#ifndef PLAYER_H
#define PLAYER_H

#include <allegro.h>
#include <alpng.h>

#include "globals.h"
#include "tools.h"
#include "projectile.h"

#include "tileMap.h"

#define ANIMATION_SPEED 5
#define JUMPING_HEIGHT 192

class player{
  public:
    player();
    ~player();

    void load_images();
    void load_sounds();

    int getX();
    int getY();

    bool getDead();

    vector<projectile> getBullets();

    void setDead(bool newDead);
    void spawncommand(tileMap *newMap);

    void update(tileMap *newMap);
    void draw(BITMAP* temp, int tile_map_x, int tile_map_y);

  private:
    int x;
    int y;

    bool canFall;
    bool jumping;
    bool jumping_animation_done;
    bool dead;
    bool shooting;
    int shooting_animation_sequence;
    int walking_animation_sequence;
    int jumping_animation_sequence;
    int jump_height;
    int characterDir;
    int yVelocity;

    vector<projectile> bullets;

    // 0-3 left, 4-7 right, 8-11 up 12-17 jump left 18-23 jump right
    BITMAP* player_images[26];

    //Sounds
    SAMPLE* walk1;
    SAMPLE* walk2;
    SAMPLE* jump;
    SAMPLE* die;
    SAMPLE* getItem;
    SAMPLE* getBonus;
};

#endif
