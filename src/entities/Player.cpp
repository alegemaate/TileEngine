#include "Player.h"

#include "../lib/input/KeyListener.h"
#include "../lib/util/Logger.h"

Player::Player(KeyListener& keyboardListener)
    : keyboardListener(keyboardListener) {
  // 0-3 left, 4-7 right, 8-11 up
  player_images[0] = Bitmap("images/character/left_1.png");
  player_images[1] = Bitmap("images/character/left_2.png");
  player_images[2] = Bitmap("images/character/left_3.png");
  player_images[3] = Bitmap("images/character/left_4.png");

  player_images[4] = Bitmap("images/character/right_1.png");
  player_images[5] = Bitmap("images/character/right_2.png");
  player_images[6] = Bitmap("images/character/right_3.png");
  player_images[7] = Bitmap("images/character/right_4.png");

  player_images[8] = Bitmap("images/character/up_1.png");
  player_images[9] = Bitmap("images/character/up_2.png");
  player_images[10] = Bitmap("images/character/up_3.png");
  player_images[11] = Bitmap("images/character/up_4.png");

  player_images[12] = Bitmap("images/character/jump_left_1.png");
  player_images[13] = Bitmap("images/character/jump_left_2.png");
  player_images[14] = Bitmap("images/character/jump_left_3.png");
  player_images[15] = Bitmap("images/character/jump_left_4.png");
  player_images[16] = Bitmap("images/character/jump_left_5.png");
  player_images[17] = Bitmap("images/character/jump_left_6.png");

  player_images[18] = Bitmap("images/character/jump_right_1.png");
  player_images[19] = Bitmap("images/character/jump_right_2.png");
  player_images[20] = Bitmap("images/character/jump_right_3.png");
  player_images[21] = Bitmap("images/character/jump_right_4.png");
  player_images[22] = Bitmap("images/character/jump_right_5.png");
  player_images[23] = Bitmap("images/character/jump_right_6.png");

  player_images[24] = Bitmap("images/character/shoot_left.png");
  player_images[25] = Bitmap("images/character/shoot_right.png");

  walk1 = Sound("sounds/keen_walk_1.wav");
  walk2 = Sound("sounds/keen_walk_2.wav");
  jump = Sound("sounds/keen_jump.wav");
  die = Sound("sounds/keen_die.wav");
  getItem = Sound("sounds/get_item.wav");
  getBonus = Sound("sounds/get_bonus.wav");
  win = Sound("sounds/get_bonus.wav");
}

// Set keys
void Player::setKeys(Key up,
                     Key down,
                     Key left,
                     Key right,
                     Key jump,
                     Key shoot) {
  // Differentiate controls
  upKey = up;
  downKey = down;
  leftKey = left;
  rightKey = right;
  jumpKey = jump;
  shootKey = shoot;
}

// Return X
int Player::getX() {
  return x;
}

// Return Y
int Player::getY() {
  return y;
}

// Get finished
bool Player::getFinished() {
  return finished;
}

// Dead?
bool Player::getDead() {
  if (dead) {
    x = 128;
    y = 128;
  }
  return dead;
}

// Get bullets
std::vector<Projectile> Player::getBullets() {
  return bullets;
}

// Set finished
void Player::setFinished(bool newFinished) {
  finished = newFinished;
}

// Set dead
void Player::setDead(bool newDead) {
  dead = newDead;
}

// Draw character
void Player::draw(int tile_map_x, int tile_map_y) {
  if (!shooting) {
    // Jumping
    if (!jumping_animation_done) {
      // Left
      if (characterDir == CharacterDirection::LEFT) {
        player_images[12 + jumping_animation_sequence / 2].draw(x - tile_map_x,
                                                                y - tile_map_y);
      }
      // Right
      else {
        player_images[18 + jumping_animation_sequence / 2].draw(x - tile_map_x,
                                                                y - tile_map_y);
      }
    }
    // Falling
    else if (jumping || canFall) {
      // Left
      if (characterDir == CharacterDirection::LEFT) {
        player_images[17].draw(x - tile_map_x, y - tile_map_y);
      }
      // Right
      else {
        player_images[23].draw(x - tile_map_x, y - tile_map_y);
      }
    }
    // Walking
    else {
      player_images[static_cast<int>(characterDir) +
                    walking_animation_sequence / ANIMATION_SPEED]
          .draw(x - tile_map_x, y - tile_map_y);
    }
  }
  // Shooting
  else {
    // Left
    if (characterDir == CharacterDirection::LEFT) {
      player_images[24].draw(x - tile_map_x, y - tile_map_y);
    }
    // Right
    else {
      player_images[25].draw(x - tile_map_x, y - tile_map_y);
    }
  }

  // Draw bullets
  for (auto& bullet : bullets) {
    bullet.draw(tile_map_x, tile_map_y);
  }
}

