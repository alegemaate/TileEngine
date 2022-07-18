#ifndef STATE_STATE_ENGINE_H_
#define STATE_STATE_ENGINE_H_

class State;

class StateEngine {
 public:
  // Update
  void update();

  // Draw
  void draw();

  // Set next state
  void setNextState(int newState);

  // Get state id
  int getStateId();

  // Game states
  enum programStates {
    STATE_NULL,
    STATE_INIT,
    STATE_INTRO,
    STATE_MENU,
    STATE_EDIT,
    STATE_GAME,
    STATE_EXIT,
  };

 private:
  // Change state
  void changeState();

  // Current state object
  State* currentState = nullptr;

  // Next state
  int nextState = STATE_NULL;

  // State id
  int stateId = STATE_NULL;
};

#endif  // STATE_STATE_ENGINE_H_