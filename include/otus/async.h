#pragma once

#include "context.h"
#include "execute.h"
#include "log.h"
#include <cstddef>

#include <map>
#include <memory>
namespace async {

using handle_t = void *;

handle_t connect(std::size_t);
void receive(handle_t, const char *, std::size_t);
void disconnect(handle_t);

} // namespace async
