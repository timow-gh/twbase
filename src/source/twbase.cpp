#include "twbase/twbase.hpp"
#include <iostream>

namespace twbase
{

void libraryFunction(int value)
{
  std::cout << "Hello from libraryFunction! The answer is " << value << "\n";
}

}