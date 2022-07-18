/**
 * @file Intro.h
 * @author Allan Legemaate (alegemaate@gmail.com)
 * @brief The intro state which displays the intro animation
 * @version 0.1
 * @date 2022-07-16
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef INTRO_H
#define INTRO_H

#include "./engine/State.h"

#include "../globals.h"
#include "../tools.h"
#include "../util/Bitmap.h"
#include "../util/Sound.h"
#include "../util/Timer.h"

/**
 * @brief Intro state
 *
 */
class Intro : public State {
 public:
  /**
   * @brief Construct a new Intro object
   *
   */
  Intro();

  /**
   * @brief Destroy the Intro object
   *
   */
  ~Intro() = default;

  /**
   * @brief Update the Intro state
   *
   * @param engine Pointer to state engine
   */
  void update(StateEngine* engine) override;

  /**
   * @brief Draw the Intro state
   *
   */
  void draw() override;

 private:
  /**
   * @brief Intro screen background
   *
   */
  Bitmap intro{};

  /**
   * @brief Intro animation images
   *
   */
  Bitmap images[100]{};

  /**
   * @brief Epic intro music
   *
   */
  Sound introSound{};

  /**
   * @brief Timer for managing intro animation
   * @todo Move this to its own system
   *
   */
  Timer timer{};

  /**
   * @brief Current frame of intro animation
   *
   */
  uint8_t frame{0};
};

#endif  // INTRO_H
