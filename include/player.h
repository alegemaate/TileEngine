#ifndef PLAYER_H
#define PLAYER_H

#include <allegro.h>
#include <alpng.h>
#include <vector>

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

    void load_images( int newType);
    void load_sounds();
    void set_keys( int up, int down, int left, int right, int jump, int shoot, int newJoyNumber);

    int getX();
    int getY();
    int getDeathcount();

    bool getFinished();

    bool getDead();

    vector<projectile> getBullets();

    void setFinished( bool newFinished);
    void setDead(bool newDead);
    void setDeathcount(int newDeathcount);
    void spawncommand(tileMap *fullMap);

    void update(tileMap *fullMap);
    void draw(BITMAP* temp, int tile_map_x, int tile_map_y);

  private:
    int x, y;

    int deathcount;
    int idle_timer;

    float sprintSpeed;

    bool canFall;
    bool jumping;
    bool jumping_animation_done;
    bool dead;
    bool sprinting;
    bool finished;
    bool shooting;
    int shooting_animation_sequence;
    int walking_animation_sequence;
    int jumping_animation_sequence;
    int jump_height;
    int characterDir;
    int yVelocity;

    // Keys
    int upKey, downKey, leftKey, rightKey, jumpKey, shootKey;
    int joyNumber;

    vector<projectile> bullets;
    tileMap *newMap;

    // 0-3 left, 4-7 right, 8-11 up 12-17 jump left 18-23 jump right 24-27 slide 28-19 is idle
    BITMAP* player_images[30];

    //Sounds
    SAMPLE* walk1;
    SAMPLE* walk2;
    SAMPLE* jump;
    SAMPLE* die;
    SAMPLE* getItem;
    SAMPLE* getBonus;
    SAMPLE* win;
};

#endif
