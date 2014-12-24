#include "Game.h"

Game::Game()
{
  // Creates a random number generator (based on time)
  srand (time(NULL));

  // Other Sprites
  buffer = create_bitmap( 1280, 960);

  // Player
  character.load_images();
  character.load_sounds();

  // Init
  init();
}

void Game::init(){
  // Variables
  character.setDead(false);

  // Create map
  tile_map = new tileMap("data/levels/level_01");

  // Load enemies
  for(int i = 0; i < tile_map -> mapTiles.size(); i++){
    if(tile_map -> mapTiles.at(i).getType() > 199){
      enemy newBadGuy( tile_map -> mapTiles.at(i).getX(), tile_map -> mapTiles.at(i).getY(), tile_map -> mapTiles.at(i).getType() - 200);
      newBadGuy.load_images();
      newBadGuy.load_sounds();
      badGuy.push_back(newBadGuy);
      tile_map -> mapTiles.at(i).setType(0);
    }
  }

  //Create duplicate map with only solids for updating enemies
  newTileMap = new tileMap("blank");

  for(int i = 0; i < tile_map -> mapTiles.size(); i++){
    if(tile_map -> mapTiles.at(i).getAttribute() != gas){
      newTileMap -> mapTiles.push_back(tile_map -> mapTiles.at(i));
    }
  }
}

void Game::update()
{
  // Character movements (runs only every 2nd loop)
  if(frames_done % 2 == 0){
    character.update(tile_map);

    for(int i = 0; i < badGuy.size(); i++){
      badGuy.at(i).update(newTileMap, character);
    }
  }

  // Scroll Map
  if(character.getY() - tile_map -> y < 300 && tile_map -> y > 0){
     tile_map -> y -= 16;
  }
  if(character.getY() - tile_map -> y > 650 && tile_map -> y < tile_map -> height * 64 -  960){
     tile_map -> y += 16;
  }
  if(character.getX() - tile_map -> x < 400 && tile_map -> x > 0){
     tile_map -> x -= 8;
  }
  if(character.getX() - tile_map -> x > 880 && tile_map -> x < tile_map -> width * 64 - 1280){
     tile_map -> x += 8;
  }

  // Die
  if(character.getDead()){
    init();
  }

  // Respawn
  if(key[KEY_Y]){
    character.spawncommand(tile_map);
  }

  // Spawn enemy
  if(key[KEY_R]){
    enemy newBadGuy( character.getX(), character.getY(), enemy_danny);
    newBadGuy.load_images();
    newBadGuy.load_sounds();
    badGuy.push_back(newBadGuy);
  }

  // Back to menu
  if( key[KEY_M]){
    set_next_state( STATE_MENU);
  }
}

void Game::draw()
{
  // Black background (just in case)
  rectfill( buffer, 0, 0, 1280, 960, makecol(0,0,0));

  // Draw tiles
  tile_map -> draw_map(buffer);

  // Draw character
  character.draw(buffer, tile_map -> x, tile_map -> y);

  // Draw enemies
  for(int i = 0; i < badGuy.size(); i++){
    badGuy.at(i).draw(buffer, tile_map -> x, tile_map -> y);
  }

  // Draw buffer
  stretch_sprite( screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}

Game::~Game()
{
  destroy_bitmap( buffer);
}
