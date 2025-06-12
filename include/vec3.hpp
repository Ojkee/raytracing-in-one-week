#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <format>
#include <functional>
#include <iostream>
#include <ostream>
#include <ranges>
#include <type_traits>
#include "globals.hpp"
#include "pipeline_helper.hpp"

#define LOG_V(v) std::clog << (v) << "\n"

template <typename T,
          typename = typename std::enable_if_t<std::is_arithmetic_v<T>, T>>
class Vec3 {
 public:
  Vec3() : m_x(), m_y(), m_z() {};
  Vec3(const T& x, const T& y, const T& z) : m_x(x), m_y(y), m_z(z) {};

  using type = T;

  [[nodiscard]] auto x() const noexcept -> const T& { return m_x; };
  [[nodiscard]] auto y() const noexcept -> const T& { return m_y; };
  [[nodiscard]] auto z() const noexcept -> const T& { return m_z; };

  [[nodiscard]] auto operator-() const noexcept -> Vec3<T> {
    return Vec3(-m_x, -m_y, -m_z);
  };

  auto operator+=(const Vec3& other) noexcept -> Vec3<T>& {
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;
    return *this;
  }

  auto operator*=(const T& t) noexcept -> Vec3<T>& {
    m_x *= t;
    m_y *= t;
    m_z *= t;
    return *this;
  }

  auto operator/=(const T& t) noexcept -> Vec3<T>& { return *this *= 1 / t; }

  [[nodiscard]] auto length_squared() const noexcept -> T {
    return m_x * m_x + m_y * m_y + m_z * m_z;
  }

  [[nodiscard]] auto length() const noexcept -> T {
    return std::sqrt(length_squared());
  }

  [[nodiscard]] static auto random(const T& min, const T& max) noexcept
      -> Vec3<T> {
    return Vec3<T>(globals::random_t<T>(min, max),
                   globals::random_t<T>(min, max),
                   globals::random_t<T>(min, max));
  }

  [[nodiscard]] static auto random() noexcept -> Vec3<T> {
    return random(0, 1);
  }

  // TODO: generates 1 unnecessary vec at the end
  [[nodiscard]] static inline auto random_in_unit_sphere() noexcept -> Vec3<T> {
    // auto result = Vec3<T>{};
    // auto is_in = [&result](const auto& v) {
    //   if (v.length_squared() < 1) {
    //     result = v;
    //     return true;
    //   }
    //   return false;
    // };
    // auto gen_vec = [](auto) { return Vec3<T>::random(-1, 1); };
    // const auto units = std::views::iota(0) | std::views::transform(gen_vec);
    // std::ranges::find_if(units, is_in);
    // return result;
    using pipeline::operator|;
    auto is_in = [](const auto& v) { return v.length_squared() < 1; };
    auto norm = [&is_in](auto v) { return (is_in(v)) ? v : v / 2.; };
    return Vec3<T>::random(-1, 1) | norm;
  }

  [[nodiscard]] static inline auto random_unit_vector() -> Vec3<T> {
    return unit_vector<T>(random_in_unit_sphere());
  }

  [[nodiscard]] static inline auto random_on_hemisphere(
      const Vec3<T>& normal) noexcept -> Vec3<T> {
    const auto on_unit_sphere = random_unit_vector();
    return (dot(on_unit_sphere, normal) > 0.) ? on_unit_sphere
                                              : -on_unit_sphere;
  }

 private:
  T m_x{}, m_y{}, m_z{};
};

template <class T>
inline auto operator<<(std::ostream& out, const Vec3<T>& v) noexcept
    -> std::ostream& {
  return out << std::format("{} {} {}", v.x(), v.y(), v.z());
}

template <class T>
inline auto operator+(const Vec3<T>& u, const Vec3<T>& v) noexcept -> Vec3<T> {
  return Vec3<T>(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

template <class T>
inline auto operator-(const Vec3<T>& u, const Vec3<T>& v) noexcept -> Vec3<T> {
  auto x = u.x() - v.x();
  auto y = u.y() - v.y();
  auto z = u.z() - v.z();
  return Vec3<T>(x, y, z);
}

template <class T>
inline auto operator-(const Vec3<T>& v) noexcept -> Vec3<T> {
  return Vec3<T>(-v.x(), -v.y(), -v.z());
}

template <class T>
inline auto operator*(const Vec3<T>& u, const Vec3<T>& v) noexcept -> Vec3<T> {
  return Vec3<T>(u.x() * v.x(), u.y() * v.y(), u.z() * u.z());
}

template <class T>
inline auto operator*(const Vec3<T>& v, const T& t) noexcept -> Vec3<T> {
  auto u{v};
  return u *= t;
}

template <class T>
inline auto operator*(const T& t, const Vec3<T>& v) noexcept -> Vec3<T> {
  return v * t;
}

template <class T>
inline auto operator/(const Vec3<T>& v, const T& t) noexcept -> Vec3<T> {
  auto u{v};
  u /= t;
  return u;
}

template <class T>
inline auto dot(const Vec3<T>& u, const Vec3<T>& v) noexcept -> T {
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

template <class T>
inline auto cross(const Vec3<T>& u, const Vec3<T>& v) noexcept -> Vec3<T> {
  return Vec3<T>(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
                 u.x() * v.y() - u.y() * v.x());
}

template <class R, class T>
inline auto unit_vector(const Vec3<T>& v) noexcept -> Vec3<R> {
  return v / v.length();
}

#endif  // !VEC3_HPP
