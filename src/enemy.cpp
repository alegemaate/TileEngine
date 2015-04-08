#include "enemy.h"

enemy::enemy( int newX, int newY, int newType){
  //Set enemy variables
  canFall = true;
  dead = false;
  jumping = false;
  characterDir = 0;
  walking_counter = 0;

  jumpChance = 0;
  maxSteps = 0;
  minSteps = 0;
  jumpHeight = 0;
  walkChance = 0;
  jumpSpeed = 0;
  speed = 0;

  x = newX;
  y = newY;
  type = newType;

  newMap = new tileMap("blank");
}

enemy::~enemy(){

}

// 0-3 left, 4-7 right, 8-11 up
void enemy::load_images(){
  if(type == enemy_vorticon){
    enemy_images[0] = load_bitmap ( "images/enemys/vorticon/left_1.png", NULL);
    enemy_images[1] = load_bitmap ( "images/enemys/vorticon/left_2.png", NULL);
    enemy_images[2] = load_bitmap ( "images/enemys/vorticon/left_3.png", NULL);
    enemy_images[3] = load_bitmap ( "images/enemys/vorticon/left_4.png", NULL);

    enemy_images[4] = load_bitmap ( "images/enemys/vorticon/right_1.png", NULL);
    enemy_images[5] = load_bitmap ( "images/enemys/vorticon/right_2.png", NULL);
    enemy_images[6] = load_bitmap ( "images/enemys/vorticon/right_3.png", NULL);
    enemy_images[7] = load_bitmap ( "images/enemys/vorticon/right_4.png", NULL);

    enemy_images[8] = load_bitmap ( "images/enemys/vorticon/jump_left.png", NULL);
    enemy_images[9] = load_bitmap ( "images/enemys/vorticon/jump_right.png", NULL);

    enemy_images[10] = load_bitmap ( "images/enemys/vorticon/dead.png", NULL);

    jumpChance = 100;
    maxSteps = 100;
    minSteps = 20;
    jumpHeight = 128;
    walkChance = 20;
    jumpSpeed = 2;
    speed = 3;
  }
  else if(type == enemy_robot){
    enemy_images[0] = load_bitmap ( "images/enemys/robot/left_1.png", NULL);
    enemy_images[1] = load_bitmap ( "images/enemys/robot/left_2.png", NULL);
    enemy_images[2] = load_bitmap ( "images/enemys/robot/left_3.png", NULL);
    enemy_images[3] = load_bitmap ( "images/enemys/robot/left_4.png", NULL);

    enemy_images[4] = load_bitmap ( "images/enemys/robot/right_1.png", NULL);
    enemy_images[5] = load_bitmap ( "images/enemys/robot/right_2.png", NULL);
    enemy_images[6] = load_bitmap ( "images/enemys/robot/right_3.png", NULL);
    enemy_images[7] = load_bitmap ( "images/enemys/robot/right_4.png", NULL);

    enemy_images[8] = load_bitmap ( "images/enemys/robot/jump_left.png", NULL);
    enemy_images[9] = load_bitmap ( "images/enemys/robot/jump_right.png", NULL);

    enemy_images[10] = load_bitmap ( "images/enemys/robot/dead.png", NULL);

    jumpChance = 0;
    maxSteps = 100;
    minSteps = 20;
    jumpHeight = 0;
    walkChance = 1;
    jumpSpeed = 0;
    speed = 2;
  }
  else if(type == enemy_danny){
    enemy_images[0] = load_bitmap ( "images/enemys/danny/left_1.png", NULL);
    enemy_images[1] = load_bitmap ( "images/enemys/danny/left_2.png", NULL);
    enemy_images[2] = load_bitmap ( "images/enemys/danny/left_3.png", NULL);
    enemy_images[3] = load_bitmap ( "images/enemys/danny/left_4.png", NULL);

    enemy_images[4] = load_bitmap ( "images/enemys/danny/right_1.png", NULL);
    enemy_images[5] = load_bitmap ( "images/enemys/danny/right_2.png", NULL);
    enemy_images[6] = load_bitmap ( "images/enemys/danny/right_3.png", NULL);
    enemy_images[7] = load_bitmap ( "images/enemys/danny/right_4.png", NULL);

    enemy_images[8] = load_bitmap ( "images/enemys/danny/jump_left.png", NULL);
    enemy_images[9] = load_bitmap ( "images/enemys/danny/jump_right.png", NULL);

    enemy_images[10] = load_bitmap ( "images/enemys/danny/dead.png", NULL);

    jumpChance = 20;
    maxSteps = 200;
    minSteps = 100;
    jumpHeight = 256;
    walkChance = 1;
    jumpSpeed = 3;
    speed = 4;
  }
  width = enemy_images[0] -> w;
  height = enemy_images[0] -> h;
}

