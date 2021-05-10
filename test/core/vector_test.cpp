#define APOLLO_DISABLE_VECTOR_TEMPLATE_SPECIALISATIONS
#include <core/vector.hpp>

#include <catch2/catch.hpp>

static constexpr auto N{3};

TEMPLATE_TEST_CASE("[Vector] - constructors", "[core]", float, double, int)
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

TEMPLATE_TEST_CASE("[Vector] - operator[]", "[core]", float, double, int)
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

TEMPLATE_TEST_CASE("[Vector] - has_nans", "[core]", float, double)
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

TEMPLATE_TEST_CASE("[Vector] - unary_op", "[core]", float, double, int)
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

TEMPLATE_TEST_CASE("[Vector] - binary_op", "[core]", float, double, int)
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

TEMPLATE_TEST_CASE("[Vector] - operator+=", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};
    core::Vector<TestType, N> u{TestType{1}};

    u += v;

    REQUIRE(u[0] == TestType{2});
    REQUIRE(u[1] == TestType{3});
    REQUIRE(u[2] == TestType{4});
}

TEMPLATE_TEST_CASE("[Vector] - operator-=", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};
    core::Vector<TestType, N> u{TestType{1}};

    u -= v;

    REQUIRE(u[0] == TestType{0});
    REQUIRE(u[1] == TestType{-1});
    REQUIRE(u[2] == TestType{-2});
}

TEMPLATE_TEST_CASE("[Vector] - operator*=", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};
    TestType val{2};

    v *= val;

    REQUIRE(v[0] == TestType{2});
    REQUIRE(v[1] == TestType{4});
    REQUIRE(v[2] == TestType{6});
}

TEMPLATE_TEST_CASE("[Vector] - operator/=", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{2}, TestType{4}, TestType{6}};
    TestType val{2};

    v /= val;

    REQUIRE(v[0] == TestType{1});
    REQUIRE(v[1] == TestType{2});
    REQUIRE(v[2] == TestType{3});
}

TEMPLATE_TEST_CASE("[Vector] - operator-", "[core]", float, double, int)
{
    SECTION("Unary operator")
    {
        core::Vector<TestType, N> v{TestType{1}};

        v = -v;

        REQUIRE(v[0] == TestType{-1});
        REQUIRE(v[1] == TestType{-1});
        REQUIRE(v[2] == TestType{-1});
    }

    SECTION("Binary operator")
    {
        core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};
        core::Vector<TestType, N> u{TestType{1}};

        auto w = u - v;

        REQUIRE(w[0] == TestType{0});
        REQUIRE(w[1] == TestType{-1});
        REQUIRE(w[2] == TestType{-2});
    }
}

TEMPLATE_TEST_CASE("[Vector] - operator==", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}};
    core::Vector<TestType, N> u{TestType{1}};
    core::Vector<TestType, N> w;

    REQUIRE(v == v);
    REQUIRE(v == u);
    REQUIRE_FALSE(v == w);
}

TEMPLATE_TEST_CASE("[Vector] - operator!=", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}};
    core::Vector<TestType, N> u{TestType{1}};
    core::Vector<TestType, N> w;

    REQUIRE_FALSE(v != v);
    REQUIRE_FALSE(v != u);
    REQUIRE(v != w);
}

TEMPLATE_TEST_CASE("[Vector] - dot", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}};
    core::Vector<TestType, N> u{TestType{2}};

    auto dot = core::dot(v, u);

    REQUIRE(dot == TestType{6});
}

TEMPLATE_TEST_CASE("[Vector] - length_squared", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{2}};

    auto l = core::length_squared(v);

    REQUIRE(l == TestType{12});
}

TEMPLATE_TEST_CASE("[Vector] - length", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{2}};

    auto l = core::length(v);

    REQUIRE(l == TestType{3});
}

TEMPLATE_TEST_CASE("[Vector] - operator+", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};
    core::Vector<TestType, N> u{TestType{1}};

    auto w = u + v;

    REQUIRE(w[0] == TestType{2});
    REQUIRE(w[1] == TestType{3});
    REQUIRE(w[2] == TestType{4});
}

TEMPLATE_TEST_CASE("[Vector] - operator*", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};
    TestType val{2};

    SECTION("Vector * scalar")
    {
        auto u = v * val;

        REQUIRE(u[0] == TestType{2});
        REQUIRE(u[1] == TestType{4});
        REQUIRE(u[2] == TestType{6});
    }

    SECTION("Scalar * vector")
    {
        auto u = val * v;

        REQUIRE(u[0] == TestType{2});
        REQUIRE(u[1] == TestType{4});
        REQUIRE(u[2] == TestType{6});
    }
}

