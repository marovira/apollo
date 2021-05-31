#pragma once

#include "real.hpp"
#include "utils.hpp"
#include "vector.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <zeus/assert.hpp>
#include <zeus/compiler.hpp>

namespace core
{
    template<typename T>
    class Matrix
    {
    public:
        using value_type = T;
        static constexpr auto num_rows{4};
        static constexpr auto num_cols{4};
        static constexpr auto size{16};

        Matrix()
        {
            std::memset(data.data(), 0, size * sizeof(T));
        }

        explicit Matrix(T diag) : Matrix{}
        {
            ASSERT(!is_nan(diag));
            data[0]  = diag;
            data[5]  = diag;
            data[10] = diag;
            data[15] = diag;
        }

        explicit Matrix(std::initializer_list<T> const& list)
        {
            ASSERT(list.size() == size);
            std::size_t i{0};
            for (auto val : list)
            {
                ASSERT(!is_nan(val));
                data[i] = val;
                ++i;
            }
        }

#if defined(ZEUS_BUILD_DEBUG)
        explicit Matrix(std::array<T, 16> const& list)
        {
            std::size_t i{0};
            for (auto val : list)
            {
                ASSERT(!is_nan(val));
                data[i] = val;
                ++i;
            }
        }
#else
        explicit Matrix(std::array<T, 16> const& list) : data{list}
        {}
#endif

        Vector<T, 4> row(std::size_t i) const
        {
            ASSERT(i < num_rows);

            Vector<T, 4> vec;
            for (std::size_t j{0}; j < num_rows; ++j)
            {
                vec.data[j] = data[num_rows * i + j];
            }

            return vec;
        }

        Vector<T, 4> col(std::size_t i) const
        {
            ASSERT(i < num_cols);

            Vector<T, 4> vec;
            for (std::size_t j{0}; j < num_cols; ++j)
            {
                vec.data[j] = data[num_cols * i + j];
            }

            return vec;
        }

        T& operator()(std::size_t r, std::size_t c)
        {
            ASSERT(r < num_rows);
            ASSERT(c < num_cols);

            return data[num_rows * r + c];
        }

        T operator()(std::size_t r, std::size_t c) const
        {
            ASSERT(r < num_rows);
            ASSERT(c < num_cols);

            return data[num_rows * r + c];
        }

        std::array<T, 16> data;
    };

    template<typename T, typename UnaryOp>
    Matrix<T> unary_op(Matrix<T>&& mat, UnaryOp&& fun)
    {
        Matrix<T> out{std::move(mat)};
        std::transform(out.data.begin(), out.data.end(), out.data.begin(), fun);
        return out;
    }

    template<typename T, typename UnaryOp>
    Matrix<T> unary_op(Matrix<T> const& mat, UnaryOp&& fun)
    {
        Matrix<T> out{mat};
        std::transform(out.data.begin(), out.data.end(), out.data.begin(), fun);
        return out;
    }

    template<typename T, typename BinaryOp>
    Matrix<T> binary_op(Matrix<T>&& lhs, Matrix<T> const& rhs, BinaryOp&& fun)
    {
        Matrix<T> out{std::move(lhs)};
        std::transform(out.data.begin(),
                       out.data.end(),
                       rhs.data.begin(),
                       out.data.begin(),
                       fun);
        return out;
    }

    template<typename T, typename BinaryOp>
    Matrix<T>
    binary_op(Matrix<T> const& lhs, Matrix<T> const& rhs, BinaryOp&& fun)
    {
        Matrix<T> out{lhs};
        std::transform(lhs.data.begin(),
                       lhs.data.end(),
                       rhs.data.begin(),
                       out.data.begin(),
                       fun);
        return out;
    }

    template<typename T>
    Matrix<T>& operator+=(Matrix<T>& lhs, Matrix<T> const& rhs)
    {
        lhs = binary_op(std::move(lhs), rhs, [](T a, T b) { return a + b; });
        return lhs;
    }

    template<typename T>
    Matrix<T>& operator-=(Matrix<T>& lhs, Matrix<T> const& rhs)
    {
        lhs = binary_op(std::move(lhs), rhs, [](T a, T b) { return a - b; });
        return lhs;
    }

    template<typename T>
    Matrix<T>& operator*=(Matrix<T>& lhs, T rhs)
    {
        lhs = unary_op(std::move(lhs), [rhs](T a) { return a * rhs; });
        return lhs;
    }

    template<typename T>
    Matrix<T>& operator*=(Matrix<T>& lhs, Matrix<T> const& rhs)
    {
        for (std::size_t i{0}; i < N; ++i)
        {
            auto row = lhs.row(i);
            for (std::size_t j{0}; j < N; ++j)
            {
                auto col  = rhs.col(j);
                auto val  = dot(row, col);
                lhs(i, j) = val;
            }
        }

        return lhs;
    }

    template<typename T>
    Matrix<T> operator-(Matrix<T> const& t)
    {
        return unary_op(t, [](T a) { return -a; });
    }

    template<typename T>
    bool operator==(Matrix<T> const& lhs, Matrix<T> const& rhs)
    {
        return lhs.data == rhs.data;

        return true;
    }

