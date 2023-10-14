#include "async.h"
#include <future>

void Test_1() {
  std::this_thread::sleep_for(std::chrono::microseconds(2));
  async::handle_t handle = async::connect(3);
  async::receive(handle, "11\n", 3);
  async::receive(handle, "12\n", 3);
  async::receive(handle, "13\n", 3);
  async::receive(handle, "14\n", 3);
  async::receive(handle, "15\n", 3);
  async::receive(handle, "16\n", 3);
  async::disconnect(handle);
  async::receive(handle, "16\n", 3);
  std::this_thread::sleep_for(std::chrono::seconds(7));
}
void Test_2() {
  async::handle_t h2 = async::connect(2);
  async::receive(h2, "21\n", 3);
  async::receive(h2, "22\n", 3);
  async::receive(h2, "23\n", 3);
  async::receive(h2, "24\n", 3);
  async::receive(h2, "25\n", 3);
  async::receive(h2, "26\n", 3);
  async::disconnect(h2);
  std::this_thread::sleep_for(std::chrono::seconds(5));
}
