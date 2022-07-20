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

#include "../lib/assets/Bitmap.h"
#include "../lib/assets/Sound.h"
#include "../lib/input/KeyListener.h"
#include "../lib/state/State.h"
#include "../lib/util/Timer.h"

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
  Intro(KeyListener& keyboardListener);

  /**
   * @brief Init the Intro state
   *
   */
  void init() override;

  /**
   * @brief Update the Intro state
   *
   * @param delta Time since last update
   */
  void update(double delta) override;

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

  /**
   * @brief Keyboard listener provided to skip intro
   *
   */
  KeyListener& keyboardListener;
};

#endif  // INTRO_H
