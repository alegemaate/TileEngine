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

  // Draw to screen
  virtual void draw() = 0;

  // Update logic
  virtual void update(StateEngine* engine) = 0;

  // Change state
  void setNextState(StateEngine* engine, int state);
};

#endif  // STATE_STATE_H_
