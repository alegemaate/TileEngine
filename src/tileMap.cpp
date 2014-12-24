#include "tileMap.h"

volatile long tileMap::frame = 0;

tileMap::tileMap(string fileName){
  x = 0;
  y = 0;

  width = 0;
  height = 0;

  LOCK_VARIABLE(frame);
  LOCK_FUNCTION(change_frame);
  install_int_ex(change_frame, MSEC_TO_TIMER(FRAME_MILASECONDS));

  load_images();
  load(fileName);
}

void tileMap::load_images(){
  // Tile Sprites
  // Not animated
  tileSprites[tile_air][0] = load_bitmap("images/blocks/air.png", NULL);
  tileSprites[tile_blue_top][0] = load_bitmap("images/blocks/blue_top.png", NULL);
  tileSprites[tile_blue_bottom][0] = load_bitmap("images/blocks/blue_bottom.png", NULL);
  tileSprites[tile_brick_blue][0] = load_bitmap("images/blocks/brick_blue.png", NULL);
  tileSprites[tile_brick_red][0] = load_bitmap("images/blocks/brick_red.png", NULL);
  tileSprites[tile_brick_yellow][0] = load_bitmap("images/blocks/brick_yellow.png", NULL);
  tileSprites[tile_brick_green][0] = load_bitmap("images/blocks/brick_green.png", NULL);
  tileSprites[tile_lollypop][0] = load_bitmap("images/blocks/lollypop.png", NULL);
  tileSprites[tile_old_vorticon][0] = load_bitmap("images/blocks/vorticon_old.png", NULL);
  tileSprites[tile_happy][0] = load_bitmap("images/blocks/happy.png", NULL);

  // Animated
  tileSprites[tile_lava][0] = load_bitmap("images/blocks/lava1.png", NULL);
  tileSprites[tile_lava][1] = load_bitmap("images/blocks/lava2.png", NULL);
  tileSprites[tile_lava][2] = load_bitmap("images/blocks/lava3.png", NULL);
  tileSprites[tile_lava][3] = load_bitmap("images/blocks/lava4.png", NULL);

  tileSprites[tile_light_pillar][0] = load_bitmap("images/blocks/light_pillar1.png", NULL);
  tileSprites[tile_light_pillar][1] = load_bitmap("images/blocks/light_pillar2.png", NULL);
  tileSprites[tile_light_pillar][2] = load_bitmap("images/blocks/light_pillar3.png", NULL);
  tileSprites[tile_light_pillar][3] = load_bitmap("images/blocks/light_pillar4.png", NULL);

  tileSprites[tile_zapper][0] = load_bitmap("images/blocks/zapper1.png", NULL);
  tileSprites[tile_zapper][1] = load_bitmap("images/blocks/zapper2.png", NULL);
  tileSprites[tile_zapper][2] = load_bitmap("images/blocks/zapper3.png", NULL);
  tileSprites[tile_zapper][3] = load_bitmap("images/blocks/zapper4.png", NULL);

  tileSprites[tile_green_light][0] = load_bitmap("images/blocks/green_light1.png", NULL);
  tileSprites[tile_green_light][1] = load_bitmap("images/blocks/green_light2.png", NULL);
  tileSprites[tile_green_light][2] = load_bitmap("images/blocks/green_light1.png", NULL);
  tileSprites[tile_green_light][3] = load_bitmap("images/blocks/green_light2.png", NULL);

  tileSprites[tile_red_light][0] = load_bitmap("images/blocks/red_light1.png", NULL);
  tileSprites[tile_red_light][1] = load_bitmap("images/blocks/red_light2.png", NULL);
  tileSprites[tile_red_light][2] = load_bitmap("images/blocks/red_light3.png", NULL);
  tileSprites[tile_red_light][3] = load_bitmap("images/blocks/red_light4.png", NULL);

  tileSprites[tile_id][0] = load_bitmap("images/blocks/id1.png", NULL);
  tileSprites[tile_id][1] = load_bitmap("images/blocks/id2.png", NULL);
  tileSprites[tile_id][2] = load_bitmap("images/blocks/id3.png", NULL);
  tileSprites[tile_id][3] = load_bitmap("images/blocks/id4.png", NULL);

  //Back Tiles
  tileSprites[tile_back_grey][0] = load_bitmap("images/blocks/backs/grey.png", NULL);

  tileSprites[tile_back_grey_shadow_top][0] = load_bitmap("images/blocks/backs/grey_shadow_top.png", NULL);
  tileSprites[tile_back_grey_shadow_top_right][0] = load_bitmap("images/blocks/backs/grey_shadow_top_right.png", NULL);
  tileSprites[tile_back_grey_shadow_top_left][0] = load_bitmap("images/blocks/backs/grey_shadow_top_left.png", NULL);
  tileSprites[tile_back_grey_shadow_left][0] = load_bitmap("images/blocks/backs/grey_shadow_left.png", NULL);
  tileSprites[tile_back_grey_shadow_bottom_left][0] = load_bitmap("images/blocks/backs/grey_shadow_bottom_left.png", NULL);
  tileSprites[tile_back_grey_shadow_top_left_corner][0] = load_bitmap("images/blocks/backs/grey_shadow_top_left_corner.png", NULL);

  tileSprites[tile_back_grey_dark][0] = load_bitmap("images/blocks/backs/grey_dark.png", NULL);
  tileSprites[tile_back_grey_dark_top_left][0] = load_bitmap("images/blocks/backs/grey_dark_top_left.png", NULL);
  tileSprites[tile_back_grey_dark_top_right][0] = load_bitmap("images/blocks/backs/grey_dark_top_right.png", NULL);

  tileSprites[tile_back_lamp][0] = load_bitmap("images/blocks/backs/lamp.png", NULL);

  //Spawners
  tileSprites[tile_spawn_player][0] = load_bitmap("images/blocks/spawn.png", NULL);
  tileSprites[tile_spawn_vorticon][0] = load_bitmap("images/enemys/vorticon/left_1.png", NULL);
  tileSprites[tile_spawn_robot][0] = load_bitmap("images/enemys/robot/left_1.png", NULL);
  tileSprites[tile_spawn_danny][0] = load_bitmap("images/enemys/danny/left_1.png", NULL);
}