// Load sounds
void enemy::load_sounds(){
  //if(type == enemy_vorticon){
    if(!(walk1 = load_sample("sounds/keen_walk_1.wav"))){
      abort_on_error( "Cannot find sound sounds/walk_1.wav \n Please check your files and try again");
    }
    if(!(walk2 = load_sample("sounds/keen_walk_2.wav"))){
      abort_on_error( "Cannot find sound sounds/walk_2.wav \n Please check your files and try again");
    }
    if(!(jump = load_sample("sounds/keen_jump.wav"))){
      abort_on_error( "Cannot find sound sounds/jump.wav \n Please check your files and try again");
    }
    if(!(die = load_sample("sounds/keen_die.wav"))){
      abort_on_error( "Cannot find sound sounds/die.wav \n Please check your files and try again");
    }
    if(!(getItem = load_sample("sounds/get_item.wav"))){
      abort_on_error( "Cannot find sound sounds/getItem.wav \n Please check your files and try again");
    }
    if(!(getBonus = load_sample("sounds/get_bonus.wav"))){
      abort_on_error( "Cannot find sound sounds/getBonus.wav \n Please check your files and try again");
    }
  //}
}

// Return X
int enemy::getX(){
  return x;
}

// Return Y
int enemy::getY(){
  return y;
}

// Dead?
bool enemy::getDead(){
  return dead;
}

//Set dead
void enemy::setDead(bool newDead){
  dead = newDead;
}

//Draw enemy
void enemy::draw(BITMAP* temp, int tile_map_x, int tile_map_y){
  if(!dead){
    if(jumping || canFall){
      if(characterDir == LEFT){
        draw_sprite( temp, enemy_images[8], x - tile_map_x, y - tile_map_y);
      }
      else{
        draw_sprite( temp, enemy_images[9], x - tile_map_x, y - tile_map_y);
      }
    }
    else{
      draw_sprite( temp, enemy_images[characterDir + walking_animation_sequence/ANIMATION_SPEED], x - tile_map_x, y - tile_map_y);
    }

    //Draw bullets
    for(int i = 0; i < bullets.size(); i++){
      bullets.at(i).draw(temp,tile_map_x, tile_map_y);
    }
  }
  else{
    draw_sprite( temp, enemy_images[10], x - tile_map_x, y - tile_map_y);
  }
}

