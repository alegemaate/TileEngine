#ifndef INIT_H
#define INIT_H

#include <allegro5/allegro.h>
#include <string>
#include <vector>

#include "./engine/State.h"

#include "../globals.h"
#include "../tools.h"

class Init : public State {
 public:
  Init();
  ~Init();

  void update(StateEngine* engine);
  void draw();
};

#endif  // INIT_H
