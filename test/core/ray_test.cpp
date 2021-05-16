#include <core/ray.hpp>

#include <catch2/catch.hpp>

TEMPLATE_TEST_CASE("[Ray] - constructors", "[core]", float, double)
{
    SECTION("Empty constructor")
    {
        core::Ray<TestType> r;

        REQUIRE(r.o == core::Point3<TestType>{});
        REQUIRE(r.d == core::Vector3<TestType>{});
    }

    SECTION("Parametrised constructor")
    {
        core::Point3<TestType> origin{TestType{1}, TestType{0}, TestType{1}};
        core::Vector3<TestType> dir{TestType{1}, TestType{1}, TestType{1}};
        core::Ray<TestType> r{origin, dir};

        REQUIRE(r.o == origin);
        REQUIRE(r.d == dir);
    }
}

TEMPLATE_TEST_CASE("[Ray] - operator()", "[core]", float, double)
{
    core::Ray<TestType> r{core::Point3<TestType>{TestType{0}},
                          core::Vector3<TestType>{TestType{1}}};
    auto val = r(TestType{1});

    REQUIRE(val == core::Point3<TestType>{TestType{1}});
}
