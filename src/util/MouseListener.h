/**
 * Mouse Listener
 * Allan Legemaate
 * Listens for mouse buttons JUST pressed or JUST released
 *   since the last tick
 * 18/01/2017
 **/

#ifndef MOUSELISTENER_H
#define MOUSELISTENER_H

#include "MouseButton.h"

class MouseListener {
 public:
  /**
   * @brief Update mouse states since last tick
   *
   */
  void update();

  /**
   * @brief Get mouse X position
   *
   * @return int mouse x
   */
  int getX() const;

  /**
   * @brief Get mouse Y position
   *
   * @return int mouse y
   */
  int getY() const;

  /**
   * @brief Did the mouse move since last frame?
   *
   * @return true Mouse moved
   * @return false Mouse did not move
   */
  bool didMove() const;

  /**
   * @brief Check if mouse was pressed
   *
   * @param button Button to check
   * @return true Button was just pressed
   * @return false Button was not just pressed
   */
  bool wasPressed(MouseButton button) const;

  /**
   * @brief Check if mouse was released
   *
   * @param button Button to check
   * @return true Button was just released
   * @return false Button was not just released
   */
  bool wasReleased(MouseButton button) const;

  /**
   * @brief Check if mouse button is down
   *
   * @param button Button to check
   * @return true Button is down
   * @return false Button is not down
   */
  bool isDown(MouseButton button) const;

 private:
  int mouse_x{0};
  int mouse_y{0};

  int old_mouse_x{0};
  int old_mouse_y{0};

  unsigned char mouse_button{0};
  unsigned char mouse_pressed{0};
  unsigned char mouse_released{0};
  unsigned char mouse_old{0};

  bool mouse_moved{false};
};

#endif  // MOUSELISTENER_H
