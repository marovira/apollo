#include <core/matrix.hpp>

#include <catch2/catch.hpp>
#include <zeus/float.hpp>

TEMPLATE_TEST_CASE("[Matrix] - constructors", "[core]", float, double)
{
    SECTION("Empty constructor")
    {
        core::Matrix<TestType> m;
        for (std::size_t i{0}; i < 16; ++i)
        {
            REQUIRE(m.data[i] == TestType{0});
        }
    }

    SECTION("Single value constructor")
    {
        core::Matrix<TestType> m(TestType{1});
        for (std::size_t i{0}, diag_idx{0}; i < 16; ++i)
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

        for (std::size_t i{0}; i < 16; ++i)
        {
            REQUIRE(m.data[i] == static_cast<TestType>(i + 1));
        }
    }

    SECTION("Array constructor")
    {
        // clang-format off
        std::array<TestType, 16> data{
            TestType{1},  TestType{2},  TestType{3},  TestType{4},
            TestType{5},  TestType{6},  TestType{7},  TestType{8},
            TestType{9},  TestType{10}, TestType{11}, TestType{12},
            TestType{13}, TestType{14}, TestType{15}, TestType{16}};
        // clang-format on
        core::Matrix<TestType> m{data};

        for (std::size_t i{0}; i < 16; ++i)
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

TEMPLATE_TEST_CASE("[Matrix] - unary_op", "[core]", float, double)
{
    core::Matrix<TestType> m;

    SECTION("R-value version")
    {
        auto result = core::unary_op(
            std::move(m), [](TestType a) { return a + TestType{1}; });
        for (std::size_t i{0}; i < 16; ++i)
        {
            REQUIRE(result.data[i] == TestType{1});
        }
    }

    SECTION("L-value version")
    {
        auto result =
            core::unary_op(m, [](TestType a) { return a + TestType{1}; });
        for (std::size_t i{0}; i < 16; ++i)
        {
            REQUIRE(result.data[i] == TestType{1});
        }
    }
}

TEMPLATE_TEST_CASE("[Matrix] - binary_op", "[core", float, double)
{
    // clang-format off
    core::Matrix<TestType> m1;
    core::Matrix<TestType> m2{
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1}
    };
    // clang-format on

    SECTION("R-value version")
    {
        auto result = core::binary_op(
            std::move(m1), m2, [](auto a, auto b) { return a + b; });

        for (std::size_t i{0}; i < 16; ++i)
        {
            REQUIRE(result.data[i] == TestType{1});
        }
    }

    SECTION("L-value version")
    {
        auto result =
            core::binary_op(m1, m2, [](auto a, auto b) { return a + b; });

        for (std::size_t i{0}; i < 16; ++i)
        {
            REQUIRE(result.data[i] == TestType{1});
        }
    }
}

TEMPLATE_TEST_CASE("[Matrix] - operator +=", "[core]", float, double)
{
    // clang-format off
    core::Matrix<TestType> m1;
    core::Matrix<TestType> m2{
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1}
    };
    // clang-format on

    m1 += m2;
    for (std::size_t i{0}; i < 16; ++i)
    {
        REQUIRE(m1.data[i] == TestType{1});
    }
}

TEMPLATE_TEST_CASE("[Matrix] - operator -=", "[core]", float, double)
{
    // clang-format off
    core::Matrix<TestType> m1;
    core::Matrix<TestType> m2{
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1}
    };
    // clang-format on

    m1 -= m2;
    for (std::size_t i{0}; i < 16; ++i)
    {
        REQUIRE(m1.data[i] == TestType{-1});
    }
}

TEMPLATE_TEST_CASE("[Matrix] - operator *=", "[core]", float, double)
{
    // clang-format off
    core::Matrix<TestType> m1{
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1}
    };

    core::Matrix<TestType> m2{
        TestType{2}, TestType{2}, TestType{2}, TestType{2},
        TestType{2}, TestType{2}, TestType{2}, TestType{2},
        TestType{2}, TestType{2}, TestType{2}, TestType{2},
        TestType{2}, TestType{2}, TestType{2}, TestType{2}
    };
    // clang-format on

    SECTION("Matrix * scalar")
    {
        TestType a{2};

        m1 *= a;

        for (std::size_t i{0}; i < 16; ++i)
        {
            REQUIRE(m1.data[i] == TestType{2});
        }
    }

    SECTION("Matrix * matrix")
    {
        m1 *= m2;

        for (std::size_t i{0}; i < 16; ++i)
        {
            REQUIRE(m1.data[i] == TestType{8});
        }
    }
}

TEMPLATE_TEST_CASE("[Matrix] - operator -", "[core]", float, double)
{
    // clang-format off
    core::Matrix<TestType> m1;
    core::Matrix<TestType> m2{
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1}
    };
    // clang-format on

    SECTION("Unary version")
    {
        auto result = -m2;
        for (std::size_t i{0}; i < 16; ++i)
        {
            REQUIRE(result.data[i] == TestType{-1});
        }
    }

    SECTION("Binary version")
    {
        auto result = m1 - m2;

        for (std::size_t i{0}; i < 16; ++i)
        {
            REQUIRE(result.data[i] == TestType{-1});
        }
    }
}

