#ifndef GLOBALS_H
#define GLOBALS_H

// All libraries
#include <allegro5/allegro.h>

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
