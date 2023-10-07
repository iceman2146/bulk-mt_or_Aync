#pragma once

#include "observer.h"
#include "command.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <string>
/**
 * @file read.h 
 * 
 */

/**
 * @brief класс "читатель", реализующий модель подписчика
 */ 
class Reader : public BaseSubscriber<std::vector<Command>>
{
private:
  std::istream &input_stream;
  std::size_t block_size;
  std::size_t nested_counter;
  std::vector<Command> command_vector;

private:
  Reader(std::size_t m_block_size, std::istream &m_input_stream = std::cin)
      : input_stream(m_input_stream), block_size(m_block_size), nested_counter(0)
  {
  }
  void clear_input()
  {
    if (command_vector.size() > 0)
    {
      notify(command_vector);
      command_vector.clear();
    }
  }

public:
  static std::shared_ptr<Reader> create(std::size_t m_block_size, std::istream &m_input_stream = std::cin)
  {
    auto ptr = std::shared_ptr<Reader>{new Reader{m_block_size, m_input_stream}};
    return ptr;
  }

  void enter()
  {
    command_vector.reserve(block_size);

    std::string cmd_line;
    while (std::getline(input_stream, cmd_line))
    {
      if (cmd_line == "{")
      {
        if (nested_counter == 0)
        {
          clear_input();
        }
        nested_counter++;
      }
      else if (cmd_line == "}")
      {
        if (nested_counter == 0)
        {
          throw std::logic_error("unexpected symbol '}'");
        }
        nested_counter--;
      }
      else
      {
        command_vector.emplace_back(Command(cmd_line));
      }

      if (!nested_counter && command_vector.size() >= block_size)
      {
        clear_input();
      }
    }
  }
};
