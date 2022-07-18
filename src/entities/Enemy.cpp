#include "Enemy.h"

#include <allegro5/allegro_audio.h>

#include "../util/DisplayMode.h"
#include "../util/Logger.h"

Enemy::Enemy(int newX, int newY, int newType) {
  width = 0;
  height = 0;

  speed = 0;

  jumpChance = 0;
  maxSteps = 0;
  minSteps = 0;
  jumpHeight = 0;
  walkChance = 0;
  jumpSpeed = 0;

  canFall = false;
  jumping = false;
  dead = false;
  walking_animation_sequence = 0;
  jumping_animation_sequence = 0;
  jump_height = 0;
  characterDir = 0;
  yVelocity = 0;
  walking_counter = 0;

  x = newX;
  y = newY;
  type = newType;

  newMap = new TileMap("blank");
}

Enemy::~Enemy() {}

// 0-3 left, 4-7 right, 8-11 up
void Enemy::load_images() {
  if (type == enemy_vorticon) {
    enemy_images[0] = Bitmap("images/enemys/vorticon/left_1.png");
    enemy_images[1] = Bitmap("images/enemys/vorticon/left_2.png");
    enemy_images[2] = Bitmap("images/enemys/vorticon/left_3.png");
    enemy_images[3] = Bitmap("images/enemys/vorticon/left_4.png");

    enemy_images[4] = Bitmap("images/enemys/vorticon/right_1.png");
    enemy_images[5] = Bitmap("images/enemys/vorticon/right_2.png");
    enemy_images[6] = Bitmap("images/enemys/vorticon/right_3.png");
    enemy_images[7] = Bitmap("images/enemys/vorticon/right_4.png");

    enemy_images[8] = Bitmap("images/enemys/vorticon/jump_left.png");
    enemy_images[9] = Bitmap("images/enemys/vorticon/jump_right.png");

    enemy_images[10] = Bitmap("images/enemys/vorticon/dead.png");

    jumpChance = 100;
    maxSteps = 100;
    minSteps = 20;
    jumpHeight = 128;
    walkChance = 20;
    jumpSpeed = 2;
    speed = 3;
  } else if (type == enemy_robot) {
    enemy_images[0] = Bitmap("images/enemys/robot/left_1.png");
    enemy_images[1] = Bitmap("images/enemys/robot/left_2.png");
    enemy_images[2] = Bitmap("images/enemys/robot/left_3.png");
    enemy_images[3] = Bitmap("images/enemys/robot/left_4.png");

    enemy_images[4] = Bitmap("images/enemys/robot/right_1.png");
    enemy_images[5] = Bitmap("images/enemys/robot/right_2.png");
    enemy_images[6] = Bitmap("images/enemys/robot/right_3.png");
    enemy_images[7] = Bitmap("images/enemys/robot/right_4.png");

    enemy_images[8] = Bitmap("images/enemys/robot/jump_left.png");
    enemy_images[9] = Bitmap("images/enemys/robot/jump_right.png");

    enemy_images[10] = Bitmap("images/enemys/robot/dead.png");

    jumpChance = 0;
    maxSteps = 100;
    minSteps = 20;
    jumpHeight = 0;
    walkChance = 1;
    jumpSpeed = 0;
    speed = 2;
  } else if (type == enemy_danny) {
    enemy_images[0] = Bitmap("images/enemys/danny/left_1.png");
    enemy_images[1] = Bitmap("images/enemys/danny/left_2.png");
    enemy_images[2] = Bitmap("images/enemys/danny/left_3.png");
    enemy_images[3] = Bitmap("images/enemys/danny/left_4.png");

    enemy_images[4] = Bitmap("images/enemys/danny/right_1.png");
    enemy_images[5] = Bitmap("images/enemys/danny/right_2.png");
    enemy_images[6] = Bitmap("images/enemys/danny/right_3.png");
    enemy_images[7] = Bitmap("images/enemys/danny/right_4.png");

    enemy_images[8] = Bitmap("images/enemys/danny/jump_left.png");
    enemy_images[9] = Bitmap("images/enemys/danny/jump_right.png");

    enemy_images[10] = Bitmap("images/enemys/danny/dead.png");

    jumpChance = 20;
    maxSteps = 200;
    minSteps = 100;
    jumpHeight = 256;
    walkChance = 1;
    jumpSpeed = 3;
    speed = 4;
  }
  width = enemy_images[0].getWidth();
  height = enemy_images[0].getHeight();
}

