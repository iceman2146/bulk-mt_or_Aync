#include "execute.h"

Executer::Executer(const std::string &a_cmd_name,
                   std::ostream &a_command_out_stream,
                   std::ostream &a_stream_out)
    : m_osOut{a_command_out_stream}, m_stream_out{a_stream_out},
      m_is_done{false}, m_thread{&Executer::procces, this, a_cmd_name} {}

Executer::~Executer() { join_t(); }

std::shared_ptr<Executer> Executer::create(
    const std::string &a_cmd_name, std::shared_ptr<Commander> &a_commander_ptr,
    std::ostream &a_command_out_stream, std::ostream &a_stream_out) {
  auto ptr = std::shared_ptr<Executer>{
      new Executer{a_cmd_name, a_command_out_stream, a_stream_out}};
  ptr->set_commander_ptr(a_commander_ptr);
  return ptr;
}

void Executer::update(const Command &a_command) {
  {
    std::unique_lock<std::mutex> lock(m_mutex_queue);
    m_queue_of_commands.push(a_command);
  }
  m_condition_queue.notify_all();
}

void Executer::procces(std::string a_cmd_name) {
  Counters counters{a_cmd_name};
  Command commandBlock;
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
      {
        std::unique_lock<std::mutex> locker(m_mutex_print_line);
        m_osOut << "bulk: " << commandBlock << std::endl;
      }

      ++counters.m_block_commands;
      counters.m_commands += commandBlock.get_length();
      commandBlock.clr();
    }
  }

  m_stream_out << m_context << ' ' << counters << std::endl;
}

void Executer::join_t() {
  m_is_done = true;
  m_condition_queue.notify_all();
  if (m_thread.joinable()) {
    m_thread.join();
  }
}

void Executer::set_commander_ptr(std::shared_ptr<Commander> &a_commander_ptr) {
  m_command_ptr = a_commander_ptr;
  auto ptrCommander = m_command_ptr.lock();
  if (ptrCommander) {
    ptrCommander->subscribe(shared_from_this());
  }
}

void Executer::setup_context(void *a_context) { m_context = a_context; }
