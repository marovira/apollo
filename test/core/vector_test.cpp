#include <core/vector.hpp>

#include <catch2/catch.hpp>
#include <type_traits>

static constexpr auto N{3};

TEMPLATE_TEST_CASE("[Vector] - constructors", "", float, double, int)
{
    SECTION("Empty constructor")
    {
        core::Vector<TestType, N> v;

        REQUIRE(v.data[0] == TestType{0});
        REQUIRE(v.data[1] == TestType{0});
        REQUIRE(v.data[2] == TestType{0});
    }

    SECTION("Uniform constructor")
    {
        core::Vector<TestType, N> v{TestType{1}};

        REQUIRE(v.data[0] == TestType{1});
        REQUIRE(v.data[1] == TestType{1});
        REQUIRE(v.data[2] == TestType{1});
    }

    SECTION("Parameterised constructor")
    {
        core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};

        REQUIRE(v.data[0] == TestType{1});
        REQUIRE(v.data[1] == TestType{2});
        REQUIRE(v.data[2] == TestType{3});
    }

    SECTION("Copy-constructor: N - 1")
    {
        core::Vector<TestType, N> v{TestType{1}};
        core::Vector<TestType, N + 1> u{v};

        REQUIRE(u.data[0] == TestType{1});
        REQUIRE(u.data[1] == TestType{1});
        REQUIRE(u.data[2] == TestType{1});
    }

    SECTION("Copy-constructor: N + 1")
    {
        core::Vector<TestType, N + 1> v{TestType{1}};
        core::Vector<TestType, N> u{v};

        REQUIRE(u.data[0] == TestType{1});
        REQUIRE(u.data[1] == TestType{1});
        REQUIRE(u.data[2] == TestType{1});
    }
}

TEMPLATE_TEST_CASE("[Vector] - operator[]", "", float, double, int)
{
    SECTION("Non-const version")
    {
        core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};

        REQUIRE(v[0] == TestType{1});
        REQUIRE(v[1] == TestType{2});
        REQUIRE(v[2] == TestType{3});
    }

    SECTION("Const version")
    {
        const core::Vector<TestType, N> v{
            TestType{1}, TestType{2}, TestType{3}};

        REQUIRE(v[0] == TestType{1});
        REQUIRE(v[1] == TestType{2});
        REQUIRE(v[2] == TestType{3});
    }
}

TEMPLATE_TEST_CASE("[Vector] - has_nans", "", float, double)
{
    core::Vector<TestType, N> v;
    auto nan = std::numeric_limits<TestType>::quiet_NaN();

    REQUIRE_FALSE(core::has_nans(v));

    v[0] = nan;
    REQUIRE(core::has_nans(v));

    v[1] = nan;
    REQUIRE(core::has_nans(v));

    v[2] = nan;
    REQUIRE(core::has_nans(v));
}

TEMPLATE_TEST_CASE("[Vector] - unary_op", "", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};

    SECTION("R-value version")
    {
        auto result = core::unary_op(
            std::move(v), [](auto val) { return val + TestType{1}; });

        REQUIRE(result[0] == TestType{2});
        REQUIRE(result[1] == TestType{3});
        REQUIRE(result[2] == TestType{4});
    }

    SECTION("L-value version")
    {
        auto result =
            core::unary_op(v, [](auto val) { return val + TestType{1}; });

        REQUIRE(result[0] == TestType{2});
        REQUIRE(result[1] == TestType{3});
        REQUIRE(result[2] == TestType{4});
    }
}

TEMPLATE_TEST_CASE("[Vector] - binary_op", "", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};
    core::Vector<TestType, N> u{TestType{1}};

    SECTION("R-value version")
    {
        auto result = core::binary_op(
            std::move(v), u, [](auto a, auto b) { return a + b; });

        REQUIRE(result[0] == TestType{2});
        REQUIRE(result[1] == TestType{3});
        REQUIRE(result[2] == TestType{4});
    }

    SECTION("L-value version")
    {
        auto result =
            core::binary_op(v, u, [](auto a, auto b) { return a + b; });

        REQUIRE(result[0] == TestType{2});
        REQUIRE(result[1] == TestType{3});
        REQUIRE(result[2] == TestType{4});
    }
}
