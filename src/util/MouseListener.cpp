#include "MouseListener.h"
#include "DisplayMode.h"

#include <allegro5/allegro.h>
#include <type_traits>

// Check those buttons!
void MouseListener::update() {
  // Get state
  ALLEGRO_MOUSE_STATE state;
  al_get_mouse_state(&state);

  // Position
  mouse_x =
      (state.x - DisplayMode::getTranslationX()) / DisplayMode::getScaleX();
  mouse_y =
      (state.y - DisplayMode::getTranslationY()) / DisplayMode::getScaleY();

  mouse_moved = false;

  if (old_mouse_x != mouse_x || old_mouse_y != mouse_y) {
    mouse_moved = true;
  }

  old_mouse_x = mouse_x;
  old_mouse_y = mouse_y;

  // Check button just pressed
  for (int i = 0; i < 4; i++) {
    // Clear old values
    mouse_pressed &= ~(1 << i);
    mouse_released &= ~(1 << i);

    // Down or not
    const int check_state = ((state.buttons >> i) & 1);
    mouse_button = (mouse_button & ~(1 << i)) | (check_state << i);

    // Pressed since last tick?
    if (((mouse_button >> i) & 1) != 0 && ((mouse_old >> i) & 1) == 0) {
      mouse_pressed |= 1 << i;
    }

    // Released since last tick?
    if (((mouse_button >> i) & 1) == 0 && ((mouse_old >> i) & 1) != 0) {
      mouse_released |= 1 << i;
    }

    // Button changed
    if (((mouse_button >> i) & 1) != ((mouse_old >> i) & 1)) {
      mouse_old ^= 1 << i;
    }
  }
}

int MouseListener::getX() const {
  return mouse_x;
}

int MouseListener::getY() const {
  return mouse_y;
}

bool MouseListener::didMove() const {
  return mouse_moved;
}

bool MouseListener::wasPressed(MouseButton button) const {
  const int buttonCode =
      static_cast<std::underlying_type<MouseButton>::type>(button);
  return (mouse_pressed >> buttonCode) & 1;
}

bool MouseListener::wasReleased(MouseButton button) const {
  const int buttonCode =
      static_cast<std::underlying_type<MouseButton>::type>(button);
  return (mouse_released >> buttonCode) & 1;
}

bool MouseListener::isDown(MouseButton button) const {
  const int buttonCode =
      static_cast<std::underlying_type<MouseButton>::type>(button);
  return (mouse_button >> buttonCode) & 1;
}