#pragma once

#include <chrono>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
/**
 * @file command.h
 *
 */

/**
 * @brief класс описывающий команду
 */
class Command {

private:
  std::chrono::system_clock::time_point m_current_time_point;
  std::vector<std::string> m_command_list;

public:
  Command() = default;
  Command(size_t a_nBlockSize) { m_command_list.reserve(a_nBlockSize); }
  /**
   * @brief вернуть время "исполнения"
   * @return время, в секундах, исполнения команды
   */
  std::string get_current_time() const {
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
                       m_current_time_point.time_since_epoch())
                       .count();

    return std::to_string(seconds);
  }
  /**
   * @brief проверка, явлется ли вектор входных  команд пустым
   * @return он пустой
   */
  bool is_empty() const { return m_command_list.empty(); }
  /**
   * @brief сколько элементов в векторе команд
   * @return длина вектора
   */
  std::size_t get_length() const { return m_command_list.size(); }

  void clr() { m_command_list.clear(); }

  Command &operator<<(const std::string &a_command_name) {
    if (!a_command_name.empty()) {
      m_command_list.emplace_back(a_command_name);
    }
    if (m_command_list.empty()) {
      m_current_time_point = std::chrono::system_clock::now();
    }
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &a_command_out_stream,
                                  const Command &a_command) {
    for (auto iterator = a_command.m_command_list.begin();
         iterator != a_command.m_command_list.cend(); ++iterator) {
      if (iterator != a_command.m_command_list.begin()) {
        a_command_out_stream << ", ";
      }
      a_command_out_stream << *iterator;
    }
    return a_command_out_stream;
  }
};
