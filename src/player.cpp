#include "player.h"

player::player(){

  x = 128;
  y = 128;

  canFall = true;
  jumping = false;
  jumping_animation_done = true;
  dead = false;
  shooting = false;

  shooting_animation_sequence = 0;
  walking_animation_sequence = 0;
  jumping_animation_sequence = 0;

  jump_height = 0;
  characterDir = 0;
  yVelocity = 0;
}

// 0-3 left, 4-7 right, 8-11 up
void player::load_images(){
  player_images[0] = load_bitmap ( "images/character/left_1.png", NULL);
  player_images[1] = load_bitmap ( "images/character/left_2.png", NULL);
  player_images[2] = load_bitmap ( "images/character/left_3.png", NULL);
  player_images[3] = load_bitmap ( "images/character/left_4.png", NULL);

  player_images[4] = load_bitmap ( "images/character/right_1.png", NULL);
  player_images[5] = load_bitmap ( "images/character/right_2.png", NULL);
  player_images[6] = load_bitmap ( "images/character/right_3.png", NULL);
  player_images[7] = load_bitmap ( "images/character/right_4.png", NULL);

  player_images[8] = load_bitmap ( "images/character/up_1.png", NULL);
  player_images[9] = load_bitmap ( "images/character/up_2.png", NULL);
  player_images[10] = load_bitmap ( "images/character/up_3.png", NULL);
  player_images[11] = load_bitmap ( "images/character/up_4.png", NULL);

  player_images[12] = load_bitmap ( "images/character/jump_left_1.png", NULL);
  player_images[13] = load_bitmap ( "images/character/jump_left_2.png", NULL);
  player_images[14] = load_bitmap ( "images/character/jump_left_3.png", NULL);
  player_images[15] = load_bitmap ( "images/character/jump_left_4.png", NULL);
  player_images[16] = load_bitmap ( "images/character/jump_left_5.png", NULL);
  player_images[17] = load_bitmap ( "images/character/jump_left_6.png", NULL);

  player_images[18] = load_bitmap ( "images/character/jump_right_1.png", NULL);
  player_images[19] = load_bitmap ( "images/character/jump_right_2.png", NULL);
  player_images[20] = load_bitmap ( "images/character/jump_right_3.png", NULL);
  player_images[21] = load_bitmap ( "images/character/jump_right_4.png", NULL);
  player_images[22] = load_bitmap ( "images/character/jump_right_5.png", NULL);
  player_images[23] = load_bitmap ( "images/character/jump_right_6.png", NULL);

  player_images[24] = load_bitmap ( "images/character/shoot_left.png", NULL);
  player_images[25] = load_bitmap ( "images/character/shoot_right.png", NULL);

  projectileSprites[0][0] = load_bitmap("images/laser.png", NULL);
  projectileSprites[0][1] = load_bitmap("images/laser.png", NULL);
  projectileSprites[0][2] = load_bitmap("images/laser_hit.png", NULL);
}

// Load sounds
void player::load_sounds(){
  walk1 = load_sample("sounds/keen_walk_1.wav");
  walk2 = load_sample("sounds/keen_walk_2.wav");
  jump = load_sample("sounds/keen_jump.wav");
  die = load_sample("sounds/keen_die.wav");
  getItem = load_sample("sounds/get_item.wav");
  getBonus = load_sample("sounds/get_bonus.wav");
}

// Return X
int player::getX(){
  return x;
}

// Return Y
int player::getY(){
  return y;
}

// Dead?
bool player::getDead(){
  if(dead){
    x = 128;
    y = 128;
  }
  return dead;
}

//Get bullets
vector<projectile> player::getBullets(){
  return bullets;
}

//Set dead
void player::setDead(bool newDead){
  dead = newDead;
}

//Draw character
void player::draw(BITMAP* temp, int tile_map_x, int tile_map_y){
  if( !shooting){
    if(!jumping_animation_done){
      if(characterDir == LEFT){
        draw_sprite( temp, player_images[12 + jumping_animation_sequence/2], x - tile_map_x, y - tile_map_y);
      }
      else{
        draw_sprite( temp, player_images[18 + jumping_animation_sequence/2], x - tile_map_x, y - tile_map_y);
      }
    }
    else if(jumping || canFall){
      if(characterDir == LEFT){
        draw_sprite( temp, player_images[17], x - tile_map_x, y - tile_map_y);
      }
      else{
        draw_sprite( temp, player_images[23], x - tile_map_x, y - tile_map_y);
      }
    }
    else{
      draw_sprite( temp, player_images[characterDir + walking_animation_sequence/ANIMATION_SPEED], x - tile_map_x, y - tile_map_y);
    }
  }
  else{
    if(characterDir == LEFT){
      draw_sprite( temp, player_images[24], x - tile_map_x, y - tile_map_y);
    }
    else{
      draw_sprite( temp, player_images[25], x - tile_map_x, y - tile_map_y);
    }
  }

  //Draw bullets
  for(int i = 0; i < bullets.size(); i++){
    bullets.at(i).draw(temp,tile_map_x, tile_map_y);
  }
}

