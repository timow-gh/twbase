#ifndef TWBASE_INCLUDE_GUARD_STATELESS_ALLOCATOR_HPP
#define TWBASE_INCLUDE_GUARD_STATELESS_ALLOCATOR_HPP

#include "twbase/config.hpp"
#include <algorithm>
#include <cstddef>
#include <limits>
#include <new>

namespace TWBASE_NAMESPACE
{

namespace detail
{

template <typename T>
constexpr std::align_val_t alignment_v = static_cast<std::align_val_t>(std::max(alignof(T), alignof(std::max_align_t)));

inline void* allocate(const std::size_t bytes)
{
  return ::operator new(bytes);
}

inline void deallocate(void* ptr) noexcept
{
  ::operator delete(ptr);
}

inline void* allocate_aligned(const std::size_t bytes, const std::align_val_t alignment)
{
  return ::operator new(bytes, alignment);
}

inline void deallocate_aligned(void* ptr, std::size_t, const std::align_val_t alignment) noexcept
{
  ::operator delete(ptr, alignment);
}

template <std::size_t ElemSize>
constexpr inline std::size_t get_number_of_bytes(const std::size_t count)
{
  constexpr std::size_t max = std::numeric_limits<std::size_t>::max() / ElemSize;
  if (count > max)
  {
    throw std::bad_array_new_length();
  }

  return count * ElemSize;
}

} // namespace detail

template <typename T>
class stateless_allocator
{
public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap = std::true_type;
  using is_always_equal = std::true_type;

  constexpr stateless_allocator() = default;

  template <typename U>
  constexpr stateless_allocator(const stateless_allocator<U>&) noexcept
  {
  }

  template <typename U>
  struct rebind
  {
    using other = stateless_allocator<U>;
  };

  [[nodiscard]] T* allocate(std::size_t count)
  {
    if (count == 0)
    {
      return nullptr;
    }

    return static_cast<T*>(detail::allocate_aligned(detail::get_number_of_bytes<sizeof(T)>(count), detail::alignment_v<T>));
  }

  void deallocate(T* ptr, const std::size_t count) noexcept { detail::deallocate_aligned(ptr, sizeof(T) * count, detail::alignment_v<T>); }

  constexpr std::size_t max_size() const noexcept { return std::numeric_limits<std::size_t>::max() / sizeof(T); }

  constexpr friend bool operator==(const stateless_allocator& lhs, const stateless_allocator& rhs) noexcept { return true; }

  constexpr friend bool operator!=(const stateless_allocator& lhs, const stateless_allocator& rhs) noexcept { return false; }
};

} // namespace TWBASE_NAMESPACE

#endif // TWBASE_INCLUDE_GUARD_STATELESS_ALLOCATOR_HPP