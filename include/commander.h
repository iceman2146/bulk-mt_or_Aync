#pragma once

#include "command.h"
#include "counter.h"
#include "observer.h"

#include <iostream>
#include <string>

class Commander : public Subscriber<Command> {

private:
  void *m_context;
  std::ostream &m_stream_out;
  std::size_t m_cmd_size;
  std::size_t m_nested_depth;
  Counters m_counter;
  Command m_cmd;

private:
  Commander(const std::string &, std::size_t, std::ostream & = std::cout);

public:
  ~Commander();

public:
  void run();
  void get_line(const std::string &);
  void setup_context(void *);
  static std::shared_ptr<Commander> create(const std::string &, std::size_t,
                                           std::ostream & = std::cout);

private:
  void clear();
};
