#ifndef INIT_H
#define INIT_H

#include <allegro5/allegro.h>

#include "./engine/State.h"

class Init : public State {
 public:
  void init() override;
  void update(double delta) override;
  void draw() override;
};

#endif  // INIT_H
