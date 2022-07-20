#ifndef TOOLS_H
#define TOOLS_H

#include "./lib/util/BoundingBox.h"

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

extern bool bbCollisionPoint(int x, int y, BoundingBox b);
extern bool bbCollision(BoundingBox b1, BoundingBox b2);

// Random number generator
extern int random(int newLowest, int newHighest);

#endif  // TOOLS_H
