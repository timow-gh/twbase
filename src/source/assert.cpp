#include "twbase/assert.hpp"
#include <atomic>
#include <cstdio>

namespace TWBASE_NAMESPACE
{

namespace
{
std::atomic<assertion_handler> g_assertion_handler{nullptr};
} // namespace

void assertion(const char* fileName, int line, const char* funcName, const char* message)
{
  auto handler = get_assertion_handler();
  if (handler)
  {
    handler(fileName, line, funcName, message);
  }

  std::fprintf(stderr, "%s:%d: internal check failed in '%s': '%s'\n", fileName, line, funcName, message);
  std::fflush(stderr);
}

assertion_handler set_assertion_handler(assertion_handler handler) noexcept
{
  return g_assertion_handler.exchange(handler, std::memory_order_acq_rel);
}

assertion_handler get_assertion_handler() noexcept
{
  return g_assertion_handler.load(std::memory_order_acquire);
}

} // namespace TWBASE_NAMESPACE
