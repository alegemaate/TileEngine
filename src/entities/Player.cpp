#include "Player.h"

#include "../util/KeyListener.h"
#include "../util/Logger.h"

Player::Player() {
  x = 128;
  y = 128;

  deathcount = 0;

  canFall = false;
  jumping = false;
  jumping_animation_done = true;
  dead = false;
  shooting = false;

  shooting_animation_sequence = 0;
  walking_animation_sequence = 0;
  jumping_animation_sequence = 0;

  characterDir = 0;
  jump_height = 0;
  yVelocity = 0;

  upKey = 0;
  downKey = 0;
  leftKey = 0;
  rightKey = 0;
  jumpKey = 0;
  sprintSpeed = 8;

  newMap = new TileMap("blank");
}

// 0-3 left, 4-7 right, 8-11 up
void Player::load_images(int newType) {
  if (newType == 1) {
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
  } else if (newType == 2) {
  }

  projectileSprites[0][0] = al_load_bitmap("images/laser.png");
  projectileSprites[0][1] = al_load_bitmap("images/laser.png");
  projectileSprites[0][2] = al_load_bitmap("images/laser_hit.png");
}

// Load sounds
void Player::load_sounds() {
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
  if (!(win = al_load_sample("sounds/get_bonus.wav"))) {
    Logger::fatal(
        "Cannot find sound sounds/getBonus.wav \n Please check your files and "
        "try again");
  }
}

// Set keys
void Player::set_keys(int up,
                      int down,
                      int left,
                      int right,
                      int jump,
                      int shoot) {
  // Differentiate controls
  upKey = up;
  downKey = down;
  leftKey = left;
  rightKey = right;
  jumpKey = jump;
  shootKey = shoot;
}

int Player::getDeathcount() {
  return deathcount;
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
vector<Projectile> Player::getBullets() {
  return bullets;
}

// Set finished
void Player::setFinished(bool newFinished) {
  finished = newFinished;
}
// Set deathcount
void Player::setDeathcount(int newDeathcount) {
  deathcount = newDeathcount;
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
      if (characterDir == LEFT) {
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
      if (characterDir == LEFT) {
        player_images[17].draw(x - tile_map_x, y - tile_map_y);
      }
      // Right
      else {
        player_images[23].draw(x - tile_map_x, y - tile_map_y);
      }
    }
    // Walking
    else {
      player_images[characterDir + walking_animation_sequence / ANIMATION_SPEED]
          .draw(x - tile_map_x, y - tile_map_y);
    }
  }
  // Shooting
  else {
    // Left
    if (characterDir == LEFT) {
      player_images[24].draw(x - tile_map_x, y - tile_map_y);
    }
    // Right
    else {
      player_images[25].draw(x - tile_map_x, y - tile_map_y);
    }
  }

  // Draw bullets
  for (uint32_t i = 0; i < bullets.size(); i++) {
    bullets.at(i).draw(tile_map_x, tile_map_y);
  }
}

// Spawn
void Player::spawncommand(TileMap* fullMap) {
  for (uint32_t i = 0; i < fullMap->mapTiles.size(); i++) {
    if (fullMap->mapTiles.at(i).containsAttribute(spawn)) {
      x = fullMap->mapTiles.at(i).getX();
      y = fullMap->mapTiles.at(i).getY();
    }
  }
}