// Update
void enemy::update(tileMap *fullMap, player *newPlayer){
  // Only update if on screen (save CPU power)!
  bool onScreen = true;
  if(collisionAny(x, x + width, newPlayer -> getX() - SCREEN_W, newPlayer -> getX() + SCREEN_W, y, y + height, newPlayer -> getY() - SCREEN_H, newPlayer -> getY() + SCREEN_H)){
    onScreen = true;
  }
  else{
    onScreen = false;
  }

  if(onScreen){
    if(!dead ){
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

      newMap -> mapTiles.clear();
      newMap -> width = fullMap -> width;
      newMap -> height = fullMap -> height;

      // Add close elements
      for(int i = 0; i < fullMap -> mapTiles.size(); i++){
        if(collisionAny(x - 140, x + 140, fullMap -> mapTiles.at(i).getX(), fullMap -> mapTiles.at(i).getX() +  fullMap -> mapTiles.at(i).getWidth(), y - 140, y + 140, fullMap -> mapTiles.at(i).getY(), fullMap -> mapTiles.at(i).getY() + fullMap -> mapTiles.at(i).getHeight())){
          newMap -> mapTiles.push_back( fullMap -> mapTiles.at(i));
        }
      }

      //Check for collision
      for(int i = 0; i < newMap -> mapTiles.size(); i++){
        if( newMap -> mapTiles.at(i).containsAttribute(solid)){
          if(collisionAny(x - speed, x + width - speed, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + height, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64) &&
             collisionLeft(x - speed, x + width - speed, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64)){
            canMoveLeft = false;
          }
        }
        if( newMap -> mapTiles.at(i).containsAttribute(solid)){
          if(collisionAny(x + speed, x + width + speed, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + height, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64) &&
             collisionRight(x + speed, x + width + speed, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64)){
            canMoveRight = false;
          }
        }
        if( newMap -> mapTiles.at(i).containsAttribute(climb)){
          if(collisionAny(x, x + width, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + height, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
            canClimbUp2 = true;
          }
        }
        if( newMap -> mapTiles.at(i).containsAttribute(climb)){
          if(collisionAny(x, x + width, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + height + 16, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
            canClimbDown2 = true;
          }
        }
        if( newMap -> mapTiles.at(i).containsAttribute(solid)){
          if(collisionAny(x, x + width, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y - 16, y, newMap -> mapTiles.at(i).getY(),newMap -> mapTiles.at(i).getY() + 64)){
            canClimbUp = false;
          }
        }
        if( newMap -> mapTiles.at(i).containsAttribute(solid)){
          if(collisionAny(x, x + width, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + height + 16, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
            canClimbDown = false;
          }
        }
        if( !newMap -> mapTiles.at(i).containsAttribute(gas) && !newMap -> mapTiles.at(i).containsAttribute(liquid)){
          if(collisionAny(x, x + width, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + height, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
            canJump = false;
          }
        }
        if( !newMap -> mapTiles.at(i).containsAttribute(gas) && !newMap -> mapTiles.at(i).containsAttribute(liquid)){
          if(collisionAny(x, x + width, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64, y, y + 144) &&
          collisionTop(newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 128, y + yVelocity, y + height + 16 + yVelocity)){
            canJumpUp = false;
          }
        }
        if( !newMap -> mapTiles.at(i).containsAttribute(liquid)){
          if(collisionAny(x, x + width, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y - 16, y + height, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
            inLiquid = true;
          }
        }
      }

      //Check for points and dangers
      for(int i = 0; i < newMap -> mapTiles.size(); i++){
        if(collisionAny(x - width*2, x + width*2, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y - height*2, y + height*2, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
          //Get point
          //if(newMap -> mapTiles.at(i).getType() == tile_lollypop){
          //  if(collisionAny(x, x + width, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y-16, y + height, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 128)){
          //    play_sample(getBonus,255,125,1000,0);
          //    newMap -> mapTiles.at(i).setType(0);
          //  }
          //}

          //Die
          if(newMap -> mapTiles.at(i).containsAttribute(harmful)){
            if(collisionAny(x, x + width, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y-16, y + height, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 128)){
              dead = true;
            }
          }
        }
      }

      // Check collision with bullets
      for(int i = 0; i < newPlayer -> getBullets().size(); i++){
        if(collisionAny(x, x + width, newPlayer -> getBullets().at(i).getX(), newPlayer -> getBullets().at(i).getX() + 64, y, y + height, newPlayer -> getBullets().at(i).getY(), newPlayer -> getBullets().at(i).getY() + 64)){
          dead = true;
        }
      }

      // AI set direction of walking
      if( walking_counter == 0){
        if(random(0,walkChance) == 0 && walkChance != 0){
          if(random(0,1) == 0){
            characterDir = RIGHT;
          }
          else{
            characterDir = LEFT;
          }
          walking_counter = random(minSteps,maxSteps);
        }
      }
      if( walking_counter > 0){
        walking_counter--;
      }

      //Move right
      if(characterDir == RIGHT && walking_counter > 0){
        if(canMoveRight){
          x += speed;
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
      if(characterDir == LEFT && walking_counter > 0){
        if(canMoveLeft){
          x -= speed;
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

      //Jumping
      if(jumping && canJumpUp){
        if(canFall || jump_height > 0 ){
          y -= yVelocity;
        }

        if(jump_height <= 0){
          if(yVelocity > -16){
            yVelocity -= jumpSpeed;
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
      if(random(0,jumpChance) == 0 && jumpChance != 0){
        if(inLiquid){
          y -= 16;
        }
        else if(!canFall && canJump && !jumping){
          yVelocity = 16;
          jump_height = jumpHeight;
          jumping = true;
        }
      }

      // Reset animation sequence
      if(walking_animation_sequence > ANIMATION_SPEED * 4){
        walking_animation_sequence = 0;
      }

      // Update bullets
      for(int i = 0; i < bullets.size(); i++){
        bullets.at(i).update();
        if(bullets.at(i).getContact(newMap) && bullets.at(i).getContactFrameCounter() == 10){
          bullets.erase(bullets.begin() + i);
        }
      }
    }

    canFall = true;
    bool smoothFall = false;
    //Falling (calculated seperately to ensure collision accurate)
    for(int i = 0; i < newMap -> mapTiles.size(); i++){
      if(collisionAny(x - width*2, x + width*2, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y - height*2, y + height*2, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
        if(newMap -> mapTiles.at(i).containsAttribute(solid) || newMap -> mapTiles.at(i).containsAttribute(climb)){
          if(collisionAny(x, x + 96, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + 144, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64) &&
             collisionTop(y, y + 144, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 128)){
            canFall = false;
            if(!collisionAny(x, x + 96, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + 129, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64) &&
             !collisionTop(y, y + 129, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 129)){
              smoothFall = true;
            }
          }
        }
      }
    }

    //Fall
    if(canFall && !jumping){
      y += 16;
    }
    //Smooth falling
    if(smoothFall){
      y += 1;
    }
  }
}
