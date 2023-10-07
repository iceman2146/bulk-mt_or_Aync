#include "read.h"
#include "execute.h"
#include "log.h"
#include "async.h"
#include <string>
#include <memory>
/**
 * @file main.cpp 
 * 
 */

/**
* @brief функция main
* Основная функция main
* @return возвращает код ошибки
*/

int main(int argc, const char **argv)
{

    std::size_t bulk = 5;
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);
    async::receive(h, "1", 1);
    async::receive(h2, "1\n", 2);
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h);
    async::disconnect(h2);


  if (argc < 2)
  {
    std::cerr << "bulk.exe <N>(N>2)(bulk.exe 4)" << std::endl;
    return 1;
  }
  std::size_t blocks{0};

  std::shared_ptr<Reader> read = Reader::create(blocks);
  std::shared_ptr<Executer> execute = Executer::create(read);
  std::shared_ptr<Logger> log = Logger::create(read);

  read->enter();

  return 0;
}