#include "commander.h"

Commander::Commander(const std::string &a_cmd_name, std::size_t a_cmd_size,
                     std::ostream &a_stream_out)
    : m_stream_out{a_stream_out}, m_cmd_size{a_cmd_size}, m_nested_depth{0},
      m_counter{a_cmd_name}, m_cmd{a_cmd_size} {}

Commander::~Commander() {
  m_stream_out << m_context << ' ' << m_counter << std::endl;
}

std::shared_ptr<Commander> Commander::create(const std::string &a_cmd_name,
                                             std::size_t a_cmd_size,
                                             std::ostream &a_stream_out) {
  auto ptr = std::shared_ptr<Commander>{
      new Commander{a_cmd_name, a_cmd_size, a_stream_out}};
  return ptr;
}

void Commander::get_line(const std::string &a_cmd_line) {
  if (a_cmd_line.empty()) {
    return;
  }

  ++m_counter.m_lines;
  if (a_cmd_line == "{") {
    if (m_nested_depth == 0) {
      clear();
    }
    m_nested_depth++;
  } else if (a_cmd_line == "}") {
    if (m_nested_depth == 0) {
      m_cmd.clr();
    }
    m_nested_depth--;
  } else {
    m_cmd << a_cmd_line;
  }

  if (!m_nested_depth && m_cmd.get_length() >= m_cmd_size) {
    clear();
  }
}

void Commander::clear() {
  if (m_cmd.get_length() > 0) {
    notify(m_cmd);
    m_counter.m_commands += m_cmd.get_length();
    ++m_counter.m_block_commands;
    m_cmd.clr();
  }
}

void Commander::setup_context(void *a_context) { m_context = a_context; }
