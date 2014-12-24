#ifndef TILE_H
#define TILE_H

#include <allegro.h>
#include <alpng.h>

#include "globals.h"
#include "tools.h"

class tile{
  public:
    tile(int newType);
    ~tile();

    int getX();
    int getY();

    void setX(int newX);
    void setY(int newY);

    int getType();
    int getAttribute();

    BITMAP* getImage();

    void setType(int newType);

    void setImages(BITMAP* image1);
    void setImagesAnimated(BITMAP* image1, BITMAP* image2, BITMAP* image3, BITMAP* image4);

    void draw_tile(BITMAP* temp, int xOffset, int yOffset, int frame);

  private:
    int x;
    int y;
    int type;
    int attribute;

    bool animated;

    BITMAP* images[4];
};

#endif
