#include "Game.h"

volatile int Game::timer1 = 00;

// Create game state
Game::Game(){
  // Creates a random number generator (based on time)
  srand (time(NULL));

  // Other Sprites
  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  lightingEnabled = false;

  // Player
  player1.load_images(1);
  player1.load_sounds();
  player1.set_keys( KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LCONTROL, KEY_ALT, 0);

  // Init
  init();
}

void Game::gameTicker(){
  timer1++;
}
END_OF_FUNCTION(gameTicker)

// Init game state
void Game::init(){
  // Timer
  LOCK_VARIABLE(timer1);
  LOCK_FUNCTION(gameTicker);
  install_int_ex(gameTicker, BPS_TO_TIMER(100));

  tile_map = new tileMap( "blank");

  // Create map
  if( levelOn == 0){
    tile_map -> load( "data/levels/level_01");
  }
  else if( levelOn == 1){
    tile_map -> load( "data/levels/level_test");
  }
  else if( levelOn == 2){
    tile_map -> load( "data/saves/danny");
  }
  else{
    tile_map -> load( "data/saves/dannyII");
  }

  // Variables
  player1.setDead(false);
  player1.spawncommand( tile_map);

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
}

// Update game
void Game::update(){
  // Character movements (runs only every 2nd loop)
  if(frames_done % 2 == 0){
    player1.update(tile_map);

    for(int i = 0; i < badGuy.size(); i++){
      badGuy.at(i).update(tile_map, &player1);
    }
  }

  // Scroll Map
  if(player1.getY() - tile_map -> y < (SCREEN_H/4) && tile_map -> y > 0){ // UP
    if( tile_map -> y - (SCREEN_H/4 - abs(player1.getY() - tile_map -> y)) >= 0)
      tile_map -> y -= (SCREEN_H/4 - abs(player1.getY() - tile_map -> y));
    else //Close But not quite
      tile_map -> y = 0;
  }
  if(player1.getY() - tile_map -> y > (3 * (SCREEN_H/4)) && tile_map -> y < (tile_map -> height * 64 -  SCREEN_H)){ // DOWN
    if( tile_map -> y + (abs(player1.getY() - tile_map -> y) - (3 * (SCREEN_H/4))) <= (tile_map -> height * 64 -  SCREEN_H))
      tile_map -> y += (abs(player1.getY() - tile_map -> y) - (3 * (SCREEN_H/4)));
    else //Close But not quite
      tile_map -> y = (tile_map -> height * 64 -  SCREEN_H);
  }
  if(player1.getX() - tile_map -> x < (SCREEN_W/4) && tile_map -> x > 0){ // LEFT
    if( tile_map -> x - (SCREEN_W/4 - abs(player1.getX() - tile_map -> x)) >= 0)
      tile_map -> x -= (SCREEN_W/4 - abs(player1.getX() - tile_map -> x));
    else //Close But not quite
      tile_map -> x = 0;
  }
  if(player1.getX() - tile_map -> x > (3 * (SCREEN_W/4)) && tile_map -> x < (tile_map -> width * 64 - SCREEN_W)){ // RIGHT
    if( tile_map -> x + (abs(player1.getX() - tile_map -> x) - (3 * (SCREEN_W/4))) <= (tile_map -> width * 64 - SCREEN_W))
      tile_map -> x += (abs(player1.getX() - tile_map -> x) - (3 * (SCREEN_W/4)));
    else //Close But not quite
      tile_map -> x = (tile_map -> width * 64 - SCREEN_W);
  }

  // Die
  if(player1.getDead()){
    init();
  }

  // Respawn
  if(key[KEY_Y]){
    player1.spawncommand(tile_map);
  }

  // Spawn enemy
  if(key[KEY_R]){
    if( key[KEY_1] || key[KEY_2] || key[KEY_3]){
      if( key[KEY_1]){
        enemy newBadGuy( player1.getX(), player1.getY(), enemy_vorticon);
        newBadGuy.load_images();
        newBadGuy.load_sounds();
        badGuy.push_back(newBadGuy);
      }
      if( key[KEY_2]){
        enemy newBadGuy( player1.getX(), player1.getY(), enemy_robot);
        newBadGuy.load_images();
        newBadGuy.load_sounds();
        badGuy.push_back(newBadGuy);
      }
      if( key[KEY_3]){
        enemy newBadGuy( player1.getX(), player1.getY(), enemy_vorticon);
        newBadGuy.load_images();
        newBadGuy.load_sounds();
        badGuy.push_back(newBadGuy);
      }
      rest( 100);
    }
  }

  // Back to menu
  if( key[KEY_M]){
    set_next_state( STATE_MENU);
  }
}

// Draw game
void Game::draw()
{
  // Black background (just in case)
  rectfill( buffer, 0, 0, 1280, 960, makecol(0,0,0));

  // Draw tiles
  tile_map -> draw_map(buffer);

  // Draw player1
  player1.draw(buffer, tile_map -> x, tile_map -> y);

  // Frame
  rectfill( buffer, 0, 0, SCREEN_W, 16, makecol( 0,0,0));
  rectfill( buffer, 0, 0, 16, SCREEN_H, makecol( 0,0,0));
  rectfill( buffer, SCREEN_W-16, 0, SCREEN_W, SCREEN_H, makecol( 0,0,0));
  rectfill( buffer, 0, SCREEN_H-16, SCREEN_W, SCREEN_H, makecol( 0,0,0));

  // Draw enemies
  for(int i = 0; i < badGuy.size(); i++){
    badGuy.at(i).draw(buffer, tile_map -> x, tile_map -> y);
  }

  // Draw buffer
  stretch_sprite( screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}

// Destroy game
Game::~Game()
{
  destroy_bitmap( buffer);
}