// Load sounds
void Enemy::load_sounds() {
  if (!(walk1 = al_load_sample("sounds/keen_walk_1.wav"))) {
    Logger::fatal(
        "Cannot find sound sounds/walk_1.wav \n Please check your files and "
        "try again");
  }
  if (!(walk2 = al_load_sample("sounds/keen_walk_2.wav"))) {
    Logger::fatal(
        "Cannot find sound sounds/walk_2.wav \n Please check your files and "
        "try again");
  }
  if (!(jump = al_load_sample("sounds/keen_jump.wav"))) {
    Logger::fatal(
        "Cannot find sound sounds/jump.wav \n Please check your files and try "
        "again");
  }
  if (!(die = al_load_sample("sounds/keen_die.wav"))) {
    Logger::fatal(
        "Cannot find sound sounds/die.wav \n Please check your files and try "
        "again");
  }
  if (!(getItem = al_load_sample("sounds/get_item.wav"))) {
    Logger::fatal(
        "Cannot find sound sounds/getItem.wav \n Please check your files and "
        "try again");
  }
  if (!(getBonus = al_load_sample("sounds/get_bonus.wav"))) {
    Logger::fatal(
        "Cannot find sound sounds/getBonus.wav \n Please check your files and "
        "try again");
  }
}

// Return X
int Enemy::getX() {
  return x;
}

// Return Y
int Enemy::getY() {
  return y;
}

// Dead?
bool Enemy::getDead() {
  return dead;
}

// Set dead
void Enemy::setDead(bool newDead) {
  dead = newDead;
}

// Draw enemy
void Enemy::draw(int tile_map_x, int tile_map_y) {
  if (!dead) {
    if (jumping || canFall) {
      if (characterDir == LEFT) {
        enemy_images[8].draw(x - tile_map_x, y - tile_map_y);
      } else {
        enemy_images[9].draw(x - tile_map_x, y - tile_map_y);
      }
    } else {
      enemy_images[characterDir + walking_animation_sequence / ANIMATION_SPEED]
          .draw(x - tile_map_x, y - tile_map_y);
    }

    // Draw bullets
    for (uint32_t i = 0; i < bullets.size(); i++) {
      bullets.at(i).draw(tile_map_x, tile_map_y);
    }
  } else {
    enemy_images[10].draw(x - tile_map_x, y - tile_map_y);
  }
}

