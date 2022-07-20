#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../lib/assets/Bitmap.h"
#include "../lib/assets/Sound.h"
#include "../tile/TileMap.h"
#include "../tools.h"

class Projectile {
 public:
  Projectile(int type, int x, int y, int speed);
  ~Projectile();

  int getX();
  int getY();

  int getType();
  int getContactFrameCounter();
  bool getContact(TileMap* fullMap);

  void update();
  void draw(int xOffset, int yOffset);

 private:
  int x{0};
  int y{0};
  int speed{0};
  int type{0};
  int contactCounter{0};

  bool contact{false};

  Bitmap image[3];
  Sound sound[2];
};

#endif
