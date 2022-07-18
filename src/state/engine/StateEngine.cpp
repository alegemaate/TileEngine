
#include "StateEngine.h"

#include "../../util/Logger.h"
#include "./State.h"

#include "../Editor.h"
#include "../Game.h"
#include "../Init.h"
#include "../Intro.h"
#include "../Menu.h"

// Draw
void StateEngine::draw() {
  if (currentState) {
    currentState->draw();
  }
}

// Update
void StateEngine::update() {
  if (currentState) {
    currentState->update(this);
  }
  changeState();
}

// Set next state
void StateEngine::setNextState(int newState) {
  nextState = newState;
}

// Get state id
int StateEngine::getStateId() {
  return stateId;
}

// Change game screen
void StateEngine::changeState() {
  // If the state needs to be changed
  if (nextState == STATE_NULL) {
    return;
  }

  // Delete the current state
  if (currentState != nullptr) {
    delete currentState;
  }

  // Change the state
  switch (nextState) {
    case STATE_GAME:
      currentState = new Game();
      Logger::log("Switched state to game.");
      break;
    case STATE_EDIT:
      currentState = new Editor();
      Logger::log("Switched state to editor.");
      break;
    case STATE_INIT:
      currentState = new Init();
      Logger::log("Switched state to init.");
      break;
    case STATE_INTRO:
      currentState = new Intro();
      Logger::log("Switched state to intro.");
      break;
    case STATE_MENU:
      currentState = new Menu();
      Logger::log("Switched state to menu.");
      break;
    default:
      currentState = nullptr;
      Logger::log("Exiting program.");
  }

  // Change the current state ID
  stateId = nextState;

  // NULL the next state ID
  nextState = STATE_NULL;
}
