#ifndef STATE_STATE_ENGINE_H_
#define STATE_STATE_ENGINE_H_

#include "../../lib/input/JoystickListener.h"
#include "../../lib/input/KeyListener.h"
#include "../../lib/input/MouseListener.h"

class State;

// Game states
enum class ProgramState {
  NONE,
  INIT,
  INTRO,
  MENU,
  EDIT,
  GAME,
  EXIT,
};

class StateEngine {
 public:
  /**
   * @brief Process allegro events
   *
   * @param event Allegro event
   */
  void processEvent(const ALLEGRO_EVENT& event);

  /**
   * @brief Update the state
   *
   * @param delta Time passed since last update
   */
  void update(double delta);

  /**
   * @brief Draw state
   *
   */
  void draw();

  /**
   * @brief Get the active State Id
   *
   * @return ProgramState Active state id
   */
  ProgramState getStateId() const;

  /**
   * @brief Change the state
   *
   * @param nextState Next program state id
   */
  void changeState(ProgramState nextState);

 private:
  // Current state object
  State* currentState{nullptr};

  // State id
  ProgramState stateId{ProgramState::NONE};

  // Listeners
  KeyListener keyboardListener{};
  MouseListener mouseListener{};
  JoystickListener joystickListener{};
};

#endif  // STATE_STATE_ENGINE_H_