// Update
void Enemy::update(TileMap* fullMap, Player* newPlayer) {
  // Only update if on screen (save CPU power)!
  bool onScreen = true;
  if (collisionAny(x, x + width,
                   newPlayer->getX() - DisplayMode::getDrawWidth(),
                   newPlayer->getX() + DisplayMode::getDrawWidth(), y,
                   y + height, newPlayer->getY() - DisplayMode::getDrawHeight(),
                   newPlayer->getY() + DisplayMode::getDrawHeight())) {
    onScreen = true;
  } else {
    onScreen = false;
  }

  if (onScreen) {
    if (!dead) {
      // Collision stuff
      bool canMoveLeft = true;
      bool canMoveRight = true;
      bool canClimbUp = true;
      bool canClimbDown = true;
      bool canClimbUp2 = false;
      bool canClimbDown2 = false;
      bool canJump = true;
      bool canJumpUp = true;
      bool inLiquid = false;

      Logger::log("here 1");

      newMap->mapTiles.clear();
      newMap->width = fullMap->width;
      newMap->height = fullMap->height;

      // Add close elements
      for (uint32_t i = 0; i < fullMap->mapTiles.size(); i++) {
        if (collisionAny(x - 140, x + 140, fullMap->mapTiles.at(i).getX(),
                         fullMap->mapTiles.at(i).getX() +
                             fullMap->mapTiles.at(i).getWidth(),
                         y - 140, y + 140, fullMap->mapTiles.at(i).getY(),
                         fullMap->mapTiles.at(i).getY() +
                             fullMap->mapTiles.at(i).getHeight())) {
          newMap->mapTiles.push_back(fullMap->mapTiles.at(i));
        }
      }

      Logger::log("here 2");

      // Check for collision
      for (uint32_t i = 0; i < newMap->mapTiles.size(); i++) {
        if (newMap->mapTiles.at(i).containsAttribute(solid)) {
          if (collisionAny(x - speed, x + width - speed,
                           newMap->mapTiles.at(i).getX(),
                           newMap->mapTiles.at(i).getX() + 64, y, y + height,
                           newMap->mapTiles.at(i).getY(),
                           newMap->mapTiles.at(i).getY() + 64) &&
              collisionLeft(x - speed, x + width - speed,
                            newMap->mapTiles.at(i).getX(),
                            newMap->mapTiles.at(i).getX() + 64)) {
            canMoveLeft = false;
          }
        }
        if (newMap->mapTiles.at(i).containsAttribute(solid)) {
          if (collisionAny(x + speed, x + width + speed,
                           newMap->mapTiles.at(i).getX(),
                           newMap->mapTiles.at(i).getX() + 64, y, y + height,
                           newMap->mapTiles.at(i).getY(),
                           newMap->mapTiles.at(i).getY() + 64) &&
              collisionRight(x + speed, x + width + speed,
                             newMap->mapTiles.at(i).getX(),
                             newMap->mapTiles.at(i).getX() + 64)) {
            canMoveRight = false;
          }
        }
        if (newMap->mapTiles.at(i).containsAttribute(climb)) {
          if (collisionAny(x, x + width, newMap->mapTiles.at(i).getX(),
                           newMap->mapTiles.at(i).getX() + 64, y, y + height,
                           newMap->mapTiles.at(i).getY(),
                           newMap->mapTiles.at(i).getY() + 64)) {
            canClimbUp2 = true;
          }
        }
        if (newMap->mapTiles.at(i).containsAttribute(climb)) {
          if (collisionAny(x, x + width, newMap->mapTiles.at(i).getX(),
                           newMap->mapTiles.at(i).getX() + 64, y,
                           y + height + 16, newMap->mapTiles.at(i).getY(),
                           newMap->mapTiles.at(i).getY() + 64)) {
            canClimbDown2 = true;
          }
        }
        if (newMap->mapTiles.at(i).containsAttribute(solid)) {
          if (collisionAny(x, x + width, newMap->mapTiles.at(i).getX(),
                           newMap->mapTiles.at(i).getX() + 64, y - 16, y,
                           newMap->mapTiles.at(i).getY(),
                           newMap->mapTiles.at(i).getY() + 64)) {
            canClimbUp = false;
          }
        }
        if (newMap->mapTiles.at(i).containsAttribute(solid)) {
          if (collisionAny(x, x + width, newMap->mapTiles.at(i).getX(),
                           newMap->mapTiles.at(i).getX() + 64, y,
                           y + height + 16, newMap->mapTiles.at(i).getY(),
                           newMap->mapTiles.at(i).getY() + 64)) {
            canClimbDown = false;
          }
        }
        if (!newMap->mapTiles.at(i).containsAttribute(gas) &&
            !newMap->mapTiles.at(i).containsAttribute(liquid)) {
          if (collisionAny(x, x + width, newMap->mapTiles.at(i).getX(),
                           newMap->mapTiles.at(i).getX() + 64, y, y + height,
                           newMap->mapTiles.at(i).getY(),
                           newMap->mapTiles.at(i).getY() + 64)) {
            canJump = false;
          }
        }
        if (!newMap->mapTiles.at(i).containsAttribute(gas) &&
            !newMap->mapTiles.at(i).containsAttribute(liquid)) {
          if (collisionAny(x, x + width, newMap->mapTiles.at(i).getX(),
                           newMap->mapTiles.at(i).getX() + 64,
                           newMap->mapTiles.at(i).getY(),
                           newMap->mapTiles.at(i).getY() + 64, y, y + 144) &&
              collisionTop(newMap->mapTiles.at(i).getY(),
                           newMap->mapTiles.at(i).getY() + 128, y + yVelocity,
                           y + height + 16 + yVelocity)) {
            canJumpUp = false;
          }
        }
        if (!newMap->mapTiles.at(i).containsAttribute(liquid)) {
          if (collisionAny(x, x + width, newMap->mapTiles.at(i).getX(),
                           newMap->mapTiles.at(i).getX() + 64, y - 16,
                           y + height, newMap->mapTiles.at(i).getY(),
                           newMap->mapTiles.at(i).getY() + 64)) {
            inLiquid = true;
          }
        }
      }
      Logger::log("here 3");

      // Check for points and dangers
      for (uint32_t i = 0; i < newMap->mapTiles.size(); i++) {
        if (collisionAny(x - width * 2, x + width * 2,
                         newMap->mapTiles.at(i).getX(),
                         newMap->mapTiles.at(i).getX() + 64, y - height * 2,
                         y + height * 2, newMap->mapTiles.at(i).getY(),
                         newMap->mapTiles.at(i).getY() + 64)) {
          // Get point
          // if (newMap->mapTiles.at(i).getType() == tile_lollypop) {
          //   if (collisionAny(x, x + width, newMap->mapTiles.at(i).getX(),
          //                    newMap->mapTiles.at(i).getX() + 64, y - 16,
          //                    y + height, newMap->mapTiles.at(i).getY(),
          //                    newMap->mapTiles.at(i).getY() + 128)) {
          //     al_play_sample(getBonus, 1.0f, 0.0f, 1.0f,
          //     ALLEGRO_PLAYMODE_ONCE,
          //                    0);
          //     newMap->mapTiles.at(i).setType(0);
          //   }
          // }

          // Die
          if (newMap->mapTiles.at(i).containsAttribute(harmful)) {
            if (collisionAny(x, x + width, newMap->mapTiles.at(i).getX(),
                             newMap->mapTiles.at(i).getX() + 64, y - 16,
                             y + height, newMap->mapTiles.at(i).getY(),
                             newMap->mapTiles.at(i).getY() + 128)) {
              dead = true;
            }
          }
        }
      }
      Logger::log("here 4");

      // Check collision with bullets
      for (uint32_t i = 0; i < newPlayer->getBullets().size(); i++) {
        if (collisionAny(x, x + width, newPlayer->getBullets().at(i).getX(),
                         newPlayer->getBullets().at(i).getX() + 64, y,
                         y + height, newPlayer->getBullets().at(i).getY(),
                         newPlayer->getBullets().at(i).getY() + 64)) {
          dead = true;
        }
      }

      // AI set direction of walking
      if (walking_counter == 0) {
        if (random(0, walkChance) == 0 && walkChance != 0) {
          if (random(0, 1) == 0) {
            characterDir = RIGHT;
          } else {
            characterDir = LEFT;
          }
          walking_counter = random(minSteps, maxSteps);
        }
      }
      if (walking_counter > 0) {
        walking_counter--;
      }

      Logger::log("here 5");
      // Move right
      if (characterDir == RIGHT && walking_counter > 0) {
        if (canMoveRight) {
          x += speed;
          walking_animation_sequence++;
          if (walking_animation_sequence % (ANIMATION_SPEED * 2) == 0 &&
              !canFall && !jumping) {
            if (random(0, 1)) {
              al_play_sample(walk1, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
            } else {
              al_play_sample(walk2, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
            }
          }
        }
      }

      // Move left
      if (characterDir == LEFT && walking_counter > 0) {
        if (canMoveLeft) {
          x -= speed;
          walking_animation_sequence++;
          if (walking_animation_sequence % (ANIMATION_SPEED * 2) == 0 &&
              !canFall && !jumping) {
            if (random(0, 1)) {
              al_play_sample(walk1, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
            } else {
              al_play_sample(walk2, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
            }
          }
        }
      }

      Logger::log("here 6");

      // Jumping
      if (jumping && canJumpUp) {
        if (canFall || jump_height > 0) {
          y -= yVelocity;
        }

        if (jump_height <= 0) {
          if (yVelocity > -16) {
            yVelocity -= jumpSpeed;
          } else {
            yVelocity = 0;
            jumping = false;
          }
        } else {
          jump_height -= yVelocity;
        }
      } else if (jumping && !canJumpUp) {
        jumping = false;
      }

      // Jump
      if (random(0, jumpChance) == 0 && jumpChance != 0) {
        if (inLiquid) {
          y -= 16;
        } else if (!canFall && canJump && !jumping) {
          yVelocity = 16;
          jump_height = jumpHeight;
          jumping = true;
        }
      }

      // Reset animation sequence
      if (walking_animation_sequence > ANIMATION_SPEED * 4) {
        walking_animation_sequence = 0;
      }

      // Update bullets
      for (uint32_t i = 0; i < bullets.size(); i++) {
        bullets.at(i).update();
        if (bullets.at(i).getContact(newMap) &&
            bullets.at(i).getContactFrameCounter() == 10) {
          bullets.erase(bullets.begin() + i);
        }
      }
    }

    canFall = true;
    bool smoothFall = false;
    // Falling (calculated seperately to ensure collision accurate)
    for (uint32_t i = 0; i < newMap->mapTiles.size(); i++) {
      if (collisionAny(x - width * 2, x + width * 2,
                       newMap->mapTiles.at(i).getX(),
                       newMap->mapTiles.at(i).getX() + 64, y - height * 2,
                       y + height * 2, newMap->mapTiles.at(i).getY(),
                       newMap->mapTiles.at(i).getY() + 64)) {
        if (newMap->mapTiles.at(i).containsAttribute(solid) ||
            newMap->mapTiles.at(i).containsAttribute(climb)) {
          if (collisionAny(x, x + 96, newMap->mapTiles.at(i).getX(),
                           newMap->mapTiles.at(i).getX() + 64, y, y + 144,
                           newMap->mapTiles.at(i).getY(),
                           newMap->mapTiles.at(i).getY() + 64) &&
              collisionTop(y, y + 144, newMap->mapTiles.at(i).getY(),
                           newMap->mapTiles.at(i).getY() + 128)) {
            canFall = false;
            if (!collisionAny(x, x + 96, newMap->mapTiles.at(i).getX(),
                              newMap->mapTiles.at(i).getX() + 64, y, y + 129,
                              newMap->mapTiles.at(i).getY(),
                              newMap->mapTiles.at(i).getY() + 64) &&
                !collisionTop(y, y + 129, newMap->mapTiles.at(i).getY(),
                              newMap->mapTiles.at(i).getY() + 129)) {
              smoothFall = true;
            }
          }
        }
      }
    }

    // Fall
    if (canFall && !jumping) {
      y += 16;
    }
    // Smooth falling
    if (smoothFall) {
      y += 1;
    }
  }
}
