/**
 * @file Bitmap.h
 * @author Allan Legemaate (alegemaate@gmail.com)
 * @brief Bitmap wrapper for ALLEGRO_BITMAPS
 * @version 0.1
 * @date 2022-07-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef UTIL_BITMAP_H_
#define UTIL_BITMAP_H_

#include <allegro5/allegro.h>
#include <memory>
#include <string>

class Bitmap {
 public:
  /**
   * @brief Construct a new Bitmap object
   *
   */
  Bitmap() = default;

  /**
   * @brief Construct a new Bitmap object
   *
   * @param path Path to image file
   */
  Bitmap(const std::string& path);

  /**
   * @brief Destroy the Bitmap object
   *
   */
  ~Bitmap() = default;

  /**
   * @brief Load bitmap from file
   *
   * @param path Path to file
   */
  void load(const std::string& path);

  /**
   * @brief Draw bitmap to screen
   *
   * @param x X position
   * @param y Y position
   */
  void draw(const int x, const int y) const;

  /**
   * @brief Draw scaled bitmap
   *
   * @param x X position
   * @param y Y position
   * @param width Width
   * @param height Height
   */
  void drawScaled(const int x,
                  const int y,
                  const int width,
                  const int height) const;

  /**
   * @brief Get the width of the bitmap
   *
   * @return int width in pixels
   */
  int getWidth() const;

  /**
   * @brief Get the height of the bitmap
   *
   * @return int height in pixels
   */
  int getHeight() const;

 private:
  /**
   * @brief Manged Allegro bitmap
   *
   */
  std::shared_ptr<ALLEGRO_BITMAP> bitmap{nullptr};
};

#endif  // UTIL_BITMAP_H_