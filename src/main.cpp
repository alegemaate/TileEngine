/**
 * Main
 * Allan Legemaate and Danny Van Stemp
 * This is the main for the game.
 * Calls state machine update and draw functions
 * 09/05/2017
 **/
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "lib/display/DisplayMode.h"
#include "lib/state/StateEngine.h"
#include "lib/util/Logger.h"
#include "tools.h"

// FPS system variables
double t = 0.0;
const double dt = 0.01;

double currentTime = 0.0;
double accumulator = 0.0;

// Closing or naw
bool closing = false;

// Allegro events
ALLEGRO_EVENT_QUEUE* eventQueue = nullptr;
ALLEGRO_DISPLAY* display = nullptr;
ALLEGRO_BITMAP* buffer;

// State engine
StateEngine stateManager;

// Setup game
void setup() {
  Logger::log("Initializing Allegro.");

  // Init allegro
  if (!al_init()) {
    Logger::fatal("Allegro could not initilize");
  }

  // Window title
  al_set_window_title(display, "Loading...");

  // Controls
  al_install_keyboard();
  al_install_mouse();
  al_install_joystick();

  // GFX addons
  al_init_image_addon();
  al_init_primitives_addon();

  // Font
  al_init_ttf_addon();

  // Audio
  al_install_audio();
  al_init_acodec_addon();
  al_reserve_samples(20);

  // Set display mode to windowed
  DisplayMode::setActiveDisplay(&display);
  DisplayMode::setMode(DisplayMode::WINDOWED);
  buffer = al_create_bitmap(DisplayMode::getDrawWidth(),
                            DisplayMode::getDrawHeight());

  if (!display) {
    Logger::fatal("Screen could not be created");
  }

  // Register events
  eventQueue = al_create_event_queue();
  al_register_event_source(eventQueue, al_get_display_event_source(display));
  al_register_event_source(eventQueue, al_get_keyboard_event_source());
  al_register_event_source(eventQueue, al_get_joystick_event_source());

  // Window title
  al_set_window_title(display, "Tile Engine");

  Logger::systemInformation();

  currentTime = al_get_time();
}

// Handle events
void processEvents() {
  ALLEGRO_EVENT ev;
  bool hasEvent = al_get_next_event(eventQueue, &ev);

  while (hasEvent) {
    // Exit
    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      closing = true;
    }

    // Keyboard
    else {
      stateManager.processEvent(ev);
    }

    // Get next event
    hasEvent = al_get_next_event(eventQueue, &ev);
  }
}

// Update and render
void update() {
  const double newTime = al_get_time();
  const double frameTime = newTime - currentTime;
  currentTime = newTime;

  accumulator += frameTime;

  while (accumulator >= dt) {
    // Drain event queue
    processEvents();

    // Update state
    stateManager.update(dt);

    // Exit
    if (stateManager.getStateId() == ProgramState::EXIT) {
      closing = true;
    }

    // Update timestep
    accumulator -= dt;
    t += dt;
  }

  // Drawing
  // Render a frame
  al_set_target_bitmap(buffer);
  al_clear_to_color(al_map_rgb(0, 0, 0));
  stateManager.draw();

  al_set_target_backbuffer(display);
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_scaled_bitmap(
      buffer, 0, 0, DisplayMode::getDrawWidth(), DisplayMode::getDrawHeight(),
      DisplayMode::getTranslationX(), DisplayMode::getTranslationY(),
      DisplayMode::getScaleWidth(), DisplayMode::getScaleHeight(), 0);

  // Flip (OpenGL)
  al_flip_display();
}

// Start here
int main(int argc, char** argv) {
  // Ignore argc, argv
  (void)argc;
  (void)argv;

  // Basic init
  setup();

  // Set the current state ID
  stateManager.changeState(ProgramState::MENU);

  // Run game
  while (!closing) {
    update();
  }

  return 0;
}