// Spawn
void player::spawncommand(tileMap *newMap){
  for(int i = 0; i < newMap -> mapTiles.size(); i++){
    if(newMap -> mapTiles.at(i).getType() == 199){
      x = newMap -> mapTiles.at(i).getX();
      y = newMap -> mapTiles.at(i).getY();
    }
  }
}

//Movement
void player::update(tileMap *newMap){
  //Collision stuff
  bool canMoveLeft = true;
  bool canMoveRight = true;
  bool canClimbUp = true;
  bool canClimbDown = true;
  bool canClimbUp2 = false;
  bool canClimbDown2 = false;
  bool canJump = true;
  bool canJumpUp = true;
  bool inLiquid = false;

  //Check for collision
  for(int i = 0; i < newMap -> mapTiles.size(); i++){
    if(collisionAny(x - 64*2, x + 64*2, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y - 128*2, y + 128*2, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
      if(newMap -> mapTiles.at(i).getAttribute() == solid){
        if(collisionAny(x - 4, x + 60, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64) &&
           collisionLeft(x - 4, x + 60, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64)){
          canMoveLeft = false;
        }
      }
      if(newMap -> mapTiles.at(i).getAttribute() == solid){
        if(collisionAny(x + 4, x + 68, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64) &&
           collisionRight(x + 4, x + 68, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64)){
          canMoveRight = false;
        }
      }
      if(newMap -> mapTiles.at(i).getAttribute() == climb){
        if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
          canClimbUp2 = true;
        }
      }
      if(newMap -> mapTiles.at(i).getAttribute() == climb){
        if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + 144, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
          canClimbDown2 = true;
        }
      }
      if(newMap -> mapTiles.at(i).getAttribute() == solid){
        if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y - 16, y, newMap -> mapTiles.at(i).getY(),newMap -> mapTiles.at(i).getY() + 64)){
          canClimbUp = false;
        }
      }
      if(newMap -> mapTiles.at(i).getAttribute() == solid){
        if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + 144, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
          canClimbDown = false;
        }
      }
      if(newMap -> mapTiles.at(i).getAttribute() != gas && newMap -> mapTiles.at(i).getAttribute() != liquid){
        if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
          canJump = false;
        }
      }
      if(newMap -> mapTiles.at(i).getAttribute() != gas && newMap -> mapTiles.at(i).getAttribute() != liquid){
        if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64, y, y + 144) &&
        collisionTop(newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 128, y + yVelocity, y + 144 + yVelocity)){
          canJumpUp = false;
        }
      }
      if(newMap -> mapTiles.at(i).getAttribute() == liquid){
        if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y-16, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
          inLiquid = true;
        }
      }
    }
  }

  //Check for points and dangers
  for(int i = 0; i < newMap -> mapTiles.size(); i++){
    //Get point
    if(newMap -> mapTiles.at(i).getType() == tile_lollypop){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y-16, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 128)){
        play_sample(getItem,255,125,1000,0);
        newMap -> mapTiles.at(i).setType(0);
      }
    }

    //Die
    if(newMap -> mapTiles.at(i).getAttribute() == harmful){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y-16, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 128)){
        play_sample(die,255,125,1000,0);
        dead = true;
      }
    }
    if(x > newMap -> width*64 || x < 0 || y > newMap -> height*64){
      play_sample(die,255,125,1000,0);
      dead = true;
    }
  }

  //Move up
  if(key[KEY_W]){
    characterDir = UP;
    if(canClimbUp2 && canClimbUp){
      y -= 16;
      walking_animation_sequence++;
    }
  }

  //Move down
  if(key[KEY_S]){
    characterDir = UP;
    if(canClimbDown2 && canClimbDown){
      y += 16;
      walking_animation_sequence++;
    }
  }

  //Move right
  if(key[KEY_D] && jumping_animation_done && x < newMap -> width*64){
    characterDir = RIGHT;
    if(canMoveRight){
      x += 8;
      walking_animation_sequence++;
      if(walking_animation_sequence %(ANIMATION_SPEED * 2) == 0 && !canFall && !jumping){
        if(random(0,1)){
          play_sample(walk1,255,125,1000,0);
        }
        else{
          play_sample(walk2,255,125,1000,0);
        }
      }
    }
  }

  //Move left
  if(key[KEY_A] && jumping_animation_done && x > 0){
    characterDir = LEFT;
    if(canMoveLeft){
      x -= 8;
      walking_animation_sequence++;
      if(walking_animation_sequence %(ANIMATION_SPEED * 2) == 0 && !canFall && !jumping){
        if(random(0,1)){
          play_sample(walk1,255,125,1000,0);
        }
        else{
          play_sample(walk2,255,125,1000,0);
        }
      }
    }
  }

  canFall = true;
  bool smoothFall = false;
  //Falling (calculated seperately to ensure collision accurate)
  for(int i = 0; i < newMap -> mapTiles.size(); i++){
    if(collisionAny(x - 64*2, x + 64*2, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y - 128*2, y + 128*2, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
      if(newMap -> mapTiles.at(i).getAttribute() == solid || newMap -> mapTiles.at(i).getAttribute() == climb){
        if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + 144, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64) &&
           collisionTop(y, y + 144, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 128)){
          canFall = false;
          if(!collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + 129, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64) &&
           !collisionTop(y, y + 129, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 129)){
            smoothFall = true;
          }
        }
      }
    }
  }

  //Fall
  if(canFall && !jumping){
    if(inLiquid){
      y += 16;
    }
    else{
      y += 16;
    }
  }
  //Smooth falling
  if(smoothFall){
    y += 1;
  }

  //Jumping
  if(jumping && canJumpUp){
    if(canFall || jump_height > 0 ){
      y -= yVelocity;
    }

    if(jump_height <= 0){
      if(yVelocity > -16){
        yVelocity -= 2;
      }
      else{
        yVelocity = 0;
        jumping = false;
      }
    }
    else{
      jump_height -= yVelocity;
    }
  }
  else if(jumping && !canJumpUp){
    jumping = false;
  }

  //Jump
  if(key[KEY_SPACE]){
    if(inLiquid){
      y -= 16;
    }
    else if(!canFall && canJump && !jumping && jumping_animation_done){
      yVelocity = 16;
      play_sample(jump,255,125,1000,0);
      jumping_animation_done = false;
      jump_height = 0;
    }
  }

  //Increse jump animation sequence
  if(!jumping_animation_done){
    jumping_animation_sequence++;
    if(key[KEY_SPACE]){
      if(jumping_animation_sequence < 7){
        jump_height=32;
      }
      else if(jumping_animation_sequence < 11){
        jump_height=86;
      }
      else if(jumping_animation_sequence < 12){
        jump_height = 160;
      }
    }
    if(jumping_animation_sequence > 10){
      jumping_animation_sequence = 0;
      jumping_animation_done = true;
      jumping = true;
    }
  }
  // Reset animation sequence
  if(walking_animation_sequence > ANIMATION_SPEED * 4){
    walking_animation_sequence = 0;
  }
  // Prevents getting stuck through an animation
  else if(!key[KEY_LEFT] && !key[KEY_RIGHT] && !key[KEY_A] && !key[KEY_D]){
    walking_animation_sequence = 0;
  }

  // Not shooting anymore
  if(shooting){
    shooting_animation_sequence++;
    if( shooting_animation_sequence > 9){
      shooting = false;
      shooting_animation_sequence = 0;
    }
  }

  // Shoot
  if(key[KEY_ENTER] && !shooting){
    if(characterDir == RIGHT){
      projectile newBullet( 0, x + 57, y + 70, 100);
      bullets.push_back(newBullet);
    }
    else if(characterDir == LEFT){
      projectile newBullet( 0, x - 64, y + 70, -100);
      bullets.push_back(newBullet);
    }
    shooting = true;
  }

  // Update bullets
  for(int i = 0; i < bullets.size(); i++){
    bullets.at(i).update();
    if(bullets.at(i).getContact(newMap) && bullets.at(i).getContactFrameCounter() == 10){
      bullets.erase(bullets.begin() + i);
    }
  }
}

player::~player(){
  bullets.clear();

  for(int i = 0; i < 26; i++){
		if(player_images[i]){
			destroy_bitmap(player_images[i]);
		}
	}

  destroy_sample( walk1);
  destroy_sample( walk2);
  destroy_sample( jump);
  destroy_sample( die);
  destroy_sample( getItem);
  destroy_sample( getBonus);
}
