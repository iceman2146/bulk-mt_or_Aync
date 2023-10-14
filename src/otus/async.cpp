#include "async.h"

namespace async {

std::map<handle_t, std::shared_ptr<Context>> contexts;
std::mutex context_mutex;

static handle_t setup_context(const std::shared_ptr<Context> &a_context_ptr) {
  std::lock_guard<std::mutex> lock(context_mutex);
  contexts.emplace(std::make_pair(a_context_ptr.get(), a_context_ptr));
  return a_context_ptr.get();
}

static void buffer(handle_t handle, const char *data, std::size_t size) {
  std::shared_ptr<Context> context_ptr;
  {
    std::lock_guard<std::mutex> lock(context_mutex);
    auto iter = contexts.find(handle);
    if (iter != contexts.end()) {
      context_ptr = iter->second;
    }
  }

  if (context_ptr) {
    context_ptr->buffer(data, size);
  }
}

static void un_setup_context(handle_t handle) {
  std::lock_guard<std::mutex> lock(context_mutex);
  auto iter = contexts.find(handle);
  if (iter != contexts.end()) {
    contexts.erase(iter);
  }
}

handle_t connect(std::size_t bulk) {
  auto context_ptr = std::make_shared<Context>(bulk);
  return setup_context(context_ptr);
}

void receive(handle_t handle, const char *data, std::size_t size) {
  buffer(handle, data, size);
}

void disconnect(handle_t handle) { un_setup_context(handle); }
} // namespace async
