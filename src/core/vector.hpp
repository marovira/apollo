#pragma once

#include "real.hpp"
#include "utils.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <numeric>
#include <tuple>
#include <zeus/assert.hpp>
#include <zeus/compiler.hpp>

namespace core
{
    template<typename T, std::size_t N>
    class Vector
    {
    public:
        using value_type = T;
        static constexpr auto dimension{N};

        Vector()
        {
            std::memset(data.data(), 0, N * sizeof(T));
        }

        explicit Vector(T x)
        {
            ASSERT(!is_nan(x));
            std::fill(data.begin(), data.end(), x);
        }

        template<typename... Args>
        explicit Vector(Args... args) : data{args...}
        {
#if defined(ZEUS_BUILD_DEBUG)
            for (auto val : data)
            {
                ASSERT(!is_nan(val));
            }
#endif
        }

        explicit Vector(Vector<T, N - 1> const& vec) : Vector{}
        {
            std::copy(vec.data.begin(), vec.data.end(), data.begin());

#if defined(ZEUS_BUILD_DEBUG)
            for (auto val : data)
            {
                ASSERT(!is_nan(val));
            }
#endif
        }

        explicit Vector(Vector<T, N + 1> const& vec)
        {
            std::copy(vec.data.begin(), vec.data.end() - 1, data.begin());
#if defined(ZEUS_BUILD_DEBUG)
            for (auto val : data)
            {
                ASSERT(!is_nan(val));
            }
#endif
        }

        T& operator[](std::size_t i)
        {
            ASSERT(i < dimension);
            return data[i];
        }

        T operator[](std::size_t i) const
        {
            ASSERT(i < dimension);
            return data[i];
        }

        std::array<T, N> data;
    };

    template<typename T, std::size_t N>
    using Point = Vector<T, N>;

    template<typename T, std::size_t N>
    using Normal = Vector<T, N>;

    template<typename T, std::size_t N>
    bool has_nans(Vector<T, N> const& vec)
    {
        return std::any_of(vec.data.begin(), vec.data.end(), [](T elem) {
            return is_nan(elem);
        });
    }

    template<typename T, std::size_t N, typename UnaryOp>
    Vector<T, N> unary_op(Vector<T, N>&& vec, UnaryOp&& fun)
    {
        Vector<T, N> out{std::move(vec)};
        std::transform(out.data.begin(), out.data.end(), out.data.begin(), fun);
        return out;
    }

    template<typename T, std::size_t N, typename UnaryOp>
    Vector<T, N> unary_op(Vector<T, N> const& vec, UnaryOp&& fun)
    {
        Vector<T, N> out{vec};
        std::transform(out.data.begin(), out.data.end(), out.data.begin(), fun);
        return out;
    }

    template<typename T, std::size_t N, typename BinaryOp>
    Vector<T, N>
    binary_op(Vector<T, N>&& lhs, Vector<T, N> const& rhs, BinaryOp&& fun)
    {
        Vector<T, N> out{std::move(lhs)};
        std::transform(out.data.begin(),
                       out.data.end(),
                       rhs.data.begin(),
                       out.data.begin(),
                       fun);
        return out;
    }

    template<typename T, std::size_t N, typename BinaryOp>
    Vector<T, N>
    binary_op(Vector<T, N> const& lhs, Vector<T, N> const& rhs, BinaryOp&& fun)
    {
        Vector<T, N> out{lhs};
        std::transform(out.data.begin(),
                       out.data.end(),
                       rhs.data.begin(),
                       out.data.begin(),
                       fun);
        return out;
    }

    template<typename T, std::size_t N>
    Vector<T, N>& operator+=(Vector<T, N>& lhs, Vector<T, N> const& rhs)
    {
        lhs = binary_op(
            std::move(lhs), rhs, [](auto a, auto b) { return a + b; });
        return lhs;
    }

    template<typename T, std::size_t N>
    Vector<T, N>& operator-=(Vector<T, N>& lhs, Vector<T, N> const& rhs)
    {
        lhs = binary_op(
            std::move(lhs), rhs, [](auto a, auto b) { return a - b; });
        return lhs;
    }

    template<typename T, std::size_t N>
    Vector<T, N>& operator*=(Vector<T, N>& lhs, T rhs)
    {
        lhs = unary_op(std::move(lhs), [rhs](auto a) { return a * rhs; });
        return lhs;
    }

    template<typename T, std::size_t N>
    Vector<T, N>& operator/=(Vector<T, N>& lhs, T rhs)
    {
        lhs = unary_op(std::move(lhs), [rhs](auto a) { return a / rhs; });
        return lhs;
    }

    template<typename T, std::size_t N>
    Vector<T, N> operator-(Vector<T, N> const& vec)
    {
        return unary_op(vec, [](auto a) { return -a; });
    }

    template<typename T, std::size_t N>
    bool operator==(Vector<T, N> const& lhs, Vector<T, N> const& rhs)
    {
        return lhs.data == rhs.data;
    }

