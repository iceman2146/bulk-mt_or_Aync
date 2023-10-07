#pragma once
#include <string>
#include <vector>
#include <chrono>
/**
 * @file command.h 
 * 
 */

/**
 * @brief класс описывающий команду
 */ 
class Command
{
private:
  std::string command_name;
  std::chrono::system_clock::time_point current_time;

public:
  Command() = default;
  Command(const std::string &name):command_name{name}, current_time(std::chrono::system_clock::now()) {}
  ~Command() = default;
/**
 * @brief вернуть имя команды
 * @return имя команды
 */ 
  std::string get_name() const
  {
    return command_name;
  }
/**
 * @brief вернуть время "исполнения"
 * @return время, в секундах, исполнения команды
 */ 
  std::string get_current_time() const
  {
    long long seconds = std::chrono::duration_cast<std::chrono::seconds>(
                            current_time.time_since_epoch())
                            .count();

    return std::to_string(seconds);
  }
};
