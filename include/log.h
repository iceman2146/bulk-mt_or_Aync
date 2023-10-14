#pragma once

#include "command.h"
#include "commander.h"
#include "counter.h"
#include "observer.h"

#include <atomic>
#include <condition_variable>
#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

class Logger : public IPublisher<Command>,
               public std::enable_shared_from_this<Logger> {

private:
  void *m_context;

  std::weak_ptr<Commander> m_command_ptr;
  std::ostream &m_stream_out;

  std::vector<std::thread> m_threads;
  std::atomic<bool> m_is_done;

  std::mutex m_mutex_queue;
  std::mutex m_mutex_print_line;
  std::condition_variable m_condition_queue;
  std::queue<Command> m_queue_of_commands;
  Logger(const std::string &a_cmd_name, std::ostream & = std::cout);
  void set_commander_ptr(std::shared_ptr<Commander> &);

  void process(std::string);
  void thread_join();

public:
  static std::shared_ptr<Logger> create(const std::string &,
                                        std::shared_ptr<Commander> &,
                                        std::ostream & = std::cout);
  ~Logger();

  void update(const Command &) override;
  void setup_context(void *);
};
