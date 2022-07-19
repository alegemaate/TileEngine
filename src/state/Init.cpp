#include "Init.h"

void Init::init() {}

void Init::update(double delta) {
  // Change to splash screen
  setNextState(ProgramState::INTRO);
}

void Init::draw() {}
