#include "Enemy.h"

#include "../lib/display/DisplayMode.h"
#include "../lib/util/Logger.h"

Enemy::Enemy(int x, int y, EnemyType type) : x(x), y(y), type(type) {
  if (type == EnemyType::VORTICON) {
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
  } else if (type == EnemyType::ROBOT) {
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
  } else if (type == EnemyType::DANNY) {
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

  walk1 = Sound("sounds/keen_walk_1.wav");
  walk2 = Sound("sounds/keen_walk_2.wav");
  jump = Sound("sounds/keen_jump.wav");
  die = Sound("sounds/keen_die.wav");
  getItem = Sound("sounds/get_item.wav");
  getBonus = Sound("sounds/get_bonus.wav");
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
      if (characterDir == CharacterDirection::LEFT) {
        enemy_images[8].draw(x - tile_map_x, y - tile_map_y);
      } else {
        enemy_images[9].draw(x - tile_map_x, y - tile_map_y);
      }
    } else {
      enemy_images[static_cast<int>(characterDir) +
                   walking_animation_sequence / ANIMATION_SPEED]
          .draw(x - tile_map_x, y - tile_map_y);
    }

    // Draw bullets
    for (auto& bullet : bullets) {
      bullet.draw(tile_map_x, tile_map_y);
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

      // Check for collision
      for (auto& tile : fullMap->mapTiles) {
        if (tile->hasAttribute(TileAttribute::SOLID)) {
          if (collisionAny(x - speed, x + width - speed, tile->getX(),
                           tile->getX() + 64, y, y + height, tile->getY(),
                           tile->getY() + 64) &&
              collisionLeft(x - speed, x + width - speed, tile->getX(),
                            tile->getX() + 64)) {
            canMoveLeft = false;
          }
        }
        if (tile->hasAttribute(TileAttribute::SOLID)) {
          if (collisionAny(x + speed, x + width + speed, tile->getX(),
                           tile->getX() + 64, y, y + height, tile->getY(),
                           tile->getY() + 64) &&
              collisionRight(x + speed, x + width + speed, tile->getX(),
                             tile->getX() + 64)) {
            canMoveRight = false;
          }
        }
        if (tile->hasAttribute(TileAttribute::CLIMB)) {
          if (collisionAny(x, x + width, tile->getX(), tile->getX() + 64, y,
                           y + height, tile->getY(), tile->getY() + 64)) {
            canClimbUp2 = true;
          }
        }
        if (tile->hasAttribute(TileAttribute::CLIMB)) {
          if (collisionAny(x, x + width, tile->getX(), tile->getX() + 64, y,
                           y + height + 16, tile->getY(), tile->getY() + 64)) {
            canClimbDown2 = true;
          }
        }
        if (tile->hasAttribute(TileAttribute::SOLID)) {
          if (collisionAny(x, x + width, tile->getX(), tile->getX() + 64,
                           y - 16, y, tile->getY(), tile->getY() + 64)) {
            canClimbUp = false;
          }
        }
        if (tile->hasAttribute(TileAttribute::SOLID)) {
          if (collisionAny(x, x + width, tile->getX(), tile->getX() + 64, y,
                           y + height + 16, tile->getY(), tile->getY() + 64)) {
            canClimbDown = false;
          }
        }
        if (!tile->hasAttribute(TileAttribute::GAS) &&
            !tile->hasAttribute(TileAttribute::LIQUID)) {
          if (collisionAny(x, x + width, tile->getX(), tile->getX() + 64, y,
                           y + height, tile->getY(), tile->getY() + 64)) {
            canJump = false;
          }
        }
        if (!tile->hasAttribute(TileAttribute::GAS) &&
            !tile->hasAttribute(TileAttribute::LIQUID)) {
          if (collisionAny(x, x + width, tile->getX(), tile->getX() + 64,
                           tile->getY(), tile->getY() + 64, y, y + 144) &&
              collisionTop(tile->getY(), tile->getY() + 128, y + yVelocity,
                           y + height + 16 + yVelocity)) {
            canJumpUp = false;
          }
        }
        if (!tile->hasAttribute(TileAttribute::LIQUID)) {
          if (collisionAny(x, x + width, tile->getX(), tile->getX() + 64,
                           y - 16, y + height, tile->getY(),
                           tile->getY() + 64)) {
            inLiquid = true;
          }
        }

        if (collisionAny(x - width * 2, x + width * 2, tile->getX(),
                         tile->getX() + 64, y - height * 2, y + height * 2,
                         tile->getY(), tile->getY() + 64)) {
          // Get point
          // if (tile->getType() == tile_lollypop) {
          //   if (collisionAny(x, x + width, tile->getX(),
          //                    tile->getX() + 64, y - 16,
          //                    y + height, tile->getY(),
          //                    tile->getY() + 128)) {
          //     getBonus.play();
          //     tile->setType(0);
          //   }
          // }

          // Die
          if (tile->hasAttribute(TileAttribute::HARMFUL)) {
            if (collisionAny(x, x + width, tile->getX(), tile->getX() + 64,
                             y - 16, y + height, tile->getY(),
                             tile->getY() + 128)) {
              dead = true;
            }
          }
        }
      }

      // Check collision with bullets
      for (auto& bullet : newPlayer->getBullets()) {
        if (collisionAny(x, x + width, bullet.getX(), bullet.getX() + 64, y,
                         y + height, bullet.getY(), bullet.getY() + 64)) {
          dead = true;
        }
      }

      // AI set direction of walking
      if (walking_counter == 0) {
        if (random(0, walkChance) == 0 && walkChance != 0) {
          if (random(0, 1) == 0) {
            characterDir = CharacterDirection::RIGHT;
          } else {
            characterDir = CharacterDirection::LEFT;
          }
          walking_counter = random(minSteps, maxSteps);
        }
      }
      if (walking_counter > 0) {
        walking_counter--;
      }

      // Move right
      if (characterDir == CharacterDirection::RIGHT && walking_counter > 0) {
        if (canMoveRight) {
          x += speed;
          walking_animation_sequence++;
          if (walking_animation_sequence % (ANIMATION_SPEED * 2) == 0 &&
              !canFall && !jumping) {
            if (random(0, 1)) {
              walk1.play();
            } else {
              walk2.play();
            }
          }
        }
      }

      // Move left
      if (characterDir == CharacterDirection::LEFT && walking_counter > 0) {
        if (canMoveLeft) {
          x -= speed;
          walking_animation_sequence++;
          if (walking_animation_sequence % (ANIMATION_SPEED * 2) == 0 &&
              !canFall && !jumping) {
            if (random(0, 1)) {
              walk1.play();
            } else {
              walk2.play();
            }
          }
        }
      }

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
      for (auto& bullet : bullets) {
        bullet.update();
      }

      for (auto it = std::begin(bullets); it != std::end(bullets);) {
        if ((*it).getContact(fullMap) && (*it).getContactFrameCounter() == 10) {
          it = bullets.erase(it);
        } else {
          ++it;
        }
      }
    }

    canFall = true;
    bool smoothFall = false;

    // Falling (calculated seperately to ensure collision accurate)
    for (auto& tile : fullMap->mapTiles) {
      if (collisionAny(x - width * 2, x + width * 2, tile->getX(),
                       tile->getX() + 64, y - height * 2, y + height * 2,
                       tile->getY(), tile->getY() + 64)) {
        if (tile->hasAttribute(TileAttribute::SOLID) ||
            tile->hasAttribute(TileAttribute::CLIMB)) {
          if (collisionAny(x, x + 96, tile->getX(), tile->getX() + 64, y,
                           y + 144, tile->getY(), tile->getY() + 64) &&
              collisionTop(y, y + 144, tile->getY(), tile->getY() + 128)) {
            canFall = false;
            if (!collisionAny(x, x + 96, tile->getX(), tile->getX() + 64, y,
                              y + 129, tile->getY(), tile->getY() + 64) &&
                !collisionTop(y, y + 129, tile->getY(), tile->getY() + 129)) {
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
