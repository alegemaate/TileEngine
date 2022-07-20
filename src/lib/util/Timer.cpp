#include "./Timer.h"

// Available modes, fit to needs
template double Timer::getElapsedTime<milliseconds>();
template double Timer::getElapsedTime<seconds>();

// Init
Timer::Timer()
    : t1(high_resolution_clock::now()),
      t2(high_resolution_clock::now()),
      running(false) {}

// Start Timer
void Timer::start() {
  t1 = high_resolution_clock::now();
  t2 = high_resolution_clock::now();
  running = true;
}

// Stop
void Timer::stop() {
  running = false;
  t2 = high_resolution_clock::now();
}

// Is running
bool Timer::isRunning() const {
  return running;
}

// Get ms since started
template <typename Precision>
double Timer::getElapsedTime() {
  // Get time now
  if (running) {
    t2 = high_resolution_clock::now();
  }

  // Choose precision
  auto time_diff = duration_cast<Precision>(t2 - t1);

  // Return time as double
  return time_diff.count();
}
