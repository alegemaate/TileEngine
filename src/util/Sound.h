/**
 * @file Sound.h
 * @author Allan Legemaate (alegemaate@gmail.com)
 * @brief Allegro Sample wrapper
 * @version 0.1
 * @date 2022-07-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef UTIL_Sound_H_
#define UTIL_Sound_H_

#include <allegro5/allegro_audio.h>
#include <memory>
#include <string>

class Sound {
 public:
  /**
   * @brief Construct a new Sound object
   *
   */
  Sound() = default;

  /**
   * @brief Construct a new Sound object
   *
   * @param path Path to image file
   */
  Sound(const std::string& path);

  /**
   * @brief Destroy the Sound object
   *
   */
  ~Sound() = default;

  /**
   * @brief Load sound from file
   *
   * @param path Path to file
   */
  void load(const std::string& path);

  /**
   * @brief Play a sample
   *
   * @param volume Volume to play at
   * @param panning Panning to play at
   * @param speed Speed to play at
   * @param loop Whether to loop or not
   */
  void play(const float volume = 1.0f,
            const float panning = 0.0f,
            const float speed = 1.0f,
            const bool loop = false) const;

 private:
  /**
   * @brief Manged Allegro sample
   *
   */
  std::shared_ptr<ALLEGRO_SAMPLE> sample{nullptr};
};

#endif  // UTIL_SAMPLE_H_