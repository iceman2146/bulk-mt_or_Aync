#pragma once

#include "commander.h"
#include "execute.h"
#include "log.h"

#include <memory>
#include <sstream>
#include <string>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

class Context {

private:
  std::shared_ptr<Commander> m_command_ptr;
  std::shared_ptr<Executer> m_execute_ptr;
  std::shared_ptr<Logger> m_log_ptr;

  std::stringstream m_in_stream;
  std::condition_variable m_intput_condition;
  std::atomic<bool> m_is_done;
  std::atomic<bool> m_is_ready;
  std::thread m_thread;
  std::mutex m_stream_mute;

  void procces();
  void join_t();

public:
  Context(std::size_t a_cmd_size, std::ostream &a_osLoggerOut = std::cout,
          std::ostream &a_osMainMetricsOut = std::cout,
          std::ostream &a_osLogMetricsOut = std::cout,
          std::ostream &a_oFileMetricsOut = std::cout);
  ~Context();

  void buffer(const char *a_buff, std::size_t a_size);
};
