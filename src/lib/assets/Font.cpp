#include "./Font.h"

#include "../util/Logger.h"

Font::Font(const std::string& path, const int size) {
  this->load(path, size);
}

void Font::load(const std::string& path, const int size) {
  ALLEGRO_FONT* temp = al_load_ttf_font(path.c_str(), size, 0);
  if (!temp) {
    Logger::fatal("Could not load font: " + path);
  }
  font = std::shared_ptr<ALLEGRO_FONT>(temp, al_destroy_font);
}

void Font::draw(const int x,
                const int y,
                const ALLEGRO_COLOR color,
                const std::string& text) const {
  if (!font) {
    Logger::warn("Font not loaded");
    return;
  }

  al_draw_text(font.get(), color, x, y, 0, text.c_str());
}
