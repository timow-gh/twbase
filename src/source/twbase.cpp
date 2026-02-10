#include "twbase/twbase.hpp"
#include <iostream>

namespace TWBASE_NAMESPACE
{

void libraryFunction(int value)
{
  std::cout << "Hello from libraryFunction! The answer is " << value << "\n";
}

}