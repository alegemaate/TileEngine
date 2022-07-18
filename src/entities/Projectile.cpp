#include "Projectile.h"

Projectile::Projectile(int newType, int newX, int newY, int newSpeed) {
  x = newX;
  y = newY;
  speed = newSpeed;
  type = 0;
  contact_counter = 0;
  contact = false;
  setType(newType);
  setSounds(newType);
  al_play_sample(sound[0], 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
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
  return contact_counter;
}

bool Projectile::getContact(TileMap* newMap) {
  // Check for collision
  for (uint32_t i = 0; i < newMap->mapTiles.size(); i++) {
    if (newMap->mapTiles.at(i).containsAttribute(solid)) {
      if (collisionAny(x, x + al_get_bitmap_width(image[0]),
                       newMap->mapTiles.at(i).getX(),
                       newMap->mapTiles.at(i).getX() + 64, y,
                       y + al_get_bitmap_height(image[0]),
                       newMap->mapTiles.at(i).getY(),
                       newMap->mapTiles.at(i).getY() + 64)) {
        contact = true;
      }
    }
  }

  if (!contact) {
    if (x < 0 || x > newMap->width * 64 || y < 0 || y > newMap->height * 64) {
      contact = true;
    }
  } else {
    if (contact_counter == 0) {
      al_play_sample(sound[1], 1.0f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
      speed = 0;
    } else if (contact_counter > 9) {
      contact_counter = 10;
    }
    contact_counter++;
  }
  return contact;
}

// Set type
void Projectile::setType(int newType) {
  type = newType;

  // Sets images
  setImage(projectileSprites[type][0], projectileSprites[type][1],
           projectileSprites[type][2]);
}

// Set tiles images
void Projectile::setImage(ALLEGRO_BITMAP* image1,
                          ALLEGRO_BITMAP* image2,
                          ALLEGRO_BITMAP* image3) {
  image[0] = image1;
  image[1] = image2;
  image[2] = image3;
}

// Set Images
void Projectile::setSounds(int newType) {
  if (newType == 0) {
    sound[0] = al_load_sample("sounds/shoot.wav");
    sound[1] = al_load_sample("sounds/shot_hit.wav");
  }
}

// Update
void Projectile::update() {
  x += speed;
}

// Draw projectile
void Projectile::draw(int xOffset, int yOffset) {
  if (!contact) {
    al_draw_bitmap(image[0], x - xOffset, y - yOffset, 0);
  } else {
    al_draw_bitmap(image[2], x - xOffset, y - yOffset, 0);
  }
}
