#include "State.h"

// Change state
void State::setNextState(ProgramState state) {
  this->nextState = state;
}

// Get next state
ProgramState State::getNextState() const {
  return this->nextState;
}