#include <core/vector.hpp>

#include <catch2/catch.hpp>

TEMPLATE_TEST_CASE(
    "[Vector] - specialised constructors", "[core]", float, double, int)
{
    SECTION("Empty constructor: size 2")
    {
        core::Vector2<TestType> v;

        REQUIRE(v.x == TestType{0});
        REQUIRE(v.y == TestType{0});
    }

    SECTION("Empty constructor: size 3")
    {
        core::Vector3<TestType> v;

        REQUIRE(v.x == TestType{0});
        REQUIRE(v.y == TestType{0});
        REQUIRE(v.z == TestType{0});
    }

    SECTION("Empty constructor: size 4")
    {
        core::Vector4<TestType> v;

        REQUIRE(v.x == TestType{0});
        REQUIRE(v.y == TestType{0});
        REQUIRE(v.z == TestType{0});
        REQUIRE(v.w == TestType{0});
    }

    SECTION("Uniform constructor: size 2")
    {
        core::Vector2<TestType> v{TestType{1}};

        REQUIRE(v.x == TestType{1});
        REQUIRE(v.y == TestType{1});
    }

    SECTION("Uniform constructor: size 3")
    {
        core::Vector3<TestType> v{TestType{1}};

        REQUIRE(v.x == TestType{1});
        REQUIRE(v.y == TestType{1});
        REQUIRE(v.z == TestType{1});
    }

    SECTION("Uniform constructor: size 2")
    {
        core::Vector4<TestType> v{TestType{1}};

        REQUIRE(v.x == TestType{1});
        REQUIRE(v.y == TestType{1});
        REQUIRE(v.z == TestType{1});
        REQUIRE(v.w == TestType{1});
    }

    SECTION("Parameterised constructor: size 2")
    {
        core::Vector2<TestType> v{TestType{1}, TestType{2}};

        REQUIRE(v.x == TestType{1});
        REQUIRE(v.y == TestType{2});
    }

    SECTION("Parameterised constructor: size 3")
    {
        core::Vector3<TestType> v{TestType{1}, TestType{2}, TestType{3}};

        REQUIRE(v.x == TestType{1});
        REQUIRE(v.y == TestType{2});
        REQUIRE(v.z == TestType{3});
    }

    SECTION("Parameterised constructor: size 4")
    {
        core::Vector4<TestType> v{
            TestType{1}, TestType{2}, TestType{3}, TestType{4}};

        REQUIRE(v.x == TestType{1});
        REQUIRE(v.y == TestType{2});
        REQUIRE(v.z == TestType{3});
        REQUIRE(v.w == TestType{4});
    }
}

TEMPLATE_TEST_CASE(
    "[Vector] - specialised operator[]", "[core]", float, double, int)
{
    SECTION("Non-const version: size 2")
    {
        core::Vector2<TestType> v{TestType{1}, TestType{2}};

        REQUIRE(v[0] == TestType{1});
        REQUIRE(v[1] == TestType{2});
    }

    SECTION("Const version: size 2")
    {
        const core::Vector2<TestType> v{TestType{1}, TestType{2}};

        REQUIRE(v[0] == TestType{1});
        REQUIRE(v[1] == TestType{2});
    }

    SECTION("Non-const version: size 3")
    {
        core::Vector3<TestType> v{TestType{1}, TestType{2}, TestType{3}};

        REQUIRE(v[0] == TestType{1});
        REQUIRE(v[1] == TestType{2});
        REQUIRE(v[2] == TestType{3});
    }

    SECTION("Const version: size 3")
    {
        const core::Vector3<TestType> v{TestType{1}, TestType{2}, TestType{3}};

        REQUIRE(v[0] == TestType{1});
        REQUIRE(v[1] == TestType{2});
        REQUIRE(v[2] == TestType{3});
    }

    SECTION("Non-const version: size 4")
    {
        core::Vector4<TestType> v{
            TestType{1}, TestType{2}, TestType{3}, TestType{4}};

        REQUIRE(v[0] == TestType{1});
        REQUIRE(v[1] == TestType{2});
        REQUIRE(v[2] == TestType{3});
        REQUIRE(v[3] == TestType{4});
    }

    SECTION("Const version: size 4")
    {
        const core::Vector4<TestType> v{
            TestType{1}, TestType{2}, TestType{3}, TestType{4}};

        REQUIRE(v[0] == TestType{1});
        REQUIRE(v[1] == TestType{2});
        REQUIRE(v[2] == TestType{3});
        REQUIRE(v[3] == TestType{4});
    }
}
