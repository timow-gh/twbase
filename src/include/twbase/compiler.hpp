#ifndef TWBASE_INCLUDE_GUARD_COMPILER_HPP
#define TWBASE_INCLUDE_GUARD_COMPILER_HPP

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#ifndef __has_feature
#define __has_feature(x) 0
#endif

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#ifndef __has_cpp_attribute
#define __has_cpp_attribute(x) 0
#endif

// Mark functions that never return to help optimization and warnings.
// Example: TWBASE_NORETURN void fatal(const char* msg) { std::abort(); }
#if __has_cpp_attribute(noreturn)
#define TWBASE_NORETURN [[noreturn]]
#elif __has_attribute(noreturn)
#define TWBASE_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define TWBASE_NORETURN __declspec(noreturn)
#else
#define TWBASE_NORETURN
#endif

// Pack structs for ABI or wire formats when you need exact layout.
// Example: struct TWBASE_PACKED Header { std::uint8_t a; std::uint32_t b; };
#if __has_attribute(packed)
#define TWBASE_PACKED __attribute__((packed))
#elif defined(_MSC_VER)
#define TWBASE_PACKED
#else
#define TWBASE_PACKED
#endif

// MSVC packing helpers for exact layout when attributes are unavailable.
// Example: TWBASE_PACKED_BEGIN struct Header { ... }; TWBASE_PACKED_END
#if defined(_MSC_VER)
#define TWBASE_PACKED_BEGIN __pragma(pack(push, 1))
#define TWBASE_PACKED_END __pragma(pack(pop))
#else
#define TWBASE_PACKED_BEGIN
#define TWBASE_PACKED_END
#endif

// Hint likely/unlikely branches for hot paths.
// Example: if (TWBASE_LIKELY(x > 0)) { fast(); } else { slow(); }
#if __has_builtin(__builtin_expect)
#define TWBASE_LIKELY(exp) (__builtin_expect(!!(exp), true))
#define TWBASE_UNLIKELY(exp) (__builtin_expect(!!(exp), false))
#else
#define TWBASE_LIKELY(exp) (!!(exp))
#define TWBASE_UNLIKELY(exp) (!!(exp))
#endif

// Prefetch memory when you know you will touch it soon.
// Example: TWBASE_PREFETCH(ptr + 64);
#if __has_builtin(__builtin_prefetch)
#define TWBASE_PREFETCH(exp) (__builtin_prefetch(exp))
#else
#define TWBASE_PREFETCH(exp)
#endif

// Assert to the compiler a condition is always true (UB if false).
// Example: TWBASE_ASSUME(index < size);
#if defined(_MSC_VER)
#define TWBASE_ASSUME(exp) __assume(exp)
#elif __has_builtin(__builtin_assume)
#define TWBASE_ASSUME(exp) (__builtin_assume(exp))
#elif __has_builtin(__builtin_unreachable)
#define TWBASE_ASSUME(exp)                      \
	do                                            \
	{                                             \
		if (!(exp))                                 \
		{                                           \
			__builtin_unreachable();                  \
		}                                           \
	} while (false)
#else
#define TWBASE_ASSUME(exp)
#endif

// Force inline for tiny hot functions.
// Example: TWBASE_ALWAYS_INLINE int add(int a, int b) { return a + b; }
#if defined(_MSC_VER)
#define TWBASE_ALWAYS_INLINE __forceinline
#elif __has_attribute(always_inline)
#define TWBASE_ALWAYS_INLINE __attribute__((always_inline))
#else
#define TWBASE_ALWAYS_INLINE
#endif

// Prevent inlining when you want a separate call site.
// Example: TWBASE_NOINLINE void slow_path();
#if defined(_MSC_VER)
#define TWBASE_NOINLINE __declspec(noinline)
#elif __has_attribute(noinline)
#define TWBASE_NOINLINE __attribute__((noinline))
#else
#define TWBASE_NOINLINE
#endif

// Restrict pointers for better aliasing optimizations.
// Example: void f(int* TWBASE_RESTRICT a, int* TWBASE_RESTRICT b);
#if defined(_MSC_VER) && _MSC_VER >= 1900
#define TWBASE_RESTRICT __restrict
#elif (defined(__clang__) || defined(__GNUC__))
#define TWBASE_RESTRICT __restrict__
#else
#define TWBASE_RESTRICT
#endif

#endif // TWBASE_INCLUDE_GUARD_COMPILER_HPP
