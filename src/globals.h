#ifndef GLOBALS_H
#define GLOBALS_H

// All libraries
#include <allegro5/allegro.h>

// All defined presets put here
// tile Attributes
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
#define finish 13

// Character directions
//  0-3 left, 4-7 right, 8-11 up
#define CHARACTER_LEFT 0
#define CHARACTER_RIGHT 4
#define CHARACTER_UP 8

// Enemies
#define enemy_vorticon 0
#define enemy_robot 1
#define enemy_danny 2

// Projectile images (shouldn't be here :S)
extern ALLEGRO_BITMAP* projectileSprites[10][3];
extern int levelOn;

#endif