    template<typename T, std::size_t N>
    bool operator!=(Vector<T, N> const& lhs, Vector<T, N> const& rhs)
    {
        return lhs.data != rhs.data;
    }

    template<typename T, std::size_t N>
    T dot(Vector<T, N> const& lhs, Vector<T, N> const& rhs)
    {
        return std::inner_product(
            lhs.data.begin(), lhs.data.end(), rhs.data.begin(), T{0});
    }

    template<typename T, std::size_t N>
    T length_squared(Vector<T, N> const& vec)
    {
        return dot(vec, vec);
    }

    template<typename T, std::size_t N>
    auto length(Vector<T, N> const& vec)
    {
        return static_cast<T>(std::sqrt(dot(vec, vec)));
    }

    template<typename T, std::size_t N>
    Vector<T, N> operator+(Vector<T, N> const& lhs, Vector<T, N> const& rhs)
    {
        Vector<T, N> out{lhs};
        out += rhs;
        return out;
    }

    template<typename T, std::size_t N>
    Vector<T, N> operator-(Vector<T, N> const& lhs, Vector<T, N> const& rhs)
    {
        Vector<T, N> out{lhs};
        out -= rhs;
        return out;
    }

    template<typename T, std::size_t N>
    Vector<T, N> operator*(Vector<T, N> const& lhs, T rhs)
    {
        Vector<T, N> out{lhs};
        out *= rhs;
        return out;
    }

    template<typename T, std::size_t N>
    Vector<T, N> operator*(T lhs, Vector<T, N> const& rhs)
    {
        Vector<T, N> out{rhs};
        out *= lhs;
        return out;
    }

    template<typename T, std::size_t N>
    Vector<T, N> operator/(Vector<T, N> const& lhs, T rhs)
    {
        Vector<T, N> out{lhs};
        out /= rhs;
        return out;
    }

    template<typename T, std::size_t N>
    Vector<T, N> abs(Vector<T, N> const& vec)
    {
        return unary_op(vec, [](auto a) { return std::abs(a); });
    }

    template<typename T, std::size_t N>
    T abs_dot(Vector<T, N> const& lhs, Vector<T, N> const& rhs)
    {
        return std::abs(dot(lhs, rhs));
    }

    template<typename T, std::size_t N>
    Vector<T, N> cross(Vector<T, N> const& u, Vector<T, N> const& v)
    {
        static_assert(N == 3);

        auto u_x{u[0]}, u_y{u[1]}, u_z{u[2]};
        auto v_x{v[0]}, v_y{v[1]}, v_z{v[2]};

        return Vector<T, N>{(u_y * v_z) - (u_z * v_y),
                            (u_z * v_x) - (u_x * v_z),
                            (u_x * v_y) - (u_y * v_x)};
    }

    template<typename T, std::size_t N>
    Vector<T, N> normalise(Vector<T, N> const& vec)
    {
        auto len{length(vec)};
        ASSERT(len != T{0});
        return vec / len;
    }

    template<typename T, std::size_t N>
    T min_component(Vector<T, N> const& v)
    {
        return *std::min_element(v.data.begin(), v.data.end());
    }

    template<typename T, std::size_t N>
    T max_component(Vector<T, N> const& v)
    {
        return *std::max_element(v.data.begin(), v.data.end());
    }

    template<typename T, std::size_t N>
    std::size_t max_dimension(Vector<T, N> const& v)
    {
        return std::distance(v.data.begin(),
                             std::max_element(v.data.begin(), v.data.end()));
    }

    template<typename T, std::size_t N>
    std::size_t min_dimension(Vector<T, N> const& v)
    {
        return std::distance(v.data.begin(),
                             std::min_element(v.data.begin(), v.data.end()));
    }

    template<typename T, std::size_t N>
    Vector<T, N> min(Vector<T, N> const& v, Vector<T, N> const& u)
    {
        return binary_op(v, u, [](auto a, auto b) { return std::min(a, b); });
    }

    template<typename T, std::size_t N>
    Vector<T, N> max(Vector<T, N> const& v, Vector<T, N> const& u)
    {
        return binary_op(v, u, [](auto a, auto b) { return std::max(a, b); });
    }

    template<typename T, std::size_t N, typename IndexType, typename... Args>
    Vector<T, N> permute(Vector<T, N> const& v, IndexType a, Args&&... args)
    {
        std::array<IndexType, sizeof...(args) + 1> indices{a, args...};
        static_assert(indices.size() == N);
        Vector<T, N> out;
        for (std::size_t i{0}; i < N; ++i)
        {
            out[i] = v[indices[i]];
        }

        return out;
    }

