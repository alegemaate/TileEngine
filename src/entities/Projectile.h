#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

#include "../globals.h"
#include "../tools.h"

#include "../tile/TileMap.h"

class Projectile {
 public:
  Projectile(int newType, int newX, int newY, int newSpeed);
  ~Projectile();

  int getX();
  int getY();

  int getType();
  int getContactFrameCounter();
  bool getContact(TileMap* newMap);

  void setType(int newType);
  void setImage(ALLEGRO_BITMAP* image1,
                ALLEGRO_BITMAP* image2,
                ALLEGRO_BITMAP* image3);
  void setSounds(int newType);

  void update();
  void draw(int xOffset, int yOffset);

 private:
  int x;
  int y;
  int speed;
  int type;
  int contact_counter;

  bool contact;

  ALLEGRO_BITMAP* image[3];
  ALLEGRO_SAMPLE* sound[2];
};

#endif
