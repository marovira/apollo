#include <core/utils.hpp>

#include <catch2/catch.hpp>
#include <type_traits>

TEMPLATE_TEST_CASE("[core] is_nan", "", float, double, int, unsigned int)
{
    if constexpr (std::is_floating_point<TestType>::value)
    {
        auto nan = std::numeric_limits<TestType>::quiet_NaN();
        auto val = TestType{1};

        REQUIRE(core::is_nan(nan));
        REQUIRE_FALSE(core::is_nan(val));
    }
    else
    {
        auto val = TestType{1};
        REQUIRE_FALSE(core::is_nan(val));
    }
}
