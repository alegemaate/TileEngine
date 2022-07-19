/**
 * Key Listener
 * Allan Legemaate
 * Listens for keys JUST pressed or JUST released
 *   since the last tick
 * 18/01/2017
 **/

#ifndef KEYLISTENER_H
#define KEYLISTENER_H

#include <allegro5/allegro.h>
#include <array>
#include <bitset>
#include <string>

#include "./Key.h"

class KeyListener {
 public:
  /**
   * @brief Process allegro event
   *
   * @param type Event type
   * @param keyCode Code of key that changed state
   */
  void onEvent(const ALLEGRO_EVENT_TYPE type, const int key);

  /**
   * @brief Update key states
   *
   */
  void update();

  /**
   * @brief Check if key is dowm
   *
   * @param key Key to check
   * @return true Key is pressed
   * @return false Key is not pressed
   */
  bool isDown(const Key key) const;

  /**
   * @brief Check if key is just pressed
   *
   * @param key Key to check
   * @return true Key is just pressed
   * @return false Key is not just pressed
   */
  bool wasPressed(const Key key) const;

  /**
   * @brief Check if key is just released
   *
   * @param key Key to check
   * @return true Key is just released
   * @return false Key is not just released
   */
  bool wasReleased(const Key key) const;

  /**
   * @brief Get last key pressed
   *
   * @return int Last key pressed
   */
  Key getLastPressed() const;

  /**
   * @brief Check if any key pressed
   *
   * @return true Any key pressed
   * @return false No key pressed
   */
  bool isAnyDown() const;

  /**
   * @brief Check if any key just pressed
   *
   * @return true Any key just pressed
   * @return false No key just pressed
   */
  bool wasAnyPressed() const;

  /**
   * @brief Check if any key just released
   *
   * @return true Any key just released
   * @return false No key just released
   */
  bool wasAnyReleased() const;

  /**
   * @brief Convert key code to ascii
   *
   * @param key Key code to convert
   * @return char Ascii character, empty string if not a character
   */
  char toAscii(const Key key) const;

 private:
  /// If a key is down or not
  std::bitset<256> state{false};

  /// If a key was just pressed or not
  std::bitset<256> pressedState{false};

  /// If a key was just released or not
  std::bitset<256> releasedState{false};

  /// Key state from last tick
  std::bitset<256> previousState{false};

  /// Last key pressed
  Key lastPressed{Key::UNKNOWN};

  /// Last key released
  Key lastReleased{Key::UNKNOWN};

  /// Ascii lookup
  const std::array<char, 76> asciiMap{
      '\0', 'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',
      'k',  'l',  'm',  'n',  'o',  'p',  'q',  'r',  's',  't',  'u',
      'v',  'w',  'x',  'y',  'z',  '0',  '1',  '2',  '3',  '4',  '5',
      '6',  '7',  '8',  '9',  '0',  '1',  '2',  '3',  '4',  '5',  '6',
      '7',  '8',  '9',  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
      '\0', '\0', '\0', '\0', '\0', '`',  '-',  '=',  '\0', '\t', '[',
      ']',  '\0', ';',  '\'', '\\', '\0', ',',  '.',  '/',  ' '};

  /// Modifier ascii lookup
  const std::array<char, 76> asciiShiftMap{
      '\0', 'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',
      'K',  'L',  'M',  'N',  'O',  'P',  'Q',  'R',  'S',  'T',  'U',
      'V',  'W',  'X',  'Y',  'Z',  ')',  '!',  '@',  '#',  '$',  '%',
      '^',  '&',  '*',  ')',  '\0', '\0', '\0', '\0', '\0', '\0', '\0',
      '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
      '\0', '\0', '\0', '\0', '\0', '~',  '_',  '+',  '\0', '\t', '{',
      '}',  '\0', ':',  '\'', '|',  '\0', '<',  '>',  '?',  '\0'};
};

#endif  // KEYLISTENER_H
