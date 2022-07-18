#ifndef TOOLS_H
#define TOOLS_H

#include <allegro5/allegro.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Collision
extern bool collisionAny(int xMin1,
                         int xMax1,
                         int xMin2,
                         int xMax2,
                         int yMin1,
                         int yMax1,
                         int yMin2,
                         int yMax2);
extern bool collisionBottom(int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionTop(int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionRight(int xMin1, int xMax1, int xMin2, int xMax2);
extern bool collisionLeft(int xMin1, int xMax1, int xMin2, int xMax2);

// Checks if file exists
extern bool fexists(const char* filename);

// Random number generator
extern int random(int newLowest, int newHighest);

#endif  // TOOLS_H
