
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
  if (!currentState) {
    return;
  }

  currentState->draw();
}

// Update
void StateEngine::update(double delta) {
  if (!currentState) {
    return;
  }

  currentState->update(delta);

  // If the state needs to be changed
  if (currentState->getNextState() != ProgramState::NONE) {
    changeState(currentState->getNextState());
  }
}

// Get state id
ProgramState StateEngine::getStateId() const {
  return stateId;
}

// Change game screen
void StateEngine::changeState(ProgramState nextState) {
  // Delete the current state
  if (currentState != nullptr) {
    delete currentState;
  }

  // Change the state
  switch (nextState) {
    case ProgramState::GAME:
      currentState = new Game();
      Logger::log("Switched state to game.");
      break;
    case ProgramState::EDIT:
      currentState = new Editor();
      Logger::log("Switched state to editor.");
      break;
    case ProgramState::INIT:
      currentState = new Init();
      Logger::log("Switched state to init.");
      break;
    case ProgramState::INTRO:
      currentState = new Intro();
      Logger::log("Switched state to intro.");
      break;
    case ProgramState::MENU:
      currentState = new Menu();
      Logger::log("Switched state to menu.");
      break;
    case ProgramState::EXIT:
      Logger::log("Exit state provided, exiting...");
      break;
    default:
      Logger::warn("Unknown state provided.");
      break;
  }

  // Change the current state ID
  stateId = nextState;
}
