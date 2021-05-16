#pragma once

#include "vector.hpp"

#include <type_traits>

namespace core
{
    template<typename T,
             typename = std::enable_if<std::is_floating_point<T>::value>>
    class Ray
    {
    public:
        Ray() = default;

        Ray(Point3<T> const& origin, Point3<T> const& dir) : o{origin}, d{dir}
        {}

        Point3<T> operator()(T t) const
        {
            return o + t * d;
        }

        Point3<T> o;
        Vector3<T> d;
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, Ray<T> const& r)
    {
        os << "o = " << r.o << ", d = " << r.d;
        return os;
    }
} // namespace core
