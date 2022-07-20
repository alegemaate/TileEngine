/**
 * State for machine and State Engine
 * Allan Legemaate
 * 30/12/2016
 * Compartmentalize program into states
 *   which can handle only their own logic,
 *   drawing and transitions
 */

#ifndef STATE_STATE_H_
#define STATE_STATE_H_

#include "./StateEngine.h"

class State {
 public:
  // Virtual destructor
  virtual ~State(){};

  // Initialise state
  virtual void init() = 0;

  // Draw to screen
  virtual void draw() = 0;

  // Update logic
  virtual void update(double delta) = 0;

  // Change state
  void setNextState(ProgramState state);

  // Get next state
  ProgramState getNextState() const;

 private:
  ProgramState nextState = ProgramState::NONE;
};

#endif  // STATE_STATE_H_
