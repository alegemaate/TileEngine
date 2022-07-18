#ifndef STATE_STATE_ENGINE_H_
#define STATE_STATE_ENGINE_H_

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
  State* currentState = nullptr;

  // State id
  ProgramState stateId = ProgramState::NONE;
};

#endif  // STATE_STATE_ENGINE_H_