//Change frame
void tileMap::change_frame(){
  if(frame < 3){
    frame++;
  }
  else{
    frame = 0;
  }
}
END_OF_FUNCTION(change_frame)

void tileMap::load(string fileName){
  //Change size
  string fileLoad = fileName + ".txt";
  ifstream findSize(fileLoad.c_str());
  width = 0;
  height = 0;
  int data;
  while (findSize >> data) {
    if(height == 0){
      width++;
    }
    if(findSize.peek() == '\n'){
      height++;
    }
  }

  //Setup Map
  if(fileName != "blank"){
    mapTiles.clear();
    mapTilesBack.clear();

    fileLoad = fileName + ".txt";
    ifstream read(fileLoad.c_str());

    for( int t = 0; t < height; t++){
      for (int i = 0; i < width; i++){
        int newTileType;
        read >> newTileType;
        // Set tile type
        tile newTile(newTileType);
        newTile.setX( i * 64);
        newTile.setY( t * 64);
        mapTiles.push_back(newTile);
      }
    }
    read.close();

    fileLoad = fileName + "_back.txt";
    ifstream read2(fileLoad.c_str());

    for( int t = 0; t < height; t++){
      for (int i = 0; i < width; i++){
        int newTileType;
        read2 >> newTileType;
        // Set tile type
        tile newTile(newTileType);
        newTile.setX( i * 64);
        newTile.setY( t * 64);
        mapTilesBack.push_back(newTile);
      }
    }
    read2.close();
  }
}

//Draw tile map
void tileMap::draw_map(BITMAP* tempSprite){
  for(int i = 0; i < mapTilesBack.size(); i++){
    if((mapTilesBack.at(i).getX() >= x - 64) && (mapTilesBack.at(i).getX() < x + 1280) && (mapTilesBack.at(i).getY() >= y - 64) &&  (mapTilesBack.at(i).getY() < y + 960)){
      mapTilesBack.at(i).draw_tile( tempSprite, x, y, frame);
    }
  }
  for(int i = 0; i < mapTiles.size(); i++){
    if((mapTiles.at(i).getX() >= x - 64) && (mapTiles.at(i).getX() < x + 1280) && (mapTiles.at(i).getY() >= y - 64) &&  (mapTiles.at(i).getY() < y + 960)){
      mapTiles.at(i).draw_tile( tempSprite, x, y, frame);
    }
  }
}

tileMap::~tileMap(){
  mapTiles.clear();
  mapTilesBack.clear();
}
