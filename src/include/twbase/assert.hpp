#ifndef TWBASE_INCLUDE_GUARD_ASSERT_HPP
#define TWBASE_INCLUDE_GUARD_ASSERT_HPP

#include "twbase/config.hpp"
#include "twbase/twbase_export.h"
#include "twbase/compiler.hpp"
#include <cstdlib>

// Check if we're on windows and have access to __debugbreak().
#if !defined(TWBASE_DEBUGBREAK)
#if defined(_WIN32) && defined(__has_include)
#if __has_include(<intrin.h>)
#define TWBASE_DEBUGBREAK 1
#endif
#endif
#endif

#if !defined(TWBASE_DEBUGBREAK)
#define TWBASE_DEBUGBREAK 0
#endif

// Check if we're on a compiler that supports __builtin_trap().
#if !defined(TWBASE_BUILTIN_TRAP)
#if defined(__GNUC__) || defined(__clang__)
#define TWBASE_BUILTIN_TRAP 1
#else
#define TWBASE_BUILTIN_TRAP 0
#endif
#endif

#if !defined(TWBASE_BUILTIN_TRAP)
#define TWBASE_BUILTIN_TRAP 0
#endif

#if TWBASE_DEBUGBREAK
#include <intrin.h> // Provides access to the __debugbreak() function on windows.
#define TWBASE_ASSERT_TRAP() ::__debugbreak()
#elif TWBASE_BUILTIN_TRAP
#define TWBASE_ASSERT_TRAP() __builtin_trap()
#else
#define TWBASE_ASSERT_TRAP() ::std::abort()
#endif

namespace TWBASE_NAMESPACE
{

using assertion_handler = void (*)(const char* fileName, int line, const char* funcName, const char* message) noexcept;

TWBASE_EXPORT void assertion(const char* fileName, int line, const char* funcName, const char* message);
TWBASE_EXPORT assertion_handler set_assertion_handler(assertion_handler handler) noexcept;
TWBASE_EXPORT assertion_handler get_assertion_handler() noexcept;

} // namespace TWBASE_NAMESPACE

#ifdef NDEBUG
#define TWBASE_ASSERT(...)
#define TWBASE_ASSERT_MSG(expr, msg)
#else
#define TWBASE_ASSERT(...)                                                                                                                 \
  do                                                                                                                                       \
  {                                                                                                                                        \
    if (TWBASE_UNLIKELY(!(__VA_ARGS__)))                                                                                                   \
    {                                                                                                                                      \
      ::TWBASE_NAMESPACE::assertion(__FILE__, __LINE__, __func__, #__VA_ARGS__);                                                           \
      TWBASE_ASSERT_TRAP();                                                                                                                \
    }                                                                                                                                      \
  } while (false)

#define TWBASE_ASSERT_MSG(expr, msg)                                                                                                        \
  do                                                                                                                                       \
  {                                                                                                                                        \
    if (TWBASE_UNLIKELY(!(expr)))                                                                                                           \
    {                                                                                                                                      \
      ::TWBASE_NAMESPACE::assertion(__FILE__, __LINE__, __func__, (msg));                                                                   \
      TWBASE_ASSERT_TRAP();                                                                                                                 \
    }                                                                                                                                      \
  } while (false)
#endif

#endif // TWBASE_INCLUDE_GUARD_ASSERT_HPP
