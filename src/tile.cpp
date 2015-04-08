#include "tile.h"

// Automatic tile creator, when in doubt, use this one
tile::tile(int newType, vector<tile>* newTileSet){
  setTileSet( newTileSet);
  setType( newType);
}

// Manual tile creator, must be used when no tileSet has been created (for example before it is loaded)
// In order to work fully, it must be called alongside setImages(...); and addAttribute(...);
// If this is used, it is not possible to change types using setType unless setTileSet(...) is called first
tile::tile(int newType, bool newParticles, bool newLighting, string newName){
  type = newType;
  particlesEnabled = newParticles;
  lightingEnabled = newLighting;
  name = newName;
}

// Destroy tile
tile::~tile(){ }


// Get coordinates
int tile::getX(){
  return x;
}
int tile::getY(){
  return y;
}

// Get size
int tile::getWidth(){
  return width;
}
int tile::getHeight(){
  return height;
}

// Set new coordinates
void tile::setX(int newX){
  x = newX;
  initialX = x;
}
void tile::setY(int newY){
  y = newY;
  initialY = y;
}

// Get tile type
int tile::getType(){
  return type;
}

// Get animation type (0 = 1 frame, 1 = 4 frame, 8 = 8 frame)
int tile::getAnimated(){
  return animated;
}

// Gets a vector containing any attributes of the tile
// (e.g. may return a vector containing 2 attributes, gas and harmful)
vector<int> tile::getAttribute(){
  return attribute;
}

// Returns true if the tile contains the given attribute (e.g. solid, gas)
bool tile::containsAttribute( int newAttribute){
  if (find(attribute.begin(), attribute.end(), newAttribute) != attribute.end()){
  	return true;
	}
	return false;
}

// Add an attribute to the tile (e.g. solid, harmful)
void tile::addAttribute( int newAttribute){
  attribute.push_back(newAttribute);
}

// Returns images[0], even if it is NOT loaded in which case it returns NULL
BITMAP* tile::getImage(){
  return images[0];
}

// Set tile type and automatically assigns attributes/images/ect...
// Warning! tileSet must have been initialized else this will not work!
void tile::setType(int newType){
  // Reset x and y position
  x = initialX;
  y = initialY;

  // Set type to defined type
  type = newType;

  // Tile copying
  tile* tileToCopy;

  // Remove all attributes
  attribute.clear();

  // Match found
  bool matchFound = false;

  // Find matching index tile
  for( int i = 0; i < tileSet -> size(); i++){
    if( tileSet -> at(i).getType() == type){
      tileToCopy = &(tileSet -> at(i));
      matchFound = true;
      break;
    }
  }

  if( matchFound){
    // Set attributes to ones found in the index
    for( int t = 0; t < tileToCopy -> getAttribute().size(); t++){
      attribute.push_back(tileToCopy -> getAttribute().at(t));
    }

    // Sets images
    if(tileToCopy -> getAnimated() == 2){
      setImages(tileToCopy -> images[0],
                tileToCopy -> images[1],
                tileToCopy -> images[2],
                tileToCopy -> images[3],
                tileToCopy -> images[4],
                tileToCopy -> images[5],
                tileToCopy -> images[6],
                tileToCopy -> images[7]);
    }
    else if(tileToCopy -> getAnimated() == 1){
      setImages(tileToCopy -> images[0],
                tileToCopy -> images[1],
                tileToCopy -> images[2],
                tileToCopy -> images[3]);
    }
    else{
      setImages(tileToCopy -> images[0]);
    }
  }
}

// Set images (and automatically changes animation to 0, 1 or 2)
void tile::setImages(BITMAP* image1, BITMAP* image2, BITMAP* image3, BITMAP* image4, BITMAP* image5, BITMAP* image6, BITMAP* image7, BITMAP* image8){
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
void tile::setImages(BITMAP* image1, BITMAP* image2, BITMAP* image3, BITMAP* image4){
  images[0] = image1;
  images[1] = image2;
  images[2] = image3;
  images[3] = image4;
  animated = 1;
  setDimensions();
}
void tile::setImages(BITMAP* image1){
  images[0] = image1;
  animated = 0;
  setDimensions();
}

// Draws tile. If no images are assigned it prints out the type number instead
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

// Gives the tile an index of all tiles in the tile map, used when assigning type
void tile::setTileSet( vector<tile>* newTileSet){
  tileSet = newTileSet;
}

// Sets dimensions to images[0]'s dimensions
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
