#include "log.h"

const std::string SEPARATOR = "_";

Logger::Logger(const std::string &a_cmd_name, std::ostream &a_stream_out)
    : m_stream_out{a_stream_out}, m_is_done{false} {

  for (size_t i = 0; i < 2; ++i) {
    std::string strThreadName = a_cmd_name + std::to_string(i + 1);
    m_threads.emplace_back(std::thread(&Logger::process, this, strThreadName));
  }
}

Logger::~Logger() { thread_join(); }

std::shared_ptr<Logger>
Logger::create(const std::string &a_cmd_name,
               std::shared_ptr<Commander> &a_commander_ptr,
               std::ostream &a_stream_out) {
  auto ptr = std::shared_ptr<Logger>{new Logger{a_cmd_name, a_stream_out}};
  ptr->set_commander_ptr(a_commander_ptr);
  return ptr;
}

void Logger::update(const Command &a_command) {
  {
    std::unique_lock<std::mutex> lock(m_mutex_queue);
    m_queue_of_commands.push(a_command);
  }
  m_condition_queue.notify_all();
}

void Logger::process(std::string a_cmd_name) {
  Command commandBlock;
  Counters counters{a_cmd_name};
  while (!m_is_done) {
    {
      std::unique_lock<std::mutex> locker(m_mutex_queue);
      m_condition_queue.wait(
          locker, [&]() { return !m_queue_of_commands.empty() || m_is_done; });
      if (!m_queue_of_commands.empty()) {
        commandBlock = m_queue_of_commands.front();
        m_queue_of_commands.pop();
      }
    }

    if (commandBlock.get_length()) {
      std::stringstream ssFileName;
      ssFileName << "bulk" << SEPARATOR << a_cmd_name << SEPARATOR << m_context
                 << SEPARATOR << commandBlock.get_current_time() << ".log";

      std::ofstream log(ssFileName.str(), std::ios::out);

      log << "bulk: " << commandBlock << std::endl;
      ++counters.m_block_commands;
      counters.m_commands += commandBlock.get_length();
      commandBlock.clr();
      log.close();
    }
  }

  {
    std::unique_lock<std::mutex> locker(m_mutex_print_line);
    m_stream_out << m_context << ' ' << counters << std::endl;
  }
}

void Logger::thread_join() {
  m_is_done = true;
  m_condition_queue.notify_all();
  for (auto &thread : m_threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void Logger::set_commander_ptr(std::shared_ptr<Commander> &a_commander_ptr) {
  m_command_ptr = a_commander_ptr;
  auto ptrCommander = m_command_ptr.lock();
  if (ptrCommander) {
    ptrCommander->subscribe(shared_from_this());
  }
}

void Logger::setup_context(void *a_context) { m_context = a_context; }
