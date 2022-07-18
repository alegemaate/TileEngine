#include "Intro.h"

#include <allegro5/allegro_primitives.h>
#include <string>

#include "../util/DisplayMode.h"
#include "../util/KeyListener.h"

Intro::Intro() {
  // Sound
  introSound = Sound("sounds/intro.wav");

  // Load background
  intro = Bitmap("images/title.png");

  std::string fileName;
  for (int i = 0; i < 63; i++) {
    fileName = "images/intro/frame_" + std::to_string(i) + ".png";
    images[i] = Bitmap(fileName.c_str());
  }

  // Sound
  introSound.play();

  // Start timer
  timer.start();
}

void Intro::update(double delta) {
  // Set animation frame
  frame = timer.getElapsedTime<milliseconds>() / 100;

  // Wait and then go to the menu
  if (frame > 100 || KeyListener::keyPressed[ALLEGRO_KEY_SPACE]) {
    setNextState(ProgramState::MENU);
  }
}

void Intro::draw() {
  // Intro stuffs
  const int screenHeight = DisplayMode::getDrawHeight();
  const int screenWidth = DisplayMode::getDrawWidth();

  al_draw_filled_rectangle(0, 0, screenWidth, screenHeight,
                           al_map_rgb(0, 0, 0));

  if (frame < 90) {
    int index = 0;
    if (frame < 63) {
      index = frame;
    } else if (frame < 70) {
      index = 62;
    } else if (frame < 80) {
      index = 62 - (frame - 70) * 6;
    } else if (frame < 90) {
      index = 0;
    }

    images[index].drawScaled(0, 0, screenWidth, screenHeight);
  } else {
    intro.draw(0, 0);
  }
}
