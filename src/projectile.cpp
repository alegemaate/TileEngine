#include "projectile.h"

projectile::projectile(int newType, float newX, float newY, float newXSpeed, float newYSpeed){
  x = newX;
  y = newY;
  x_speed = newXSpeed;
  y_speed = newYSpeed;
  type = 0;
  contact_counter = 0;
  contact = false;
  setType(newType);
  setSounds(newType);
  play_sample(sound[0],255,125,1000,0);

  angle = find_angle( 0, 0, x_speed, y_speed);

  newMap = new tileMap("blank");
}

projectile::~projectile(){

}

//Getters/ setters
int projectile::getX(){
  return x;
}
int projectile::getY(){
  return y;
}
int projectile::getType(){
  return type;
}
int projectile::getContactFrameCounter(){
  return contact_counter;
}

bool projectile::getContact(tileMap *fullMap){
  if( !contact){
    newMap -> mapTiles.clear();
    newMap -> width = fullMap -> width;
    newMap -> height = fullMap -> height;

    // Add close elements
    for(int i = 0; i < fullMap -> mapTiles.size(); i++){
      if(collisionAny(x - 140, x + 140, fullMap -> mapTiles.at(i).getX(), fullMap -> mapTiles.at(i).getX() +  fullMap -> mapTiles.at(i).getWidth(),
                      y - 140, y + 140, fullMap -> mapTiles.at(i).getY(), fullMap -> mapTiles.at(i).getY() + fullMap -> mapTiles.at(i).getHeight())){
        newMap -> mapTiles.push_back( fullMap -> mapTiles.at(i));
      }
    }

    //Check for collision
    for(int i = 0; i < newMap -> mapTiles.size(); i++){
      if(newMap -> mapTiles.at(i).containsAttribute( solid)){
        if(collisionAny(x + x_speed, x + image[0] -> w + x_speed, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64,
                        y + y_speed, y + image[0] -> h + y_speed, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
          contact = true;
          x = newMap -> mapTiles.at(i).getX();
          y = newMap -> mapTiles.at(i).getY();
          newMap -> mapTiles.at(i).changeType(1);
          break;
        }
      }
    }
    if( x < 0 || x > newMap -> width * 64 || y < 0 || y > newMap -> height * 64){
      contact = true;
    }
  }

  return contact;
}

//Set type
void projectile::setType(int newType){
  type = newType;

  //Sets images
  setImage(projectileSprites[type][0], projectileSprites[type][1], projectileSprites[type][2]);
}

//Set tiles images
void projectile::setImage(BITMAP* image1, BITMAP* image2, BITMAP* image3){
  image[0] = image1;
  image[1] = image2;
  image[2] = image3;
}

// Set Images
void projectile::setSounds( int newType){
  if(newType == 0){
    sound[0] = load_sample("sounds/shoot.wav");
    sound[1] = load_sample("sounds/shot_hit.wav");
  }
}

//Update
void projectile::update(){
  if( !contact){
    x += x_speed;
    y += y_speed;
  }
  else{
    if( contact_counter == 0){
      play_sample(sound[1],255,125,1000,0);
    }
    else if( contact_counter > 10){
      contact_counter = 10;
    }
    contact_counter++;
  }
}

//Draw projectile
void projectile::draw(BITMAP* tempSprite, int xOffset, int yOffset){
  if(!contact){
    rotate_sprite(tempSprite, image[0], x - xOffset, y - yOffset, itofix( convertRadiansToAllegro( angle)));
  }
  else{
    draw_sprite(tempSprite, image[2], x - xOffset, y - yOffset);
  }
}
