#include "State.h"

#include "../../util/Logger.h"

/*********
 * STATE
 *********/

// Change state
void State::setNextState(StateEngine* engine, int state) {
  engine->setNextState(state);
}
