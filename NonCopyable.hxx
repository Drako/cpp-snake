#pragma once

#ifndef SNAKE_NONCOPYABLE_HXX
#define SNAKE_NONCOPYABLE_HXX

class NonCopyable {
public:
  NonCopyable() noexcept = default;

  NonCopyable(NonCopyable const&) = delete;

  NonCopyable& operator=(NonCopyable const&) = delete;
};

#endif // SNAKE_NONCOPYABLE_HXX
