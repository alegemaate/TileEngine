#include "Init.h"

Init::Init() {}

Init::~Init() {}

void Init::update(StateEngine* engine) {
  // Change to splash screen
  engine->setNextState(StateEngine::STATE_INTRO);
}

void Init::draw() {}
