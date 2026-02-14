#include "gtest/gtest.h"
#include "twbase/allocator/stateless_allocator.hpp"

TEST(stateless_allocator, basic_operations)
{
  using allcocator_type = twbase::stateless_allocator<int>;
  allcocator_type allocator;

  // Test allocation and deallocation.
  int* ptr = allocator.allocate(5);
  EXPECT_NE(ptr, nullptr);

  allocator.deallocate(ptr, 5);
}

TEST(stateless_allocator, allocate_too_large)
{
  using allocator_type = twbase::stateless_allocator<int>;
  allocator_type allocator;

  EXPECT_THROW((void)allocator.allocate(std::numeric_limits<std::size_t>::max() / sizeof(int) + 1), std::bad_array_new_length);
}

TEST(stateless_allocator, allocate_zero)
{
  using allocator_type = twbase::stateless_allocator<int>;
  allocator_type allocator;

  int* ptr = allocator.allocate(0);
  EXPECT_EQ(ptr, nullptr);
}

TEST(stateless_allocator, equality)
{
  using allocator_type = twbase::stateless_allocator<int>;
  allocator_type allocator1;
  allocator_type allocator2;

  EXPECT_TRUE(allocator1 == allocator2);
  EXPECT_FALSE(allocator1 != allocator2);
}

TEST(stateless_allocator, overaligned_type)
{
  struct alignas(64) overaligned_type
  {
    double data[8];
  };

  using allocator_type = twbase::stateless_allocator<overaligned_type>;
  allocator_type allocator;

  overaligned_type* ptr = allocator.allocate(1);
  EXPECT_NE(ptr, nullptr);
  EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % 64, 0);

  allocator.deallocate(ptr, 1);
}

TEST(stateless_allocator, max_size)
{
  using allocator_type = twbase::stateless_allocator<int>;
  allocator_type allocator;

  EXPECT_EQ(allocator.max_size(), std::numeric_limits<std::size_t>::max() / sizeof(int));
}

TEST(stateless_allocator, rebind)
{
  using allocator_type = twbase::stateless_allocator<int>;
  allocator_type allocator;

  using rebound_allocator_type = typename allocator_type::rebind<double>::other;
  rebound_allocator_type rebound_allocator;

  double* ptr = rebound_allocator.allocate(5);
  EXPECT_NE(ptr, nullptr);

  rebound_allocator.deallocate(ptr, 5);
}

TEST(stateless_allocator, stl_vector)
{
  std::vector<int, twbase::stateless_allocator<int>> vec{1, 2, 3, 4, 5};

  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[4], 5);

  vec.resize(10);
  EXPECT_EQ(vec.size(), 10);
}

TEST(stateless_allocator, stl_map)
{
  std::map<int, int, std::less<int>, twbase::stateless_allocator<std::pair<const int, int>>> my_map;

  my_map[1] = 10;
  my_map[2] = 20;

  EXPECT_EQ(my_map.size(), 2);
  EXPECT_EQ(my_map[1], 10);
  EXPECT_EQ(my_map[2], 20);
}