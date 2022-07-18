/**
 * Logger class for logging messages
 * Allan Legemaate
 * 15/06/2022
 **/

#ifndef UTIL_LOGGER_H_
#define UTIL_LOGGER_H_

#include <string>

/**
 * @brief Logger class which handles logging, and fatal error dialogues
 *
 */
class Logger {
 public:
  /**
   * @brief Display a simple console log
   *
   * @param message Message to display
   */
  static void log(const std::string& message);

  /**
   * @brief Display a simple console log
   *
   * @param message Message to display
   */
  static void log(const int message);

  /**
   * @brief Display a warning message
   *
   * @param message Message to display
   */
  static void warn(const std::string& message);

  /**
   * @brief Print out system information
   *
   */
  static void systemInformation();

  /**
   * @brief A fatal error has occurred.
   *
   * @param message The message to display in error dialogue.
   */
  static void fatal(const std::string& message);
};

#endif  // UTIL_LOGGER_H_
