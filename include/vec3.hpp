#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <format>
#include <ostream>
#include <type_traits>

template <typename T,
          typename = typename std::enable_if_t<std::is_arithmetic_v<T>, T>>
class Vec3 {
 public:
  Vec3() : m_x(), m_y(), m_z() {};
  Vec3(const T& x, const T& y, const T& z) : m_x(x), m_y(y), m_z(z) {};

  Vec3(const Vec3&) = default;
  Vec3(Vec3&&) = default;

  [[nodiscard]] auto x() const noexcept { return m_x; };
  [[nodiscard]] auto y() const noexcept { return m_y; };
  [[nodiscard]] auto z() const noexcept { return m_z; };

  [[nodiscard]] auto operator-() noexcept { return Vec3(-m_x, -m_y, -m_z); };
  auto& operator+=(const Vec3& other) noexcept {
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;
    return *this;
  }
  [[nodiscard]] auto& operator*=(const T& t) noexcept {
    m_x *= t;
    m_y *= t;
    m_z *= t;
  }
  [[nodiscard]] auto& operator/=(const T& t) { return *this *= 1 / t; }

  template <class R>
  [[nodiscard]] R length_squared() const noexcept {
    return static_cast<R>(m_x * m_x + m_y * m_y + m_z * m_z);
  }

  template <class R>
  [[nodiscard]] R length() const noexcept {
    return std::sqrt(length_squared<R>());
  }

 private:
  T m_x{}, m_y{}, m_z{};
};

template <class T>
inline auto& operator<<(std::ostream& out, const Vec3<T>& v) {
  return out << std::format("{} {} {}", v.x(), v.y(), v.z());
}

template <class T>
inline auto operator+(const Vec3<T>& u, const Vec3<T>& v) {
  return Vec3<T>(u.x() + v.x(), u.y() + v.y(), u.z() + u.z());
}

template <class T>
inline auto operator-(const Vec3<T>& u, const Vec3<T>& v) {
  return u + (-v);
}

template <class T>
inline auto operator*(const Vec3<T>& u, const Vec3<T>& v) {
  return Vec3<T>(u.x() * v.x(), u.y() * v.y(), u.z() * u.z());
}

template <class T>
inline auto operator*(const Vec3<T>& v, const T& t) {
  auto u{v};
  return u *= t;
}

template <class T>
inline auto operator/(const Vec3<T>& v, const T& t) {
  auto u{v};
  return u /= t;
}

template <class T>
inline T dot(const Vec3<T>& u, const Vec3<T>& v) {
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

template <class T>
inline auto cross(const Vec3<T>& u, const Vec3<T>& v) {
  return Vec3<T>(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
                 u.x() * v.y() - u.y() * v.x());
}

template <class R, class T>
inline Vec3<R> unit_vector(const Vec3<T>& v) {
  return v / v.length();
}

#endif  // !VEC3_HPP