    template<typename T, std::size_t N>
    std::tuple<Vector<T, N>, Vector<T, N>>
    coordinate_system(Vector<T, N> const& v1)
    {
        static_assert(N == 3);

        ASSERT(length_squared(v1) == 1);

        Vector<T, N> v2{}, v3{};
        if (std::abs(v1[0]) > std::abs(v1[1]))
        {
            v2 = Vector<T, N>{-v1[2], T{0}, v1[0]} /
                 static_cast<T>(std::sqrt(v1[0] * v1[0] + v1[2] * v1[2]));
        }
        else
        {
            v2 = Vector<T, N>{T{0}, v1[2], -v1[1]} /
                 static_cast<T>(std::sqrt(v1[1] * v1[1] + v1[2] * v1[2]));
        }

        v3 = cross(v1, v2);
        return {v2, v3};
    }

    template<typename T, std::size_t N>
    T distance(Point<T, N> const& p1, Point<T, N> const& p2)
    {
        return length(p1 - p2);
    }

    template<typename T, std::size_t N>
    T distance_squared(Point<T, N> const& p1, Point<T, N> const& p2)
    {
        return length_squared(p1 - p2);
    }

    template<typename T, std::size_t N>
    std::ostream& operator<<(std::ostream& os, Vector<T, N> const& vec)
    {
        os << "( ";
        for (auto val : vec.data)
        {
            os << val << " ";
        }
        os << ")";
        return os;
    }

#if !defined(APOLLO_DISABLE_VECTOR_TEMPLATE_SPECIALISATIONS)
    // Disable warnings about anonymous structs.
#    if defined(ZEUS_COMPILER_MSVC)
#        pragma warning(push)
#        pragma warning(disable : 4201)
#    elif defined(ZEUS_COMPILER_CLANG)
#        pragma clang diagnostic push
#        pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#        pragma clang diagnostic ignored "-Wnested-anon-types"
#    elif defined(ZEUS_COMPILER_GCC)
#        pragma GCC diagnostic push
#        pragma GCC diagnostic ignored "-Wpedantic"
#    endif

    // Useful specializations.
    template<typename T>
    class Vector<T, 2>
    {
    public:
        using value_type = T;
        static constexpr auto dimension{2};

        Vector() : data{0, 0}
        {}

        explicit Vector(T a_x) : x{a_x}, y{a_x}
        {
            ASSERT(!has_nans(*this));
        }

        explicit Vector(T a_x, T a_y) : x{a_x}, y{a_y}
        {
            ASSERT(!has_nans(*this));
        }

        T& operator[](std::size_t i)
        {
            ASSERT_MSG(i < dimension, "invalid index");
            return data[i];
        }

        T operator[](std::size_t i) const
        {
            ASSERT_MSG(i < dimension, "invalid index");
            return data[i];
        }

        union
        {
            std::array<T, 2> data;
            struct
            {
                T x, y;
            };
        };
    };

    template<typename T>
    class Vector<T, 3>
    {
    public:
        using value_type = T;
        static constexpr auto dimension{3};

        Vector() : data{0, 0, 0}
        {}

        explicit Vector(T a) : data{a, a, a}
        {
            ASSERT(!has_nans(*this));
        }

        explicit Vector(T x, T y, T z) : data{x, y, z}
        {
            ASSERT(!has_nans(*this));
        }

        T& operator[](std::size_t i)
        {
            ASSERT_MSG(i < dimension, "invalid index");
            return data[i];
        }

        T operator[](std::size_t i) const
        {
            ASSERT_MSG(i < dimension, "invalid index");
            return data[i];
        }

        union
        {
            std::array<T, 3> data;
            struct
            {
                T x, y, z;
            };
        };
    };

    template<typename T>
    class Vector<T, 4>
    {
    public:
        using value_type = T;
        static constexpr auto dimension{4};

        Vector() : data{0, 0, 0, 0}
        {}

        explicit Vector(T a) : data{a, a, a, a}
        {
            ASSERT(!has_nans(*this));
        }

        explicit Vector(T x, T y, T z, T w) : data{x, y, z, w}
        {
            ASSERT(!has_nans(*this));
        }

        T& operator[](std::size_t i)
        {
            ASSERT_MSG(i < dimension, "invalid index");
            return data[i];
        }

        T operator[](std::size_t i) const
        {
            ASSERT_MSG(i < dimension, "invalid index");
            return data[i];
        }

        union
        {
            std::array<T, 4> data;
            struct
            {
                T x, y, z, w;
            };
        };
    };

#    if defined(ZEUS_COMPILER_MSVC)
#        pragma warning(pop)
#    elif defined(ZEUS_COMPILER_CLANG)
#        pragma clang diagnostic pop
#    elif defined(ZEUS_COMPILER_GCC)
#        pragma GCC diagnostic pop
#    endif
#endif

    template<typename T>
    using Vector2 = Vector<T, 2>;
    template<typename T>
    using Vector3 = Vector<T, 3>;
    template<typename T>
    using Vector4 = Vector<T, 4>;

    template<typename T>
    using Point2 = Point<T, 2>;
    template<typename T>
    using Point3 = Point<T, 3>;
    template<typename T>
    using Point4 = Point<T, 4>;

    template<typename T>
    using Normal3 = Normal<T, 3>;
} // namespace core
