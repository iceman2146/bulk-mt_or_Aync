#pragma once

#include "command.h"
#include "observer.h"
#include "read.h"

#include <memory>
#include <fstream>
/**
 * @file log.h 
 * 
 */

 /**
 * @brief класс для вывода логов входных команд
 */ 
class Logger : public IPublisher<std::vector<Command>>, public std::enable_shared_from_this<Logger>
{
private:
  std::weak_ptr<Reader> reade_ptr;

private:
  Logger() {}

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
  static std::shared_ptr<Logger> create(std::shared_ptr<Reader> &m_reader_ptr)
  {
    auto ptr = std::shared_ptr<Logger>{new Logger{}};
    ptr->setup(m_reader_ptr);
    return ptr;
  }

  void update(const std::vector<Command> &a_Commands) override
  {
    std::string strLogName{"bulk" + a_Commands.begin()->get_current_time() + ".log"};
    std::ofstream log(strLogName, std::ios::out);

    log << "bulk: ";
    for (auto it = a_Commands.begin(); it != a_Commands.end(); ++it)
    {
      if (it != a_Commands.begin())
      {
        log << ", ";
      }
      log << it->get_name();
    }
    log << std::endl;
    log.close();
  }
};
