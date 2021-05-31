#include <core/matrix.hpp>

#include <catch2/catch.hpp>

static constexpr auto N{core::Matrix<float>::size};

TEMPLATE_TEST_CASE("[Matrix] - constructors", "[core]", float, double)
{
    SECTION("Empty constructor")
    {
        core::Matrix<TestType> m;
        for (std::size_t i{0}; i < N; ++i)
        {
            REQUIRE(m.data[i] == TestType{0});
        }
    }

    SECTION("Single value constructor")
    {
        core::Matrix<TestType> m(TestType{1});
        for (std::size_t i{0}, diag_idx{0}; i < N; ++i)
        {
            if (i == diag_idx)
            {
                REQUIRE(m.data[i] == TestType{1});
                diag_idx += 5;
                continue;
            }

            REQUIRE(m.data[i] == TestType{0});
        }
    }

    SECTION("Initializer list constructor")
    {
        // clang-format off
        core::Matrix<TestType> m{
            TestType{1},  TestType{2},  TestType{3},  TestType{4},
            TestType{5},  TestType{6},  TestType{7},  TestType{8},
            TestType{9},  TestType{10}, TestType{11}, TestType{12},
            TestType{13}, TestType{14}, TestType{15}, TestType{16}};
        // clang-format on

        for (std::size_t i{0}; i < N; ++i)
        {
            REQUIRE(m.data[i] == static_cast<TestType>(i + 1));
        }
    }

    SECTION("Array constructor")
    {
        // clang-format off
        std::array<TestType, N> data{
            TestType{1},  TestType{2},  TestType{3},  TestType{4},
            TestType{5},  TestType{6},  TestType{7},  TestType{8},
            TestType{9},  TestType{10}, TestType{11}, TestType{12},
            TestType{13}, TestType{14}, TestType{15}, TestType{16}};
        // clang-format on
        core::Matrix<TestType> m{data};

        for (std::size_t i{0}; i < N; ++i)
        {
            REQUIRE(m.data[i] == static_cast<TestType>(i + 1));
        }
    }
}

TEMPLATE_TEST_CASE("[Matrix] - vector accessors", "[core]", float, double)
{
    const core::Matrix<TestType> m(TestType{1});
    // clang-format off
    const std::array<core::Vector<TestType, 4>, 4> expected
    {
        core::Vector<TestType, 4>{TestType{1}, TestType{0}, TestType{0}, TestType{0}},
        core::Vector<TestType, 4>{TestType{0}, TestType{1}, TestType{0}, TestType{0}},
        core::Vector<TestType, 4>{TestType{0}, TestType{0}, TestType{1}, TestType{0}},
        core::Vector<TestType, 4>{TestType{0}, TestType{0}, TestType{0}, TestType{1}},
    };
    // clang-format on

    SECTION("Row")
    {
        for (std::size_t i{0}; i < 4; ++i)
        {
            auto row = m.row(i);
            REQUIRE(row == expected[i]);
        }
    }

    SECTION("Column")
    {
        for (std::size_t i{0}; i < 4; ++i)
        {
            auto col = m.col(i);
            REQUIRE(col == expected[i]);
        }
    }
}

TEMPLATE_TEST_CASE("[Matrix] - operator()", "[core]", float, double)
{
    SECTION("Non-const version")
    {
        core::Matrix<TestType> m(TestType{1});

        for (std::size_t r{0}; r < 4; ++r)
        {
            for (std::size_t c{0}; c < 4; ++c)
            {
                if (r == c)
                {
                    REQUIRE(m(r, c) == TestType{1});
                    continue;
                }

                REQUIRE(m(r, c) == TestType{0});
            }
        }
    }

    SECTION("Const version")
    {
        const core::Matrix<TestType> m(TestType{1});

        for (std::size_t r{0}; r < 4; ++r)
        {
            for (std::size_t c{0}; c < 4; ++c)
            {
                if (r == c)
                {
                    REQUIRE(m(r, c) == TestType{1});
                    continue;
                }

                REQUIRE(m(r, c) == TestType{0});
            }
        }
    }
}
