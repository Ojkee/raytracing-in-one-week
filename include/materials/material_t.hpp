#ifndef HATERIAL_T_HPP
#define HATERIAL_T_HPP

#include <utility>
#include "color.hpp"
#include "ray.hpp"

template <class T>
using ScatterData_t = std::pair<Ray<T>, Color<T>>;

#endif  // !HATERIAL_T_HPP
