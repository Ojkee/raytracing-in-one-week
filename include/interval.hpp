#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include <algorithm>
#include "globals.hpp"

template <class T>
class Interval {
 public:
  Interval() : m_min(+globals::infinity<T>), m_max(-globals::infinity<T>) {};
  Interval(const T& min, const T& max) : m_min(min), m_max(max) {};

  [[nodiscard]] auto size() const noexcept -> T;
  [[nodiscard]] auto contains(const T& x) const noexcept -> bool;
  [[nodiscard]] auto surrounds(const T& x) const noexcept -> bool;
  [[nodiscard]] auto clamp(const T& x) const noexcept -> T;
  [[nodiscard]] auto max() const noexcept -> T;
  [[nodiscard]] auto min() const noexcept -> T;

  static const Interval<T> empty;
  static const Interval<T> universe;

 private:
  T m_min{};
  T m_max{};
};

template <class T>
const Interval<T> empty =
    Interval<T>{+globals::infinity<T>, -globals::infinity<T>};

template <class T>
static const Interval<T> universe =
    Interval<T>{-globals::infinity<T>, +globals::infinity<T>};

template <class T>
auto Interval<T>::size() const noexcept -> T {
  return m_max - m_min;
}

template <class T>
auto Interval<T>::contains(const T& x) const noexcept -> bool {
  return m_min <= x && x <= m_max;
}

template <class T>
auto Interval<T>::surrounds(const T& x) const noexcept -> bool {
  return m_min < x && x < m_max;
}

template <class T>
auto Interval<T>::clamp(const T& x) const noexcept -> T {
  return std::clamp(x, m_min, m_max);
}

template <class T>
auto Interval<T>::max() const noexcept -> T {
  return m_max;
}

template <class T>
auto Interval<T>::min() const noexcept -> T {
  return m_min;
}

#endif  // !INTERVAL_HPP
