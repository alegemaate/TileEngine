#ifndef STATE_STATE_ENGINE_H_
#define STATE_STATE_ENGINE_H_

#include "../../util/JoystickListener.h"
#include "../../util/KeyListener.h"
#include "../../util/MouseListener.h"

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
  // Process events
  void processEvent(const ALLEGRO_EVENT& event);

  // Update
  void update(double delta);

  // Draw
  void draw();

  // Get state id
  ProgramState getStateId() const;

  // Change state
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