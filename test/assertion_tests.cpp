#include "twbase/assert.hpp"
#include <gtest/gtest.h>

#include <atomic>

namespace
{
std::atomic<int> g_call_count{0};
const char* g_file_name = nullptr;
int g_line = 0;
const char* g_func_name = nullptr;
const char* g_message = nullptr;

void test_handler(const char* fileName, int line, const char* funcName, const char* message) noexcept
{
  g_call_count.fetch_add(1, std::memory_order_relaxed);
  g_file_name = fileName;
  g_line = line;
  g_func_name = funcName;
  g_message = message;
}

void reset_capture()
{
  g_call_count.store(0, std::memory_order_relaxed);
  g_file_name = nullptr;
  g_line = 0;
  g_func_name = nullptr;
  g_message = nullptr;
}
} // namespace

TEST(assertion_handler, set_get_and_invoke)
{
  auto previous = twbase::set_assertion_handler(test_handler);
  EXPECT_EQ(previous, nullptr);

  reset_capture();
  twbase::assertion("file.cpp", 123, "func", "msg");

  EXPECT_EQ(g_call_count.load(std::memory_order_relaxed), 1);
  EXPECT_STREQ(g_file_name, "file.cpp");
  EXPECT_EQ(g_line, 123);
  EXPECT_STREQ(g_func_name, "func");
  EXPECT_STREQ(g_message, "msg");

  auto removed = twbase::set_assertion_handler(nullptr);
  EXPECT_EQ(removed, test_handler);
  EXPECT_EQ(twbase::get_assertion_handler(), nullptr);
}
