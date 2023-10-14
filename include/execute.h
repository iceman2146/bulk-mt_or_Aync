#pragma once

#include "command.h"
#include "commander.h"
#include "observer.h"

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

class Executer : public IPublisher<Command>,
                 public std::enable_shared_from_this<Executer> {

private:
  void *m_context;

  std::ostream &m_osOut;
  std::ostream &m_stream_out;
  std::weak_ptr<Commander> m_command_ptr;

  std::atomic<bool> m_is_done;
  std::thread m_thread;
  std::mutex m_mutex_queue;
  std::mutex m_mutex_print_line;
  std::condition_variable m_condition_queue;
  std::queue<Command> m_queue_of_commands;

public:
  static std::shared_ptr<Executer> create(const std::string &a_cmd_name,
                                          std::shared_ptr<Commander> &,
                                          std::ostream & = std::cout,
                                          std::ostream & = std::cout);
  ~Executer();

  void update(const Command &) override;

  void setup_context(void *);

private:
  Executer(const std::string &, std::ostream &, std::ostream &);
  void set_commander_ptr(std::shared_ptr<Commander> &);

  void procces(std::string);
  void join_t();
};
