#include "tile.h"

tile::tile(int newType){
  setType(newType);
}

//Getters/ setters
int tile::getX(){
  return x;
}
int tile::getY(){
  return y;
}

int tile::getWidth(){
  return width;
}
int tile::getHeight(){
  return height;
}

void tile::setX(int newX){
  x = newX;
  initialX = x;
}
void tile::setY(int newY){
  y = newY;
  initialY = y;
}
int tile::getType(){
  return type;
}
vector<int> tile::getAttribute(){
  return attribute;
}
// Contains Attribute
bool tile::containsAttribute( int newAttribute){
  if (find(attribute.begin(), attribute.end(), newAttribute) != attribute.end()) {
  	return true;
	}
	return false;
}

BITMAP* tile::getImage(){
  return images[0];
}

//Set type
void tile::setType(int newType){
  x = initialX;
  y = initialY;

  type = newType;
  attribute.clear();

  // Fronts
  if(type == tile_blue_top){
    attribute.push_back(solid);
  }
  else if(type == tile_blue_bottom){
    attribute.push_back(solid);
  }
  else if(type == tile_brick_blue){
    attribute.push_back(solid);
  }
  else if(type == tile_brick_red){
    attribute.push_back(solid);
  }
  else if(type == tile_lollypop){
    attribute.push_back(gas);
    attribute.push_back(item);
  }
  else if(type == tile_zapper){
    attribute.push_back(harmful);
  }
  else if(type == tile_brick_yellow){
    attribute.push_back(solid);
  }
  else if(type == tile_brick_green){
    attribute.push_back(solid);
  }
  else if(type == tile_lava){
    attribute.push_back(harmful);
  }
  else if(type == tile_happy){
    attribute.push_back(solid);
  }
  else if(type == tile_spawn_player){
    attribute.push_back(spawn);

  }
  else{
    attribute.push_back(gas);
  }

  // Sets images
  if(tileSprites[type][4] != NULL){
    setImagesAnimatedEight(tileSprites[type][0], tileSprites[type][1], tileSprites[type][2], tileSprites[type][3], tileSprites[type][4], tileSprites[type][5], tileSprites[type][6], tileSprites[type][7]);
  }
  else if(tileSprites[type][1] != NULL){
    setImagesAnimated(tileSprites[type][0], tileSprites[type][1], tileSprites[type][2], tileSprites[type][3]);
  }
  else{
    setImages(tileSprites[type][0]);
  }
}

//Set tiles images
void tile::setImagesAnimatedEight(BITMAP* image1, BITMAP* image2, BITMAP* image3, BITMAP* image4, BITMAP* image5, BITMAP* image6, BITMAP* image7, BITMAP* image8){
  images[0] = image1;
  images[1] = image2;
  images[2] = image3;
  images[3] = image4;
  images[4] = image5;
  images[5] = image6;
  images[6] = image7;
  images[7] = image8;
  animated = 2;
  setDimensions();
}

//Set tiles images
void tile::setImagesAnimated(BITMAP* image1, BITMAP* image2, BITMAP* image3, BITMAP* image4){
  images[0] = image1;
  images[1] = image2;
  images[2] = image3;
  images[3] = image4;
  animated = 1;
  setDimensions();
}

//Set tiles images
void tile::setImages(BITMAP* image1){
  images[0] = image1;
  animated = 0;
  setDimensions();
}

// Set images dimensions
void tile::setDimensions(){
  if(images[0] != NULL){
    width = images[0] -> w;
    height = images[0] -> h;
    if( containsAttribute( half_block_top)){
      height = 32;
    }
    else if( containsAttribute( half_block_bottom)){
      height = 32;
      y += height;
    }
    else if( containsAttribute( quarter_block_top)){
      height = 16;
    }
    else if( containsAttribute( quarter_block_bottom)){
      height = 16;
      y += 48;
    }
  }
}

//Draw tile
void tile::draw_tile(BITMAP* tempSprite, int xOffset, int yOffset, int newFrame){
  if(images[0] != NULL){
    //Not animated
    if( animated == 0){
      draw_sprite(tempSprite, images[0], x - xOffset, y - yOffset);
    }
    //Animated?
    else if(animated == 1){
      if( newFrame > 3){
        newFrame = newFrame - 4;
      }
      if(images[newFrame] != NULL){
        draw_sprite(tempSprite, images[newFrame], x - xOffset, y - yOffset);
      }
      else{
        textprintf_ex(tempSprite,font,x - xOffset, y - yOffset,makecol(0,0,0),-1,"frame");
        textprintf_ex(tempSprite,font,x - xOffset, y - yOffset + 20,makecol(0,0,0),-1,"%i NA", newFrame + 1);
      }
    }
    else if(animated == 2){
      if(images[newFrame] != NULL){
        draw_sprite(tempSprite, images[newFrame], x - xOffset, y - yOffset);
      }
      else{
        textprintf_ex(tempSprite,font,x - xOffset, y - yOffset,makecol(0,0,0),-1,"frame");
        textprintf_ex(tempSprite,font,x - xOffset, y - yOffset + 20,makecol(0,0,0),-1,"%i NA", newFrame + 1);
      }
    }
  }
  //Image not available? draw debug
  else{
    textprintf_ex(tempSprite,font,x - xOffset, y - yOffset,makecol(0,0,0),-1,"Image");
    textprintf_ex(tempSprite,font,x - xOffset, y - yOffset + 20,makecol(0,0,0),-1,"%i NA", type);
  }
}

tile::~tile(){

}
