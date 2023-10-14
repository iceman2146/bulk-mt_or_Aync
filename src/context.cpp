#include "context.h"

Context::Context(std::size_t a_cmd_size, std::ostream &a_osLoggerOut,
                 std::ostream &a_osMainMetricsOut,
                 std::ostream &a_osLogMetricsOut,
                 std::ostream &a_oFileMetricsOut)
    : m_is_done{false}, m_thread{&Context::procces, this} {
  m_command_ptr = Commander::create(
      "main", static_cast<std::size_t>(a_cmd_size), a_osMainMetricsOut);
  m_log_ptr = Logger::create("file", m_command_ptr, a_oFileMetricsOut);
  m_execute_ptr =
      Executer::create("log", m_command_ptr, a_osLoggerOut, a_osLogMetricsOut);

  m_command_ptr->setup_context(this);
  m_log_ptr->setup_context(this);
  m_execute_ptr->setup_context(this);
}

Context::~Context() { join_t(); }

void Context::buffer(const char *a_buff, std::size_t a_size) {
  {
    std::unique_lock<std::mutex> lock(m_stream_mute);
    m_in_stream.write(a_buff, a_size);
  }
  m_is_ready = true;
  m_intput_condition.notify_one();
}

void Context::procces() {
  while (!m_is_done) {
    std::unique_lock<std::mutex> locker(m_stream_mute);
    m_intput_condition.wait(locker, [&]() { return m_is_ready || m_is_done; });
    std::string strLine;
    while (std::getline(m_in_stream, strLine)) {
      m_command_ptr->get_line(strLine);
    }
    m_in_stream.clear();
    m_in_stream.str("");
    m_is_ready = false;
  }
}

void Context::join_t() {
  m_is_done = true;
  m_intput_condition.notify_all();
  if (m_thread.joinable()) {
    m_thread.join();
  }
}