    template<typename T>
    bool operator!=(Matrix<T> const& lhs, Matrix<T> const& rhs)
    {
        return !(lhs == rhs);
    }

    template<typename T>
    Matrix<T> operator+(Matrix<T> const& lhs, Matrix<T> const& rhs)
    {
        Matrix<T> out{lhs};
        out += rhs;
        return out;
    }

    template<typename T>
    Matrix<T> operator-(Matrix<T> const& lhs, Matrix<T> const& rhs)
    {
        Matrix<T> out{lhs};
        out -= rhs;
        return out;
    }

    template<typename T>
    Matrix<T> operator*(Matrix<T> const& lhs, T rhs)
    {
        Matrix<T> out{lhs};
        out *= rhs;
        return out;
    }

    template<typename T>
    Matrix<T> operator*(T lhs, Matrix<T> const& rhs)
    {
        Matrix<T> out{rhs};
        out *= lhs;
        return out;
    }

    template<typename T>
    Matrix<T> operator*(Matrix<T> const& lhs, Matrix<T> const& rhs)
    {
        Matrix<T> out{lhs};
        out *= rhs;
        return out;
    }

    template<typename T>
    Vector<T, 4> operator*(Matrix<T> const& lhs, Vector<T, 4> const& rhs)
    {
        Vector<T> out;
        for (std::size_t i{0}; i < N; ++i)
        {
            auto row = lhs.row(i);
            out[i]   = dot(row, rhs);
        }

        return out;
    }

    template<typename T>
    Matrix<T> transpose(Matrix<T> const& mat)
    {
        Matrix<T> out;
        for (std::size_t i{0}; i < N; ++i)
        {
            for (std::size_t j{0}; j < N; ++j)
            {
                out(i, j) = mat(j, i);
            }
        }

        return out;
    }

    template<typename T>
    Matrix<T> inverse(Matrix<T> const& mat)
    {
        constexpr auto N{Matrix<T>::num_rows};

        std::array<std::size_t, N> col_indices, row_indices;
        std::array<std::size_t, N> pivot_indices{0, 0, 0, 0};

        std::array<T, N * N> min_values;
        std::memcpy(min_values.data(), mat.data.data(), N * N * sizeof(T));

        for (std::size_t i{0}; i < N; ++i)
        {
            std::size_t i_row{0}, i_col{0};
            T big{0};

            // Choose pivot.
            for (std::size_t j{0}; j < N; ++j)
            {
                if (pivot_indices[j] != 1)
                {
                    for (std::size_t k{0}; k < N; ++k)
                    {
                        if (pivot_indices[k] == 0)
                        {
                            if (std::abs(min_values[N * j + k]) >= big)
                            {
                                big   = T{std::abs(min_values[N * j + k])};
                                i_row = j;
                                i_col = k;
                            }
                        }
                        else if (pivot_indices[k] > 1)
                        {
                            throw std::runtime_error{
                                "error: cannot find inverse of singular "
                                "matrix\n"};
                        }
                    }
                }
            }

            ++pivot_indices[i_col];

            // Swap rows i_row and i_col for pivot.
            if (i_row != i_col)
            {
                for (std::size_t k{0}; k < N; ++k)
                {
                    std::swap(min_values[N * i_row + k],
                              min_values[N * i_col + k]);
                }
            }

            row_indices[i] = i_row;
            col_indices[i] = i_col;

            if (min_values[N * i_col + i_col] == 0.0f)
            {
                throw std::runtime_error{
                    "error: cannot find inverse of singular "
                    "matrix\n"};
            }

            T inv_pivot = T{1} / min_values[N * i_col + i_col];
            min_values[N * i_col + i_col] = T{1};
            for (std::size_t j{0}; j < N; ++j)
            {
                min_values[N * i_col + j] *= inv_pivot;
            }

            // Subtract this row from others to zero out their columns.
            for (std::size_t j{0}; j < N; ++j)
            {
                if (j != i_col)
                {
                    T save = min_values[N * j + i_col];

                    min_values[N * j + i_col] = 0;
                    for (std::size_t k{0}; k < N; ++k)
                    {
                        min_values[N * j + k] -=
                            min_values[N * i_col + k] * save;
                    }
                }
            }
        }

        // Swap columns to reflect permutation.
        for (int j{N - 1}; j >= 0; --j)
        {
            if (row_indices[j] != col_indices[j])
            {
                for (std::size_t k{0}; k < N; ++k)
                {
                    std::swap(min_values[N * k + row_indices[j]],
                              min_values[N * k + col_indices[j]]);
                }
            }
        }

        return Matrix<T>{min_values};
    }

    template<typename T>
    bool is_identity(Matrix<T> const& mat)
    {
        std::size_t diag_idx{0};
        for (std::size_t i{0}; i < size; ++i)
        {
            if (i == diag_idx)
            {
                if (mat.data[i] != T{1})
                {
                    return false;
                }

                diag_idx += num_rows + 1;
                continue;
            }

            if (mat.data[i] != T{0})
            {
                return false;
            }
        }

        return true;
    }
} // namespace core
