#pragma once
#include "observer_interface.h"
#include <iostream>
#include <list>

template <typename T> class Subscriber : public ISubscriber<T> {
private:
  std::list<std::weak_ptr<IPublisher<T>>> m_subscribers_list;

public:
  void subscribe(const std::shared_ptr<IPublisher<T>> &a_ptrObserver) override {
    m_subscribers_list.emplace_back(a_ptrObserver);
  }

  void notify(const T &a_data) {
    auto iter = m_subscribers_list.begin();
    while (iter != m_subscribers_list.end()) {
      auto ptr = iter->lock();
      if (ptr) {
        ptr->update(a_data);
        ++iter;
      } else {
        m_subscribers_list.erase(iter++);
      }
    }
  }
};
