/**
 * @file Font.h
 * @author Allan Legemaate (alegemaate@gmail.com)
 * @brief Font wrapper for ALLEGRO_FONTs
 * @version 0.1
 * @date 2022-07-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef UTIL_FONT_H_
#define UTIL_FONT_H_

#include <allegro5/allegro_ttf.h>
#include <memory>
#include <string>

class Font {
 public:
  /**
   * @brief Construct a new Font object
   *
   */
  Font() = default;

  /**
   * @brief Construct a new Font object
   *
   * @param path Path to font file
   * @param size Font size
   */
  Font(const std::string& path, const int size);

  /**
   * @brief Destroy the Font object
   *
   */
  ~Font() = default;

  /**
   * @brief Load font from file
   *
   * @param path Path to font file
   * @param size Font size
   */
  void load(const std::string& path, const int size);

  /**
   * @brief Draw font to screen
   *
   * @param x X position
   * @param y Y position
   * @param color Color
   * @param text Text to draw
   */
  void draw(const int x,
            const int y,
            const ALLEGRO_COLOR color,
            const std::string& text) const;

 private:
  /**
   * @brief Manged Allegro font
   *
   */
  std::shared_ptr<ALLEGRO_FONT> font{nullptr};
};

#endif  // UTIL_FONT_H_