TEMPLATE_TEST_CASE("[Matrix] - operator==", "[core]", float, double)
{
    core::Matrix<TestType> m1, m2, m3(TestType{1});

    REQUIRE(m1 == m1);
    REQUIRE(m1 == m2);
    REQUIRE_FALSE(m1 == m3);
}

TEMPLATE_TEST_CASE("[Matrix] - operator!=", "[core]", float, double)
{
    core::Matrix<TestType> m1, m2, m3(TestType{1});

    REQUIRE_FALSE(m1 != m1);
    REQUIRE_FALSE(m1 != m2);
    REQUIRE(m1 != m3);
}

TEMPLATE_TEST_CASE("[Matrix] - operator+", "[core]", float, double)
{
    // clang-format off
    core::Matrix<TestType> m1;
    core::Matrix<TestType> m2{
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1}
    };
    // clang-format on

    auto result = m1 + m2;

    REQUIRE(result == m2);
}

TEMPLATE_TEST_CASE("[Matrix] - operator*", "[core]", float, double)
{
    // clang-format off
    core::Matrix<TestType> m1{
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1},
        TestType{1}, TestType{1}, TestType{1}, TestType{1}
    };

    core::Matrix<TestType> m2{
        TestType{2}, TestType{2}, TestType{2}, TestType{2},
        TestType{2}, TestType{2}, TestType{2}, TestType{2},
        TestType{2}, TestType{2}, TestType{2}, TestType{2},
        TestType{2}, TestType{2}, TestType{2}, TestType{2}
    };
    // clang-format on

    SECTION("Matrix * scalar")
    {
        auto result = m1 * TestType{2};

        REQUIRE(result == m2);
    }

    SECTION("Scalar * matrix")
    {
        auto result = TestType{2} * m1;

        REQUIRE(result == m2);
    }

    SECTION("Matrix * matrix")
    {
        auto result = m1 * m2;

        for (std::size_t i{0}; i < 16; ++i)
        {
            REQUIRE(result.data[i] == TestType{8});
        }
    }
}

TEMPLATE_TEST_CASE("[Matrix] - transpose", "[core]", float, double)
{
    SECTION("Transpose of 0 matrix")
    {
        core::Matrix<TestType> m;
        auto t = core::transpose(m);

        REQUIRE(t == m);
    }

    SECTION("Transpose of identity")
    {
        core::Matrix<TestType> m(TestType{1});
        auto t = core::transpose(m);

        REQUIRE(t == m);
    }

    SECTION("Transpose of regular matrix")
    {
        // clang-format off
        core::Matrix<TestType> m{
            TestType{1},  TestType{2},  TestType{3},  TestType{4},
            TestType{5},  TestType{6},  TestType{7},  TestType{8},
            TestType{9},  TestType{10}, TestType{11}, TestType{12},
            TestType{13}, TestType{14}, TestType{15}, TestType{16}};
        core::Matrix<TestType> m2{
            TestType{1}, TestType{5}, TestType{9},  TestType{13},
            TestType{2}, TestType{6}, TestType{10}, TestType{14},
            TestType{3}, TestType{7}, TestType{11}, TestType{15},
            TestType{4}, TestType{8}, TestType{12}, TestType{16}};
        // clang-format on

        auto t = core::transpose(m);
        REQUIRE(t == m2);
    }
}

TEMPLATE_TEST_CASE("[Matrix] - inverse", "[core]", float, double)
{
    SECTION("Inverse of a 0 matrix")
    {
        core::Matrix<TestType> m;
        REQUIRE_THROWS_AS(core::inverse(m), std::runtime_error);
    }

    SECTION("Inverse of a singular matrix")
    {
        // clang-format off
        core::Matrix<TestType> m{
            TestType{1},  TestType{2},  TestType{3},  TestType{4},
            TestType{5},  TestType{6},  TestType{7},  TestType{8},
            TestType{9},  TestType{10}, TestType{11}, TestType{12},
            TestType{13}, TestType{14}, TestType{15}, TestType{16}};
        // clang-format on

        REQUIRE_THROWS_AS(core::inverse(m), std::runtime_error);
    }

    SECTION("Inverse of identity matrix")
    {
        core::Matrix<TestType> m(TestType{1});
        auto inv = core::inverse(m);

        REQUIRE(inv == m);
    }

    SECTION("Inverse of regular matrix")
    {
        core::Matrix<TestType> m(TestType{4});
        m(0, 3) = TestType{14};
        m(1, 3) = TestType{14};
        m(2, 3) = TestType{14};
        m(3, 3) = TestType{1};

        core::Matrix<TestType> exp(TestType{1} / TestType{4});
        const TestType val = TestType{-7} / TestType{2};
        exp(0, 3)          = val;
        exp(1, 3)          = val;
        exp(2, 3)          = val;
        exp(3, 3)          = 1;

        auto inv = core::inverse(m);

        auto eps = []() {
            return std::numeric_limits<TestType>::epsilon() * 100;
        };

        for (std::size_t i{0}; i < 16; ++i)
        {
            REQUIRE(zeus::are_equal<TestType, eps>(inv.data[i], exp.data[i]));
        }
    }
}
