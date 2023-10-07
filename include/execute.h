#pragma once

#include "command.h"
#include "observer.h"
#include "read.h"

#include <memory>
/**
 * @file execute.h 
 * 
 */
class Executer : public IPublisher<std::vector<Command>>, public std::enable_shared_from_this<Executer>
{

private:
  std::ostream &output_stream;
  std::weak_ptr<Reader> reade_ptr;

private:
  Executer(std::ostream &m_output_stream)
      : output_stream(m_output_stream)
  {
  }
  void setup(std::shared_ptr<Reader> &m_reader_ptr)
  {
    reade_ptr = m_reader_ptr;
    auto reade_ptr_shared = reade_ptr.lock();
    if (reade_ptr_shared)
    {
      reade_ptr_shared->subscribe(shared_from_this());
    }
  }

public:

  static std::shared_ptr<Executer> create(std::shared_ptr<Reader> &m_reader_ptr, std::ostream &m_output_stream = std::cout)
  {
    auto ptr = std::shared_ptr<Executer>{new Executer{m_output_stream}};
    ptr->setup(m_reader_ptr);
    return ptr;
  }

  void update(const std::vector<Command> &a_Commands) override
  {
    output_stream << "bulk: ";
    for (auto command_it = a_Commands.begin(); command_it != a_Commands.cend(); ++command_it)
    {
      if (command_it != a_Commands.begin())
      {
        output_stream << ", ";
      }
      output_stream << command_it->get_name();
    }
    output_stream << std::endl;
  }
};
