#pragma once
#include <memory>

template <typename T> class IPublisher {
public:
  virtual ~IPublisher() = default;

  virtual void update(const T &) = 0;
};

template <typename T> class ISubscriber {
public:
  virtual ~ISubscriber() = default;

  virtual void subscribe(const std::shared_ptr<IPublisher<T>> &) = 0;
};