TEMPLATE_TEST_CASE("[Vector] - operator/", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{2}, TestType{4}, TestType{6}};
    TestType val{2};

    auto u = v / val;

    REQUIRE(u[0] == TestType{1});
    REQUIRE(u[1] == TestType{2});
    REQUIRE(u[2] == TestType{3});
}

TEMPLATE_TEST_CASE("[Vector] - abs", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{-1}, TestType{2}, TestType{-3}};

    auto u = core::abs(v);

    REQUIRE(u[0] == TestType{1});
    REQUIRE(u[1] == TestType{2});
    REQUIRE(u[2] == TestType{3});
}

TEMPLATE_TEST_CASE("[Vector] - abs_dot", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{-1}};
    core::Vector<TestType, N> u{TestType{2}};

    auto dot = core::abs_dot(v, u);

    REQUIRE(dot == TestType{6});
}

TEMPLATE_TEST_CASE("[Vector] - cross", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}},
        u{TestType{4}, TestType{5}, TestType{6}};

    auto w = core::cross(v, u);

    REQUIRE(w[0] == -3.0f);
    REQUIRE(w[1] == 6.0f);
    REQUIRE(w[2] == -3.0f);
}

TEMPLATE_TEST_CASE("[Vector] - normalise", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{2}};
    auto u = core::normalise(v);

    REQUIRE(u[0] == TestType{1} / TestType{3});
    REQUIRE(u[1] == TestType{2} / TestType{3});
    REQUIRE(u[2] == TestType{2} / TestType{3});
}

TEMPLATE_TEST_CASE("[Vector] - min_component", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};

    auto min = core::min_component(v);

    REQUIRE(min == TestType{1});
}

TEMPLATE_TEST_CASE("[Vector] - max_component", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};

    auto m = core::max_component(v);

    REQUIRE(m == TestType{3});
}

TEMPLATE_TEST_CASE("[Vector] - max_dimension", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};

    auto m = core::max_dimension(v);

    REQUIRE(m == 2);
}

TEMPLATE_TEST_CASE("[Vector] - min_dimension", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};

    auto m = core::min_dimension(v);

    REQUIRE(m == 0);
}

TEMPLATE_TEST_CASE("[Vector] - min", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}},
        u{TestType{0}, TestType{3}, TestType{1}};

    auto w = core::min(v, u);

    REQUIRE(w[0] == TestType{0});
    REQUIRE(w[1] == TestType{2});
    REQUIRE(w[2] == TestType{1});
}

TEMPLATE_TEST_CASE("[Vector] - max", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}},
        u{TestType{0}, TestType{3}, TestType{1}};

    auto w = core::max(v, u);

    REQUIRE(w[0] == TestType{1});
    REQUIRE(w[1] == TestType{3});
    REQUIRE(w[2] == TestType{3});
}

TEMPLATE_TEST_CASE("[Vector] - permute", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{2}, TestType{3}};

    auto u = core::permute(v, 1, 0, 2);

    REQUIRE(u[0] == TestType{2});
    REQUIRE(u[1] == TestType{1});
    REQUIRE(u[2] == TestType{3});
}

TEMPLATE_TEST_CASE("[Vector] - coordinate_system", "[core]", float, double, int)
{
    core::Vector<TestType, N> v{TestType{1}, TestType{0}, TestType{0}};

    auto [u, w] = core::coordinate_system(v);
    u           = core::abs(u);
    w           = core::abs(w);
    REQUIRE(u[0] == TestType{0});
    REQUIRE(u[1] == TestType{0});
    REQUIRE(u[2] == TestType{1});
    REQUIRE(w[0] == TestType{0});
    REQUIRE(w[1] == TestType{1});
    REQUIRE(w[2] == TestType{0});
}

TEMPLATE_TEST_CASE("[Vector] - distance", "[core]", float, double, int)
{
    core::Point<TestType, N> p1{TestType{2}, TestType{0}, TestType{0}},
        p2{TestType{0}};

    auto d = core::distance(p1, p2);
    REQUIRE(d == TestType{d});
}

TEMPLATE_TEST_CASE("[vector] - distance_squared", "[core]", float, double, int)
{
    core::Point<TestType, N> p1{TestType{2}, TestType{0}, TestType{0}},
        p2{TestType{0}};

    auto d = core::distance_squared(p1, p2);

    REQUIRE(d == TestType{d});
}
