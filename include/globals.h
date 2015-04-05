#ifndef GLOBALS_H
#define GLOBALS_H

//All libaries
#include <allegro.h>

//All defined presets put here
//tile Attributes
#define gas 0
#define solid 1
#define liquid 2
#define climb 3
#define harmful 4
#define interactive 5
#define item 6
#define spawn 7
#define half_block_top 8
#define half_block_bottom 9
#define quarter_block_top 10
#define quarter_block_bottom 11
#define light 12

//Character directions
// 0-3 left, 4-7 right, 8-11 up
#define LEFT 0
#define RIGHT 4
#define UP 8

//Tiles
#define tile_air 0
#define tile_blue_top 1
#define tile_blue_bottom 2
#define tile_brick_blue 3
#define tile_brick_red 4
#define tile_lollypop 5
#define tile_lava 6

#define tile_light_pillar 7
#define tile_zapper 8
#define tile_brick_yellow 9
#define tile_brick_green 10
#define tile_green_light 11
#define tile_red_light 12
#define tile_id 13
#define tile_old_vorticon 14
#define tile_happy 15
#define tile_finish 16

//Background Tiles
#define tile_back_grey 100

#define tile_back_grey_shadow_top 101
#define tile_back_grey_shadow_top_right 102
#define tile_back_grey_shadow_top_left 103
#define tile_back_grey_shadow_left 104
#define tile_back_grey_shadow_bottom_left 105
#define tile_back_grey_shadow_top_left_corner 106

#define tile_back_grey_dark 107
#define tile_back_grey_dark_top_left 108
#define tile_back_grey_dark_top_right 109

#define tile_back_lamp 110

//Spawners
#define tile_spawn_player 199
#define tile_spawn_vorticon 200
#define tile_spawn_robot 201
#define tile_spawn_danny 202

//Enemies
#define enemy_vorticon 0
#define enemy_robot 1
#define enemy_danny 2

extern BITMAP* tileSprites[400][8];
extern BITMAP* projectileSprites[10][3];

// Set next state
void set_next_state( int newState );

//Game states
enum GameStates
{
    STATE_NULL,
    STATE_INIT,
    STATE_INTRO,
    STATE_MENU,
    STATE_EDIT,
    STATE_GAME,
    STATE_EXIT,
};

// Frames
extern int frames_done;

// Resdiv
extern int resDiv;
extern bool single_player;

//State variables
extern int stateID;
extern int nextState;
extern int levelOn;

#endif
