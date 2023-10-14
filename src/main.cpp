#include "cmd_input_check.h"
#include "test.h"
int main(int argc, const char **argv) {
  long a_bulk_numbers{0};
  check(argc, argv);
  a_bulk_numbers = std::stoi(argv[1]);
  async::handle_t handle = async::connect(a_bulk_numbers);

  auto result_1 = std::async(std::launch::async, Test_1);
  auto result_2 = std::async(std::launch::async, Test_2);

  std::string strLine;
  while (std::getline(std::cin, strLine)) {
    async::receive(handle, strLine.c_str(), strLine.size());
  }

  async::disconnect(handle);

  result_1.wait();
  result_2.wait();

  return 0;
}
