#ifndef INIT_H
#define INIT_H

#include <allegro5/allegro.h>

#include "./engine/State.h"

class Init : public State {
 public:
  Init() = default;
  ~Init() = default;

  void update(double delta);
  void draw();
};

#endif  // INIT_H
