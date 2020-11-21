#include "player.h"

player::player(){

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

  newMap = new tileMap("blank");
}

// 0-3 left, 4-7 right, 8-11 up
void player::load_images( int newType){
  if( newType == 1){
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
  }
  else if( newType == 2){
  }

  projectileSprites[0][0] = load_bitmap("images/laser.png", NULL);
  projectileSprites[0][1] = load_bitmap("images/laser.png", NULL);
  projectileSprites[0][2] = load_bitmap("images/laser_hit.png", NULL);
}

// Load sounds
void player::load_sounds(){
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
  if(!(win = load_sample("sounds/get_bonus.wav"))){
    abort_on_error( "Cannot find sound sounds/getBonus.wav \n Please check your files and try again");
  }
}

// Set keys
void player::set_keys( int up, int down, int left, int right, int jump, int shoot, int newJoyNumber){
  // Differentiate controls
  upKey = up;
  downKey = down;
  leftKey = left;
  rightKey = right;
  jumpKey = jump;
  joyNumber = newJoyNumber;
  shootKey = shoot;
}

int player::getDeathcount(){
  return deathcount;
}

// Return X
int player::getX(){
  return x;
}

// Return Y
int player::getY(){
  return y;
}

// Get finished
bool player::getFinished(){
  return finished;
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

// Set finished
void player::setFinished( bool newFinished){
    finished = newFinished;
}
//Set deathcount
void player::setDeathcount(int newDeathcount){
  deathcount = newDeathcount;
}
//Set dead
void player::setDead(bool newDead){
  dead = newDead;
}

//Draw character
void player::draw(BITMAP* temp, int tile_map_x, int tile_map_y){
  if( !shooting){
    // Jumping
    if(!jumping_animation_done){
      // Left
      if(characterDir == LEFT){
        draw_sprite( temp, player_images[12 + jumping_animation_sequence/2], x - tile_map_x, y - tile_map_y);
      }
      // Right
      else{
        draw_sprite( temp, player_images[18 + jumping_animation_sequence/2], x - tile_map_x, y - tile_map_y);
      }
    }
    // Falling
    else if(jumping || canFall){
      // Left
      if(characterDir == LEFT){
        draw_sprite( temp, player_images[17], x - tile_map_x, y - tile_map_y);
      }
      // Right
      else{
        draw_sprite( temp, player_images[23], x - tile_map_x, y - tile_map_y);
      }
    }
    // Walking
    else{
      draw_sprite( temp, player_images[characterDir + walking_animation_sequence/ANIMATION_SPEED], x - tile_map_x, y - tile_map_y);
    }
  }
  // Shooting
  else{
    // Left
    if(characterDir == LEFT){
      draw_sprite( temp, player_images[24], x - tile_map_x, y - tile_map_y);
    }
    // Right
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
void player::spawncommand(tileMap *fullMap){
  for(int i = 0; i < fullMap -> mapTiles.size(); i++){
    if(fullMap -> mapTiles.at(i).containsAttribute(spawn)){
      x = fullMap -> mapTiles.at(i).getX();
      y = fullMap -> mapTiles.at(i).getY();
    }
  }
}

//Movement
void player::update(tileMap *fullMap){
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
    // Check moving LEFT
    if(newMap -> mapTiles.at(i).containsAttribute(solid)){
      if(collisionAny(x - 4, x + 60, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + newMap -> mapTiles.at(i).getWidth(), y, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight()) &&
         collisionLeft(x - 4, x + 60, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64)){
        canMoveLeft = false;
      }
    }

    // Check moving RIGHT
    if(newMap -> mapTiles.at(i).containsAttribute(solid)){
      if(collisionAny(x + 4, x + 68, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + newMap -> mapTiles.at(i).getWidth(), y, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight()) &&
         collisionRight(x + 4, x + 68, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64)){
        canMoveRight = false;
      }
    }
    // Check 2 for climbing up
    if(newMap -> mapTiles.at(i).containsAttribute(climb)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + newMap -> mapTiles.at(i).getWidth(), y, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        canClimbUp2 = true;
      }
    }
    // Check 2 for climbing down
    if(newMap -> mapTiles.at(i).containsAttribute(climb)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + newMap -> mapTiles.at(i).getWidth(), y, y + 144, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        canClimbDown2 = true;
      }
    }
    // Check 1 for climbing up
    if(newMap -> mapTiles.at(i).containsAttribute(solid)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + newMap -> mapTiles.at(i).getWidth(), y - 16, y, newMap -> mapTiles.at(i).getY(),newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        canClimbUp = false;
      }
    }
    // Check 2 for climbing down
    if(newMap -> mapTiles.at(i).containsAttribute(solid)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + newMap -> mapTiles.at(i).getWidth(), y, y + 144, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        canClimbDown = false;
      }
    }
    // Check jump
    if(!(newMap -> mapTiles.at(i).containsAttribute(gas)) && newMap -> mapTiles.at(i).containsAttribute(liquid)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + newMap -> mapTiles.at(i).getWidth(), y, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        canJump = false;
      }
    }
    // Can if you will not hit your head
    if(!(newMap -> mapTiles.at(i).containsAttribute(gas)) || newMap -> mapTiles.at(i).containsAttribute(liquid)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + newMap -> mapTiles.at(i).getWidth(), newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64, y, y + 144) &&
         collisionTop(newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 128, y + yVelocity, y + 144 + yVelocity)){
        canJumpUp = false;
      }
    }

    // If you are swimming
    if(newMap -> mapTiles.at(i).containsAttribute(liquid)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + newMap -> mapTiles.at(i).getWidth(), y-16, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
        inLiquid = true;
      }
    }
  }

  //Check for points and dangers
  for(int i = 0; i < fullMap -> mapTiles.size(); i++){
    //Get point
    if(fullMap -> mapTiles.at(i).containsAttribute(item)){
      if(collisionAny(x + 16, x + 48, fullMap -> mapTiles.at(i).getX(), fullMap -> mapTiles.at(i).getX() + fullMap -> mapTiles.at(i).getWidth(), y+32, y + 128, fullMap -> mapTiles.at(i).getY(), fullMap -> mapTiles.at(i).getY() +  fullMap -> mapTiles.at(i).getHeight())){
        play_sample(getItem,255,125,1000,0);
        fullMap -> mapTiles.at(i).setType(0);
      }
    }
  }

  for(int i = 0; i < newMap -> mapTiles.size(); i++){
    //Die
    if(newMap -> mapTiles.at(i).containsAttribute(finish)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y + 32, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        play_sample(win,255,125,1000,0);
        finished = true;
      }
    }
    if(newMap -> mapTiles.at(i).containsAttribute(harmful)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + newMap -> mapTiles.at(i).getWidth(), y+32, y + 128, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        play_sample(die,255,125,1000,0);
        dead = true;
        deathcount++;
      }
    }
    if(x > newMap -> width*64 || x < 0 || y > newMap -> height*64){
      x=0;
      y=0;
      play_sample(die,255,125,1000,0);
      dead = true;
      deathcount++;
    }
  }

  //Move up
  if(key[upKey]){
    characterDir = UP;
    if(canClimbUp2 && canClimbUp){
      y -= 16;
      walking_animation_sequence++;
    }
  }

  //Move down
  if(key[downKey]){
    characterDir = UP;
    if(canClimbDown2 && canClimbDown){
      y += 16;
      walking_animation_sequence++;
    }
  }

  //Move right
  if((key[rightKey] || joy[joyNumber].stick[0].axis[0].d2) && jumping_animation_done && x < newMap -> width*64){
    if( !sprinting){
      sprinting = true;
      sprintSpeed = 8;
    }
    else if( sprintSpeed < 16 && !jumping){
      sprintSpeed *= 1.05;
    }
    characterDir = RIGHT;
    if(canMoveRight){
      x += sprintSpeed;
      walking_animation_sequence++;
      if(walking_animation_sequence == ANIMATION_SPEED && !canFall && !jumping){
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
  if((key[leftKey] || joy[joyNumber].stick[0].axis[0].d1) && jumping_animation_done && x > 0){
    if( !sprinting){
      sprinting = true;
      sprintSpeed = 8;
    }
    else if( sprintSpeed < 16 && !jumping){
      sprintSpeed *= 1.05;
    }
    characterDir = LEFT;
    if(canMoveLeft){
      x -= sprintSpeed;
      walking_animation_sequence++;
      if(walking_animation_sequence == ANIMATION_SPEED && !canFall && !jumping){
        if(random(0,1)){
          play_sample(walk1,255,125,1000,0);
        }
        else{
          play_sample(walk2,255,125,1000,0);
        }
      }
    }
  }

  // Stop sprinting
  if(!key[leftKey] && !key[rightKey] && !joy[joyNumber].stick[0].axis[0].d1 && !joy[joyNumber].stick[0].axis[0].d2){
    sprinting = false;
    sprintSpeed = 0;
  }

  canFall = true;
  bool smoothFall = false;

  //Falling (calculated seperately to ensure collision accurate)
  for(int i = 0; i < newMap -> mapTiles.size(); i++){
    if(newMap -> mapTiles.at(i).containsAttribute(solid) || newMap -> mapTiles.at(i).containsAttribute(climb)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + newMap -> mapTiles.at(i).getWidth(), y, y + 144, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + newMap -> mapTiles.at(i).getHeight()) &&
         collisionTop(y, y + 144, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + newMap -> mapTiles.at(i).getHeight())){
        canFall = false;
        if(!collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + newMap -> mapTiles.at(i).getWidth(), y, y + 129, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + newMap -> mapTiles.at(i).getHeight()) &&
         !collisionTop(y, y + 129, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + newMap -> mapTiles.at(i).getHeight())){
          smoothFall = true;
        }
      }
    }
  }

  //Fall
  if(canFall && !jumping){
    if(inLiquid){
      y += 2;
    }
    else{
      if( key[downKey]){
        y += 32;
      }
      else{
        y += 16;
      }
    }
  }
  //Smooth falling
  if(smoothFall && !jumping){
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
  if(key[jumpKey]){
    if(inLiquid){
      y -= 4;
    }
    else if(!canFall && canJump && !jumping && jumping_animation_done){
      yVelocity = 16;
      play_sample(jump,255,125,1000,0);
      jumping_animation_done = false;
      jump_height = 0;
    }
  }

  //Increase jump animation sequence
  if(!jumping_animation_done){
    jumping_animation_sequence++;
    if(key[jumpKey]){
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
  if(walking_animation_sequence >= ANIMATION_SPEED * 4){
    walking_animation_sequence = 0;
  }
  // Prevents getting stuck through an animation
  else if(!key[rightKey] && !key[leftKey]){
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
  if(key[shootKey] && !shooting){
    if(characterDir == RIGHT){
      projectile newBullet( 0, x + 57, y + 70, 100, random( -10, 10));
      bullets.push_back(newBullet);
    }
    else if(characterDir == LEFT){
      projectile newBullet( 0, x - 64, y + 70, -100, 0);
      bullets.push_back(newBullet);
    }
    shooting = true;
  }

  // Update bullets
  for(int i = 0; i < bullets.size(); i++){
    if( bullets.at(i).getContact(fullMap) && bullets.at(i).getContactFrameCounter() == 10){
      bullets.erase(bullets.begin() + i);
    }
    else{
      bullets.at(i).update();
    }
  }

  // Prevents getting stuck through an animation
  if(!key[leftKey] && !key[rightKey] && !key[upKey] && !key[downKey] && !joy[joyNumber].stick[0].axis[0].d1 && !joy[joyNumber].stick[0].axis[0].d2 && !joy[joyNumber].stick[0].axis[2].d1){
    walking_animation_sequence = 0;
  }
  idle_timer++;
  if(idle_timer>100)idle_timer=0;
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
  destroy_sample( win);
  destroy_sample( die);
  destroy_sample( getItem);
  destroy_sample( getBonus);
}
