#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <allegro.h>
#include <alpng.h>

#include "globals.h"
#include "tools.h"

#include "tileMap.h"


class projectile{
  public:
    projectile(int newType, float newX, float newY, float newXSpeed, float newYSpeed);
    ~projectile();

    int getX();
    int getY();

    int getType();
    int getContactFrameCounter();
    bool getContact(tileMap *fullMap);

    void setType(int newType);
    void setImage(BITMAP* image1, BITMAP* image2, BITMAP* image3);
    void setSounds( int newType);

    void update();
    void draw(BITMAP* temp, int xOffset, int yOffset);

  private:
    float x;
    float y;
    float x_speed;
    float y_speed;

    float angle;

    int type;
    int contact_counter;

    tileMap *newMap;

    bool contact;

    BITMAP* image[3];
    SAMPLE* sound[2];
};

#endif
