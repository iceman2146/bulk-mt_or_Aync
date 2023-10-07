#pragma once
#include "observer_interface.h"

#include <iostream>
#include <list>
/**
 * @file observer.h 
 * 
 */
/**
 * @brief базовый класс, реализующий интерфейсы подписчика
 */ 
template <typename T>
class BaseSubscriber : public ISubscriber<T>
{
private:
  std::list<std::weak_ptr<IPublisher<T>>> sub_list;

public:
  void subscribe(const std::shared_ptr<IPublisher<T>> &subscriber_ptr) override
  {
    sub_list.emplace_back(subscriber_ptr);
  }

  void notify(const T &data)
  {
    auto iter = sub_list.begin();
    while (iter != sub_list.end())
    {
      auto ptr = iter->lock();
      if (ptr)
      {
        ptr->update(data);
        ++iter;
      }
      else
      {
        sub_list.erase(iter++);
      }
    }
  }
};
