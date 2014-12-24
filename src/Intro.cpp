#include "Intro.h"

Intro::Intro()
{
  // Load background
  background = load_bitmap( "images/title.png", NULL);
}

void Intro::update()
{
  // Wait 2 seconds then go to the menu
  rest(2000);
  set_next_state(STATE_GAME);
}

void Intro::draw()
{
  // Show background
  draw_sprite( screen, background, 0, 0);
}

Intro::~Intro()
{
  // Clear memory
  destroy_bitmap( background);
}
