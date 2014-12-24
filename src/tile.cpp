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

void tile::setX(int newX){
  x = newX;
}
void tile::setY(int newY){
  y = newY;
}
int tile::getType(){
  return type;
}
int tile::getAttribute(){
  return attribute;
}
BITMAP* tile::getImage(){
  return images[0];
}

//Set type
void tile::setType(int newType){
  type = newType;

  // Sets imagses
  if(tileSprites[type][1] != NULL){
    setImagesAnimated(tileSprites[type][0], tileSprites[type][1], tileSprites[type][2], tileSprites[type][3]);
  }
  else{
    setImages(tileSprites[type][0]);
  }

  // Fronts
  if(type == tile_air){
    attribute = gas;
  }
  else if(type == tile_blue_top){
    attribute = solid;
  }
  else if(type == tile_blue_bottom){
    attribute = solid;
  }
  else if(type == tile_brick_blue){
    attribute = solid;
  }
  else if(type == tile_brick_red){
    attribute = solid;
  }
  else if(type == tile_brick_yellow){
    attribute = solid;
  }
  else if(type == tile_brick_green){
    attribute = solid;
  }
  else if(type == tile_lollypop){
    attribute = gas;
  }
  else if(type == tile_lava){
    attribute = harmful;
  }
  else if(type == tile_light_pillar){
    attribute = gas;
  }
  else if(type == tile_zapper){
    attribute = harmful;
  }
  else if(type == tile_green_light){
    attribute = gas;
  }
  else if(type == tile_red_light){
    attribute = gas;
  }
  else if(type == tile_id){
    attribute = gas;
  }
  else if(type == tile_old_vorticon){
    attribute = gas;
  }
  else if(type == tile_happy){
    attribute = solid;
  }
  else if(type == tile_spawn_player){
    attribute = spawn;
  }
  else{
    attribute = gas;
  }
}

//Set tiles images
void tile::setImagesAnimated(BITMAP* image1, BITMAP* image2, BITMAP* image3, BITMAP* image4){
  images[0] = image1;
  images[1] = image2;
  images[2] = image3;
  images[3] = image4;
  animated = true;
}

//Set tiles images
void tile::setImages(BITMAP* image1){
  images[0] = image1;
  animated = false;
}

//Draw tile
void tile::draw_tile(BITMAP* tempSprite, int xOffset, int yOffset, int frame){
  if(images[0] != NULL){
    //Animated?
    if(animated){
      if(frame == 0){
        if(images[0] != NULL){
          draw_sprite(tempSprite, images[0], x - xOffset, y - yOffset);
        }
        else{
          textprintf_ex(tempSprite,font,x - xOffset, y - yOffset,makecol(0,0,0),-1,"frame");
          textprintf_ex(tempSprite,font,x - xOffset, y - yOffset + 20,makecol(0,0,0),-1,"%i NA", frame + 1);
        }
      }
      else if(frame == 1){
        if(images[1] != NULL){
          draw_sprite(tempSprite, images[1], x - xOffset, y - yOffset);
        }
        else{
          textprintf_ex(tempSprite,font,x - xOffset, y - yOffset,makecol(0,0,0),-1,"frame");
          textprintf_ex(tempSprite,font,x - xOffset, y - yOffset + 20,makecol(0,0,0),-1,"%i NA", frame + 1);
        }
      }
      else if(frame == 2){
        if(images[2] != NULL){
          draw_sprite(tempSprite, images[2], x - xOffset, y - yOffset);
        }
        else{
          textprintf_ex(tempSprite,font,x - xOffset, y - yOffset,makecol(0,0,0),-1,"frame");
          textprintf_ex(tempSprite,font,x - xOffset, y - yOffset + 20,makecol(0,0,0),-1,"%i NA", frame + 1);
        }
      }
      else if(frame == 3){
        if(images[3] != NULL){
          draw_sprite(tempSprite, images[3], x - xOffset, y - yOffset);
        }
        else{
          textprintf_ex(tempSprite,font,x - xOffset, y - yOffset,makecol(0,0,0),-1,"frame");
          textprintf_ex(tempSprite,font,x - xOffset, y - yOffset + 20,makecol(0,0,0),-1,"%i NA", frame + 1);
        }
      }
    }
    //Not animated
    else{
      draw_sprite(tempSprite, images[0], x - xOffset, y - yOffset);
    }
  }
  //Image not available? draw debug
  else{
    textprintf_ex(tempSprite,font,x - xOffset, y - yOffset,makecol(0,0,0),-1,"Image");
    textprintf_ex(tempSprite,font,x - xOffset, y - yOffset + 20,makecol(0,0,0),-1,"%i NA", type);
  }
}

tile::~tile(){
  /*for(int i = 0; i < 4; i++){
		if(images[i]){
			destroy_bitmap(images[i]);
		}
	}*/
}
