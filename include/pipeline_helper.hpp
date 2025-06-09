#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <functional>
#include <ranges>
#include <type_traits>

#define ENABLE_PIPE using namespace pipeline;

namespace pipeline {
template <class T,
          class F,
          std::enable_if_t<std::is_invocable_v<F, T> && !std::ranges::range<F>,
                           int> = 0>
auto operator|(T&& arg, F&& fn) {
  return std::invoke(std::forward<F>(fn), std::forward<T>(arg));
}
}  // namespace pipeline

#endif  // !PIPELINE_HPP
