#include "Projectile.h"

Projectile::Projectile(int type, int x, int y, int speed)
    : x(x), y(y), speed(speed), type(type) {
  sound[0] = Sound("sounds/shoot.wav");
  sound[1] = Sound("sounds/shot_hit.wav");

  image[0] = Bitmap("images/laser.png");
  image[1] = Bitmap("images/laser.png");
  image[2] = Bitmap("images/laser_hit.png");

  sound[0].play();
}

Projectile::~Projectile() {}

// Getters/ setters
int Projectile::getX() {
  return x;
}

int Projectile::getY() {
  return y;
}

int Projectile::getType() {
  return type;
}

int Projectile::getContactFrameCounter() {
  return contactCounter;
}

bool Projectile::getContact(TileMap* fullMap) {
  // Check for collision
  for (auto& tile : fullMap->mapTiles) {
    if (tile->hasAttribute(TileAttribute::SOLID)) {
      BoundingBox bounds{x, y, image[0].getWidth(), image[0].getHeight()};
      if (tile->getBounds().collides(bounds)) {
        contact = true;
      }
    }
  }

  if (!contact) {
    if (x < 0 || x > fullMap->width * 64 || y < 0 || y > fullMap->height * 64) {
      contact = true;
    }
  } else {
    if (contactCounter == 0) {
      sound[1].play();
      speed = 0;
    } else if (contactCounter > 9) {
      contactCounter = 10;
    }
    contactCounter++;
  }

  return contact;
}

// Update
void Projectile::update() {
  x += speed;
}

// Draw projectile
void Projectile::draw(int xOffset, int yOffset) {
  if (!contact) {
    image[0].draw(x - xOffset, y - yOffset);
  } else {
    image[2].draw(x - xOffset, y - yOffset);
  }
}
