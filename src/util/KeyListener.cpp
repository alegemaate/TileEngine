#include "KeyListener.h"

#include <type_traits>

// For allegro 5, we use events
void KeyListener::onEvent(const ALLEGRO_EVENT_TYPE event_type,
                          const int keyCode) {
  if (event_type == ALLEGRO_EVENT_KEY_DOWN) {
    state[keyCode] = true;
  } else if (event_type == ALLEGRO_EVENT_KEY_UP) {
    state[keyCode] = false;
  }
}

// Check those keys!
void KeyListener::update() {
  // Reset last key
  lastPressed = Key::UNKNOWN;
  lastReleased = Key::UNKNOWN;

  // Check key just pressed
  for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
    // Clear old values
    pressedState.set(i, false);
    releasedState.set(i, false);

    // Pressed since last tick?
    if (state.test(i) == true && previousState.test(i) == false) {
      pressedState.set(i, true);
      lastPressed = static_cast<Key>(i);
    }

    // Released since last tick?
    if (state.test(i) == false && previousState.test(i) == true) {
      releasedState.set(i, true);
      lastReleased = static_cast<Key>(i);
    }
  }

  // Get new values
  for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
    previousState = state;
  }
}

bool KeyListener::wasReleased(const Key key) const {
  const int keyCode = static_cast<std::underlying_type<Key>::type>(key);
  return releasedState.test(keyCode);
}

bool KeyListener::wasPressed(const Key key) const {
  const int keyCode = static_cast<std::underlying_type<Key>::type>(key);
  return pressedState.test(keyCode);
}

bool KeyListener::isDown(const Key key) const {
  const int keyCode = static_cast<std::underlying_type<Key>::type>(key);
  return state.test(keyCode);
}

Key KeyListener::getLastPressed() const {
  return lastPressed;
}

bool KeyListener::isAnyDown() const {
  return state.count() > 0;
}

bool KeyListener::wasAnyPressed() const {
  return pressedState.count() > 0;
}

bool KeyListener::wasAnyReleased() const {
  return releasedState.count() > 0;
}

char KeyListener::toAscii(const Key key) const {
  const int keyCode = static_cast<std::underlying_type<Key>::type>(key);

  // a letter key was pressed; add it to the string
  if (keyCode < asciiMap.size() && keyCode >= 0) {
    if (isDown(Key::LSHIFT) || isDown(Key::RSHIFT)) {
      return asciiShiftMap.at(keyCode);
    } else {
      return asciiMap.at(keyCode);
    }
  }

  // No character
  return '\0';
}