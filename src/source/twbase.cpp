#include "twbase/twbase.hpp"
#include "twbase/assert.hpp"
#include <iostream>

namespace TWBASE_NAMESPACE
{

void libraryFunction(int value)
{
  TWBASE_ASSERT(value >= 0);
  std::cout << "Hello from libraryFunction! The answer is " << value << "\n";
}

}