// Spawn
void Player::spawncommand(TileMap* fullMap) {
  for (auto& tile : fullMap->mapTiles) {
    if (tile->hasAttribute(TileAttribute::SPAWN)) {
      x = tile->getX();
      y = tile->getY();
    }
  }
}

// Movement
void Player::update(TileMap* fullMap, double delta) {
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
    // Check moving LEFT
    if (tile->hasAttribute(TileAttribute::SOLID)) {
      BoundingBox bounds{x - 4, y, 4, 128};
      if (tile->getBounds().collides(bounds)) {
        canMoveLeft = false;
      }
    }

    // Check moving RIGHT
    if (tile->hasAttribute(TileAttribute::SOLID)) {
      BoundingBox bounds{x + 64, y, 4, 128};
      if (tile->getBounds().collides(bounds)) {
        canMoveRight = false;
      }
    }

    // Check 2 for climbing up
    if (tile->hasAttribute(TileAttribute::CLIMB)) {
      BoundingBox bounds{x + 16, y, 32, 128};
      if (tile->getBounds().collides(bounds)) {
        canClimbUp2 = true;
      }
    }

    // Check 2 for climbing down
    if (tile->hasAttribute(TileAttribute::CLIMB)) {
      BoundingBox bounds{x + 16, y, 32, 144};
      if (tile->getBounds().collides(bounds)) {
        canClimbDown2 = true;
      }
    }

    // Check 1 for climbing up
    if (tile->hasAttribute(TileAttribute::SOLID)) {
      BoundingBox bounds{x + 16, y - 16, 32, 16};
      if (tile->getBounds().collides(bounds)) {
        canClimbUp = false;
      }
    }

    // Check 2 for climbing down
    if (tile->hasAttribute(TileAttribute::SOLID)) {
      BoundingBox bounds{x + 16, y, 32, 144};
      if (tile->getBounds().collides(bounds)) {
        canClimbDown = false;
      }
    }

    // Check jump
    if (!(tile->hasAttribute(TileAttribute::GAS)) &&
        tile->hasAttribute(TileAttribute::LIQUID)) {
      BoundingBox bounds{x + 16, y, 32, 128};
      if (tile->getBounds().collides(bounds)) {
        canJump = false;
      }
    }

    // If you will not hit your head
    if (!(tile->hasAttribute(TileAttribute::GAS)) ||
        tile->hasAttribute(TileAttribute::LIQUID)) {
      BoundingBox bounds{x + 16, y, 32, 144};
      if (tile->getBounds().collides(bounds) &&
          collisionTop(tile->getY(), tile->getY() + 128, y + yVelocity,
                       y + 144 + yVelocity)) {
        canJumpUp = false;
      }
    }

    // General
    BoundingBox bounds{x + 16, y + 32, 32, 96};
    if (tile->getBounds().collides(bounds)) {
      // Get point
      if (tile->hasAttribute(TileAttribute::ITEM)) {
        getItem.play();
        tile->setType(fullMap->findTileType(0));
      }

      // Die
      if (tile->hasAttribute(TileAttribute::FINISH)) {
        win.play();
        finished = true;
      }

      // Die
      if (tile->hasAttribute(TileAttribute::HARMFUL)) {
        die.play();
        dead = true;
      }

      // If you are swimming
      if (tile->hasAttribute(TileAttribute::LIQUID)) {
        inLiquid = true;
      }
    }

    if (x > fullMap->width * 64 || x < 0 || y > fullMap->height * 64) {
      x = 0;
      y = 0;
      die.play();
      dead = true;
    }
  }

  // Move up
  if (keyboardListener.isDown(upKey)) {
    characterDir = CharacterDirection::UP;
    if (canClimbUp2 && canClimbUp) {
      y -= 16;
      walking_animation_sequence++;
    }
  }

  // Move down
  if (keyboardListener.isDown(downKey)) {
    characterDir = CharacterDirection::UP;
    if (canClimbDown2 && canClimbDown) {
      y += 16;
      walking_animation_sequence++;
    }
  }

  // Sprint speed
  if ((keyboardListener.isDown(rightKey) || keyboardListener.isDown(leftKey)) &&
      jumping_animation_done) {
    if (!sprinting) {
      sprinting = true;
      sprintSpeed = 8;
    } else if (sprintSpeed < 16) {
      sprintSpeed *= 1.05;
    }
  }

  // Move right
  if (keyboardListener.isDown(rightKey) && jumping_animation_done) {
    characterDir = CharacterDirection::RIGHT;

    walking_animation_sequence++;
    if (walking_animation_sequence == ANIMATION_SPEED && !canFall && !jumping) {
      if (random(0, 1)) {
        walk1.play();
      } else {
        walk2.play();
      }
    }

    if (canMoveRight) {
      x += sprintSpeed;
    }
  }

  // Move left
  if (keyboardListener.isDown(leftKey) && jumping_animation_done) {
    characterDir = CharacterDirection::LEFT;

    walking_animation_sequence++;
    if (walking_animation_sequence == ANIMATION_SPEED && !canFall && !jumping) {
      if (random(0, 1)) {
        walk1.play();
      } else {
        walk2.play();
      }
    }

    if (canMoveLeft) {
      x -= sprintSpeed;
    }
  }

  // Stop sprinting
  if (!keyboardListener.isDown(leftKey) && !keyboardListener.isDown(rightKey)) {
    sprinting = false;
    sprintSpeed = 0;
  }

  canFall = true;

  // Falling (calculated seperately to ensure collision accurate)
  for (auto& tile : fullMap->mapTiles) {
    if (tile->hasAttribute(TileAttribute::SOLID) ||
        tile->hasAttribute(TileAttribute::CLIMB)) {
      BoundingBox bounds{x + 16, y + 128, 32, -yVelocity + 1};
      if (tile->getBounds().collides(bounds)) {
        canFall = false;
        y = tile->getY() - 128;
      }
    }
  }

  // Fall
  if (canFall && !jumping) {
    if (inLiquid) {
      y += 2;
    } else {
      if (keyboardListener.isDown(downKey)) {
        y += 32;
      } else {
        y += 16;
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
        yVelocity -= 2;
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
  if (keyboardListener.isDown(jumpKey)) {
    if (inLiquid) {
      y -= 4;
    } else if (!canFall && canJump && !jumping && jumping_animation_done) {
      jumping_animation_done = false;
      jump_height = 0;
    }
  }

  // Increase jump animation sequence
  if (!jumping_animation_done) {
    jumping_animation_sequence++;

    // Either we released the jump key or we've reached the end of the sequence
    if (!keyboardListener.isDown(jumpKey) || jumping_animation_sequence >= 12) {
      jump.play();
      yVelocity = 16;
      jump_height = jumping_animation_sequence * 14;
      jumping_animation_sequence = 0;
      jumping_animation_done = true;
      jumping = true;
    }
  }

  // Reset animation sequence
  if (walking_animation_sequence >= ANIMATION_SPEED * 4) {
    walking_animation_sequence = 0;
  }

  // Prevents getting stuck through an animation
  else if (!keyboardListener.isDown(rightKey) &&
           !keyboardListener.isDown(leftKey)) {
    walking_animation_sequence = 0;
  }

  // Not shooting anymore
  if (shooting) {
    shooting_animation_sequence++;
    if (shooting_animation_sequence > 9) {
      shooting = false;
      shooting_animation_sequence = 0;
    }
  }

  // Shoot
  if (keyboardListener.isDown(shootKey) && !shooting) {
    if (characterDir == CharacterDirection::RIGHT) {
      Projectile newBullet(0, x + 57, y + 70, 100);
      bullets.push_back(newBullet);
    } else if (characterDir == CharacterDirection::LEFT) {
      Projectile newBullet(0, x - 64, y + 70, -100);
      bullets.push_back(newBullet);
    }
    shooting = true;
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

  // Prevents getting stuck through an animation
  if (!keyboardListener.isDown(leftKey) && !keyboardListener.isDown(rightKey) &&
      !keyboardListener.isDown(upKey) && !keyboardListener.isDown(downKey)) {
    walking_animation_sequence = 0;
  }
}
