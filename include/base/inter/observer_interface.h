#pragma on
#include <memory>
/**
 * @file observer_interface.h 
 * 
 */
 /**
 * @brief абстрактный класс издателя
 */ 
template <typename T>
class IPublisher
{
public:
  virtual ~IPublisher() = default;
  virtual void update(const T &) = 0;
};
 /**
 * @brief абстрактный класс подписчика
 */ 
template <typename T>
class ISubscriber
{
public:

  virtual ~ISubscriber() = default;
  virtual void subscribe(const std::shared_ptr<IPublisher<T>> &subscriber_ptr) = 0;
};
