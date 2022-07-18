#include "./Logger.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <iostream>

void Logger::log(const std::string& message) {
  std::cout << message << std::endl;
}

void Logger::log(const int message) {
  std::cout << message << std::endl;
}

void Logger::warn(const std::string& message) {
  std::cout << "\033[1;33mWarning: " << message << "\033[0m" << std::endl;
}

void Logger::systemInformation() {
  // Probably never going to be relevant but pretty cool anyways
  uint32_t version = al_get_allegro_version();
  uint32_t major = version >> 24;
  uint32_t minor = (version >> 16) & 255;
  uint32_t revision = (version >> 8) & 255;
  uint32_t release = version & 255;

  // Format allegro version
  Logger::log("Allegro version " + std::to_string(major) + "." +
              std::to_string(minor) + "." + std::to_string(revision) + "." +
              std::to_string(release));

  // This is actually completely irrelevant other than making fun of Allan's PC
  // when he runs this Sorry, your PC is a very nice PC
  int ram = al_get_ram_size();
  Logger::log("Running as with " + std::to_string(ram) + " MB RAM.");
}

void Logger::fatal(const std::string& message) {
  al_show_native_message_box(nullptr, "Error", message.c_str(), message.c_str(),
                             nullptr, ALLEGRO_MESSAGEBOX_ERROR);
  exit(-1);
}