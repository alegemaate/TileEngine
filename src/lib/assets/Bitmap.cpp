#include "./Bitmap.h"

#include "../util/Logger.h"

Bitmap::Bitmap(const std::string& path) {
  this->load(path);
}

void Bitmap::load(const std::string& path) {
  ALLEGRO_BITMAP* temp = al_load_bitmap(path.c_str());
  if (!temp) {
    Logger::fatal("Could not load bitmap: " + path);
  }
  bitmap = std::shared_ptr<ALLEGRO_BITMAP>(temp, al_destroy_bitmap);
}

void Bitmap::draw(const int x, const int y) const {
  if (!bitmap) {
    Logger::warn("Bitmap not loaded");
    return;
  }

  al_draw_bitmap(bitmap.get(), x, y, 0);
}

void Bitmap::drawScaled(const int x,
                        const int y,
                        const int width,
                        const int height) const {
  if (!bitmap) {
    Logger::warn("Bitmap not loaded");
    return;
  }

  const int bitmapWidth = al_get_bitmap_width(bitmap.get());
  const int bitmapHeight = al_get_bitmap_height(bitmap.get());

  al_draw_scaled_bitmap(bitmap.get(), 0, 0, bitmapWidth, bitmapHeight, x, y,
                        width, height, 0);
}

int Bitmap::getWidth() const {
  if (!bitmap) {
    Logger::warn("Bitmap not loaded");
    return 0;
  }

  return al_get_bitmap_width(bitmap.get());
}

int Bitmap::getHeight() const {
  if (!bitmap) {
    Logger::warn("Bitmap not loaded");
    return 0;
  }

  return al_get_bitmap_height(bitmap.get());
}