// Movement
void Player::update(TileMap* fullMap) {
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

  newMap->mapTiles.clear();
  newMap->width = fullMap->width;
  newMap->height = fullMap->height;

  // Add close elements
  for (uint32_t i = 0; i < fullMap->mapTiles.size(); i++) {
    if (collisionAny(
            x - 140, x + 140, fullMap->mapTiles.at(i).getX(),
            fullMap->mapTiles.at(i).getX() + fullMap->mapTiles.at(i).getWidth(),
            y - 140, y + 140, fullMap->mapTiles.at(i).getY(),
            fullMap->mapTiles.at(i).getY() +
                fullMap->mapTiles.at(i).getHeight())) {
      newMap->mapTiles.push_back(fullMap->mapTiles.at(i));
    }
  }

  // Check for collision
  for (uint32_t i = 0; i < newMap->mapTiles.size(); i++) {
    // Check moving LEFT
    if (newMap->mapTiles.at(i).containsAttribute(solid)) {
      if (collisionAny(
              x - 4, x + 60, newMap->mapTiles.at(i).getX(),
              newMap->mapTiles.at(i).getX() + newMap->mapTiles.at(i).getWidth(),
              y, y + 128, newMap->mapTiles.at(i).getY(),
              newMap->mapTiles.at(i).getY() +
                  newMap->mapTiles.at(i).getHeight()) &&
          collisionLeft(x - 4, x + 60, newMap->mapTiles.at(i).getX(),
                        newMap->mapTiles.at(i).getX() + 64)) {
        canMoveLeft = false;
      }
    }

    // Check moving RIGHT
    if (newMap->mapTiles.at(i).containsAttribute(solid)) {
      if (collisionAny(
              x + 4, x + 68, newMap->mapTiles.at(i).getX(),
              newMap->mapTiles.at(i).getX() + newMap->mapTiles.at(i).getWidth(),
              y, y + 128, newMap->mapTiles.at(i).getY(),
              newMap->mapTiles.at(i).getY() +
                  newMap->mapTiles.at(i).getHeight()) &&
          collisionRight(x + 4, x + 68, newMap->mapTiles.at(i).getX(),
                         newMap->mapTiles.at(i).getX() + 64)) {
        canMoveRight = false;
      }
    }
    // Check 2 for climbing up
    if (newMap->mapTiles.at(i).containsAttribute(climb)) {
      if (collisionAny(
              x + 16, x + 48, newMap->mapTiles.at(i).getX(),
              newMap->mapTiles.at(i).getX() + newMap->mapTiles.at(i).getWidth(),
              y, y + 128, newMap->mapTiles.at(i).getY(),
              newMap->mapTiles.at(i).getY() +
                  newMap->mapTiles.at(i).getHeight())) {
        canClimbUp2 = true;
      }
    }
    // Check 2 for climbing down
    if (newMap->mapTiles.at(i).containsAttribute(climb)) {
      if (collisionAny(
              x + 16, x + 48, newMap->mapTiles.at(i).getX(),
              newMap->mapTiles.at(i).getX() + newMap->mapTiles.at(i).getWidth(),
              y, y + 144, newMap->mapTiles.at(i).getY(),
              newMap->mapTiles.at(i).getY() +
                  newMap->mapTiles.at(i).getHeight())) {
        canClimbDown2 = true;
      }
    }
    // Check 1 for climbing up
    if (newMap->mapTiles.at(i).containsAttribute(solid)) {
      if (collisionAny(
              x + 16, x + 48, newMap->mapTiles.at(i).getX(),
              newMap->mapTiles.at(i).getX() + newMap->mapTiles.at(i).getWidth(),
              y - 16, y, newMap->mapTiles.at(i).getY(),
              newMap->mapTiles.at(i).getY() +
                  newMap->mapTiles.at(i).getHeight())) {
        canClimbUp = false;
      }
    }
    // Check 2 for climbing down
    if (newMap->mapTiles.at(i).containsAttribute(solid)) {
      if (collisionAny(
              x + 16, x + 48, newMap->mapTiles.at(i).getX(),
              newMap->mapTiles.at(i).getX() + newMap->mapTiles.at(i).getWidth(),
              y, y + 144, newMap->mapTiles.at(i).getY(),
              newMap->mapTiles.at(i).getY() +
                  newMap->mapTiles.at(i).getHeight())) {
        canClimbDown = false;
      }
    }
    // Check jump
    if (!(newMap->mapTiles.at(i).containsAttribute(gas)) &&
        newMap->mapTiles.at(i).containsAttribute(liquid)) {
      if (collisionAny(
              x + 16, x + 48, newMap->mapTiles.at(i).getX(),
              newMap->mapTiles.at(i).getX() + newMap->mapTiles.at(i).getWidth(),
              y, y + 128, newMap->mapTiles.at(i).getY(),
              newMap->mapTiles.at(i).getY() +
                  newMap->mapTiles.at(i).getHeight())) {
        canJump = false;
      }
    }
    // Can if you will not hit your head
    if (!(newMap->mapTiles.at(i).containsAttribute(gas)) ||
        newMap->mapTiles.at(i).containsAttribute(liquid)) {
      if (collisionAny(
              x + 16, x + 48, newMap->mapTiles.at(i).getX(),
              newMap->mapTiles.at(i).getX() + newMap->mapTiles.at(i).getWidth(),
              newMap->mapTiles.at(i).getY(), newMap->mapTiles.at(i).getY() + 64,
              y, y + 144) &&
          collisionTop(newMap->mapTiles.at(i).getY(),
                       newMap->mapTiles.at(i).getY() + 128, y + yVelocity,
                       y + 144 + yVelocity)) {
        canJumpUp = false;
      }
    }

    // If you are swimming
    if (newMap->mapTiles.at(i).containsAttribute(liquid)) {
      if (collisionAny(
              x + 16, x + 48, newMap->mapTiles.at(i).getX(),
              newMap->mapTiles.at(i).getX() + newMap->mapTiles.at(i).getWidth(),
              y - 16, y + 128, newMap->mapTiles.at(i).getY(),
              newMap->mapTiles.at(i).getY() + 64)) {
        inLiquid = true;
      }
    }
  }

  // Check for points and dangers
  for (uint32_t i = 0; i < fullMap->mapTiles.size(); i++) {
    // Get point
    if (fullMap->mapTiles.at(i).containsAttribute(item)) {
      if (collisionAny(x + 16, x + 48, fullMap->mapTiles.at(i).getX(),
                       fullMap->mapTiles.at(i).getX() +
                           fullMap->mapTiles.at(i).getWidth(),
                       y + 32, y + 128, fullMap->mapTiles.at(i).getY(),
                       fullMap->mapTiles.at(i).getY() +
                           fullMap->mapTiles.at(i).getHeight())) {
        al_play_sample(getItem, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
        fullMap->mapTiles.at(i).setType(0);
      }
    }
  }

  for (uint32_t i = 0; i < newMap->mapTiles.size(); i++) {
    // Die
    if (newMap->mapTiles.at(i).containsAttribute(finish)) {
      if (collisionAny(
              x + 16, x + 48, newMap->mapTiles.at(i).getX(),
              newMap->mapTiles.at(i).getX() + newMap->mapTiles.at(i).getWidth(),
              y + 32, y + 128, newMap->mapTiles.at(i).getY(),
              newMap->mapTiles.at(i).getY() +
                  newMap->mapTiles.at(i).getHeight())) {
        al_play_sample(win, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
        finished = true;
      }
    }
    if (newMap->mapTiles.at(i).containsAttribute(harmful)) {
      if (collisionAny(
              x + 16, x + 48, newMap->mapTiles.at(i).getX(),
              newMap->mapTiles.at(i).getX() + newMap->mapTiles.at(i).getWidth(),
              y + 32, y + 128, newMap->mapTiles.at(i).getY(),
              newMap->mapTiles.at(i).getY() +
                  newMap->mapTiles.at(i).getHeight())) {
        al_play_sample(die, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
        dead = true;
        deathcount++;
      }
    }
    if (x > newMap->width * 64 || x < 0 || y > newMap->height * 64) {
      x = 0;
      y = 0;
      al_play_sample(die, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
      dead = true;
      deathcount++;
    }
  }

  // Move up
  if (KeyListener::key[upKey]) {
    characterDir = UP;
    if (canClimbUp2 && canClimbUp) {
      y -= 16;
      walking_animation_sequence++;
    }
  }

  // Move down
  if (KeyListener::key[downKey]) {
    characterDir = UP;
    if (canClimbDown2 && canClimbDown) {
      y += 16;
      walking_animation_sequence++;
    }
  }

  // Move right
  if ((KeyListener::key[rightKey]) && jumping_animation_done &&
      x < newMap->width * 64) {
    if (!sprinting) {
      sprinting = true;
      sprintSpeed = 8;
    } else if (sprintSpeed < 16 && !jumping) {
      sprintSpeed *= 1.05;
    }
    characterDir = RIGHT;
    if (canMoveRight) {
      x += sprintSpeed;
      walking_animation_sequence++;
      if (walking_animation_sequence == ANIMATION_SPEED && !canFall &&
          !jumping) {
        if (random(0, 1)) {
          al_play_sample(walk1, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
        } else {
          al_play_sample(walk2, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
        }
      }
    }
  }

  // Move left
  if ((KeyListener::key[leftKey]) && jumping_animation_done && x > 0) {
    if (!sprinting) {
      sprinting = true;
      sprintSpeed = 8;
    } else if (sprintSpeed < 16 && !jumping) {
      sprintSpeed *= 1.05;
    }
    characterDir = LEFT;
    if (canMoveLeft) {
      x -= sprintSpeed;
      walking_animation_sequence++;
      if (walking_animation_sequence == ANIMATION_SPEED && !canFall &&
          !jumping) {
        if (random(0, 1)) {
          al_play_sample(walk1, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
        } else {
          al_play_sample(walk2, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
        }
      }
    }
  }

  // Stop sprinting
  if (!KeyListener::key[leftKey] && !KeyListener::key[rightKey]) {
    sprinting = false;
    sprintSpeed = 0;
  }

  canFall = true;
  bool smoothFall = false;

  // Falling (calculated seperately to ensure collision accurate)
  for (uint32_t i = 0; i < newMap->mapTiles.size(); i++) {
    if (newMap->mapTiles.at(i).containsAttribute(solid) ||
        newMap->mapTiles.at(i).containsAttribute(climb)) {
      if (collisionAny(
              x + 16, x + 48, newMap->mapTiles.at(i).getX(),
              newMap->mapTiles.at(i).getX() + newMap->mapTiles.at(i).getWidth(),
              y, y + 144, newMap->mapTiles.at(i).getY(),
              newMap->mapTiles.at(i).getY() +
                  newMap->mapTiles.at(i).getHeight()) &&
          collisionTop(y, y + 144, newMap->mapTiles.at(i).getY(),
                       newMap->mapTiles.at(i).getY() +
                           newMap->mapTiles.at(i).getHeight())) {
        canFall = false;
        if (!collisionAny(x + 16, x + 48, newMap->mapTiles.at(i).getX(),
                          newMap->mapTiles.at(i).getX() +
                              newMap->mapTiles.at(i).getWidth(),
                          y, y + 129, newMap->mapTiles.at(i).getY(),
                          newMap->mapTiles.at(i).getY() +
                              newMap->mapTiles.at(i).getHeight()) &&
            !collisionTop(y, y + 129, newMap->mapTiles.at(i).getY(),
                          newMap->mapTiles.at(i).getY() +
                              newMap->mapTiles.at(i).getHeight())) {
          smoothFall = true;
        }
      }
    }
  }

  // Fall
  if (canFall && !jumping) {
    if (inLiquid) {
      y += 2;
    } else {
      if (KeyListener::key[downKey]) {
        y += 32;
      } else {
        y += 16;
      }
    }
  }
  // Smooth falling
  if (smoothFall && !jumping) {
    y += 1;
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
  if (KeyListener::key[jumpKey]) {
    if (inLiquid) {
      y -= 4;
    } else if (!canFall && canJump && !jumping && jumping_animation_done) {
      yVelocity = 16;
      al_play_sample(jump, 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, 0);
      jumping_animation_done = false;
      jump_height = 0;
    }
  }

  // Increase jump animation sequence
  if (!jumping_animation_done) {
    jumping_animation_sequence++;
    if (KeyListener::key[jumpKey]) {
      if (jumping_animation_sequence < 7) {
        jump_height = 32;
      } else if (jumping_animation_sequence < 11) {
        jump_height = 86;
      } else if (jumping_animation_sequence < 12) {
        jump_height = 160;
      }
    }
    if (jumping_animation_sequence > 10) {
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
  else if (!KeyListener::key[rightKey] && !KeyListener::key[leftKey]) {
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
  if (KeyListener::key[shootKey] && !shooting) {
    if (characterDir == RIGHT) {
      Projectile newBullet(0, x + 57, y + 70, 100);
      bullets.push_back(newBullet);
    } else if (characterDir == LEFT) {
      Projectile newBullet(0, x - 64, y + 70, -100);
      bullets.push_back(newBullet);
    }
    shooting = true;
  }

  // Update bullets
  for (uint32_t i = 0; i < bullets.size(); i++) {
    bullets.at(i).update();
    if (bullets.at(i).getContact(newMap) &&
        bullets.at(i).getContactFrameCounter() == 10) {
      bullets.erase(bullets.begin() + i);
    }
  }

  // Prevents getting stuck through an animation
  if (!KeyListener::key[leftKey] && !KeyListener::key[rightKey] &&
      !KeyListener::key[upKey] && !KeyListener::key[downKey]) {
    walking_animation_sequence = 0;
  }

  idle_timer++;
  if (idle_timer > 100)
    idle_timer = 0;
}

Player::~Player() {
  bullets.clear();

  al_destroy_sample(walk1);
  al_destroy_sample(walk2);
  al_destroy_sample(jump);
  al_destroy_sample(win);
  al_destroy_sample(die);
  al_destroy_sample(getItem);
  al_destroy_sample(getBonus);
}
