/*
 * Timer.h
 * A simple, cross platform timer class
 *  based on chrono
 * Allan Legemaate
 * 25-03-2019
 */

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

using namespace std::chrono;

// Timer class
class Timer {
 public:
  // Create timer
  Timer();

  // Start time
  void start();

  // Stop
  void stop();

  // Is running
  bool isRunning() const;

  // Get ms since started
  template <typename Precision>
  double getElapsedTime();

 private:
  // Holds time points for start and end
  time_point<high_resolution_clock> t1, t2;
  bool running;
};

#endif  // TIMER_H
