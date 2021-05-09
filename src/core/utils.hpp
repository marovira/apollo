#pragma once

#include <cmath>

namespace core
{
    template<typename T>
    constexpr bool is_nan(T x)
    {
        return std::isnan(x);
    }

    template<>
    constexpr bool is_nan(const int)
    {
        return false;
    }

    template<>
    constexpr bool is_nan(const unsigned int)
    {
        return false;
    }
}