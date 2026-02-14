#include "twbase/allocator/stateless_allocator.hpp"
#include <vector>

int main()
{
  using allocator_type = twbase::stateless_allocator<int>;
  allocator_type allocator;

  std::vector<int, allocator_type> vec(allocator);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);

  return 0;
}
