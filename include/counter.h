#pragma once

#include <cstddef>
#include <iostream>
#include <string>

class Counters {

public:
  std::string m_name;
  std::size_t m_lines;
  std::size_t m_block_commands;
  std::size_t m_commands;

public:
  Counters(std::string a_cmd_name)
      : m_name{a_cmd_name}, m_lines{0}, m_block_commands{0}, m_commands{0} {}

  friend std::ostream &operator<<(std::ostream &a_command_out_stream,
                                  const Counters &a_counters) {
    a_command_out_stream << a_counters.m_name << ": ";

    if (a_counters.m_lines > 0) {
      a_command_out_stream << a_counters.m_lines << "строк комманд, ";
    }
    a_command_out_stream << a_counters.m_block_commands << " количество блоков комманд, "
                         << a_counters.m_commands << " количество комманд";

    return a_command_out_stream;
  }
};
