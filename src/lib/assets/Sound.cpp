#include "./Sound.h"

#include "../util/Logger.h"

Sound::Sound(const std::string& path) {
  this->load(path);
}

void Sound::load(const std::string& path) {
  ALLEGRO_SAMPLE* temp = al_load_sample(path.c_str());
  if (!temp) {
    Logger::fatal("Could not load sample: " + path);
  }
  sample = std::shared_ptr<ALLEGRO_SAMPLE>(temp, al_destroy_sample);
}

void Sound::play(const float volume,
                 const float panning,
                 const float speed,
                 const bool loop) const {
  if (!sample) {
    Logger::warn("Sample not loaded");
    return;
  }

  al_play_sample(sample.get(), volume, panning, speed,
                 loop ? ALLEGRO_PLAYMODE_LOOP : ALLEGRO_PLAYMODE_ONCE, 0);
}