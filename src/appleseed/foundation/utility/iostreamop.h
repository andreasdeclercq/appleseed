
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010-2011 Francois Beaune
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef APPLESEED_FOUNDATION_UTILITY_IOSTREAMOP_H
#define APPLESEED_FOUNDATION_UTILITY_IOSTREAMOP_H

// appleseed.foundation headers.
#include "foundation/image/color.h"
#include "foundation/image/spectrum.h"
#include "foundation/math/matrix.h"
#include "foundation/math/ray.h"
#include "foundation/math/vector.h"
#include "foundation/utility/containers/specializedarrays.h"
#include "foundation/utility/string.h"

// Standard headers.
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

namespace foundation
{

//
// I/O of common types from/to C++ streams.
//

// std::vector.
template <typename Type, typename Allocator>
std::ostream& operator<<(std::ostream& s, const std::vector<Type, Allocator>& vector);
template <typename Allocator>
std::ostream& operator<<(std::ostream& s, const std::vector<std::string, Allocator>& vector);
template <typename Allocator>
std::ostream& operator<<(std::ostream& s, const std::vector<char*, Allocator>& vector);
template <typename Allocator>
std::ostream& operator<<(std::ostream& s, const std::vector<const char*, Allocator>& vector);

// foundation::Array.
std::ostream& operator<<(std::ostream& s, const FloatArray& array);
std::istream& operator>>(std::istream& s, FloatArray& array);
std::ostream& operator<<(std::ostream& s, const DoubleArray& array);
std::istream& operator>>(std::istream& s, DoubleArray& array);

// foundation::Vector.
template <typename T, size_t N>
std::ostream& operator<<(std::ostream& s, const Vector<T, N>& vector);
template <typename T, size_t N>
std::istream& operator>>(std::istream& s, Vector<T, N>& vector);

// foundation::Matrix.
template <typename T, size_t M, size_t N>
std::ostream& operator<<(std::ostream& s, const Matrix<T, M, N>& matrix);
template <typename T, size_t M, size_t N>
std::istream& operator>>(std::istream& s, Matrix<T, M, N>& matrix);

// foundation::Color.
template <typename T, size_t N>
std::ostream& operator<<(std::ostream& s, const Color<T, N>& color);
template <typename T, size_t N>
std::istream& operator>>(std::istream& s, Color<T, N>& color);

// foundation::RegularSpectrum.
template <typename T, size_t N>
std::ostream& operator<<(std::ostream& s, const Color<T, N>& spectrum);
template <typename T, size_t N>
std::istream& operator>>(std::istream& s, Color<T, N>& spectrum);

// foundation::Ray.
template <typename T, size_t N>
std::ostream& operator<<(std::ostream& s, const Ray<T, N>& ray);
template <typename T, size_t N>
std::istream& operator>>(std::istream& s, Ray<T, N>& ray);


//
// iostream operators implementation.
//

namespace iostreamop_impl
{
    template <typename Sequence>
    std::ostream& write_sequence(std::ostream& s, const Sequence& sequence, const size_t n)
    {
        if (n > 0)
        {
            for (size_t i = 0; i < n - 1; ++i)
                s << sequence[i] << ' ';
            s << sequence[n - 1];
        }

        return s;
    }

    template <typename Sequence>
    std::ostream& write_sequence_quotes(std::ostream& s, const Sequence& sequence, const size_t n)
    {
        if (n > 0)
        {
            for (size_t i = 0; i < n - 1; ++i)
                s << "\"" << sequence[i] << "\" ";
            s << "\"" << sequence[n - 1] << "\"";
        }

        return s;
    }

    template <typename Sequence>
    std::istream& read_sequence(std::istream& s, Sequence& sequence, const size_t n)
    {
        for (size_t i = 0; i < n; ++i)
            s >> sequence[i];

        return s;
    }
}

template <typename Type, typename Allocator>
std::ostream& operator<<(std::ostream& s, const std::vector<Type, Allocator>& vector)
{
    return iostreamop_impl::write_sequence(s, vector, vector.size());
}

template <typename Allocator>
std::ostream& operator<<(std::ostream& s, const std::vector<std::string, Allocator>& vector)
{
    return iostreamop_impl::write_sequence_quotes(s, vector, vector.size());
}

template <typename Allocator>
std::ostream& operator<<(std::ostream& s, const std::vector<char*, Allocator>& vector)
{
    return iostreamop_impl::write_sequence_quotes(s, vector, vector.size());
}

template <typename Allocator>
std::ostream& operator<<(std::ostream& s, const std::vector<const char*, Allocator>& vector)
{
    return iostreamop_impl::write_sequence_quotes(s, vector, vector.size());
}

namespace iostreamop_impl
{
    template <typename ArrayType>
    std::istream& read_array(std::istream& s, ArrayType& array)
    {
        std::string token;

        while (s >> token)
            array.push_back(from_string<typename ArrayType::value_type>(token));

        // Clear the fail bit, reaching eof is not an error.
        if (s.eof())
            s.clear(s.rdstate() & ~std::ios::failbit);

        return s;
    }
}

inline std::ostream& operator<<(std::ostream& s, const FloatArray& array)
{
    return iostreamop_impl::write_sequence(s, array, array.size());
}

inline std::istream& operator>>(std::istream& s, FloatArray& array)
{
    return iostreamop_impl::read_array(s, array);
}

inline std::ostream& operator<<(std::ostream& s, const DoubleArray& array)
{
    return iostreamop_impl::write_sequence(s, array, array.size());
}

inline std::istream& operator>>(std::istream& s, DoubleArray& array)
{
    return iostreamop_impl::read_array(s, array);
}

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& s, const Vector<T, N>& vector)
{
    return iostreamop_impl::write_sequence(s, vector, N);
}

template <typename T, size_t N>
std::istream& operator>>(std::istream& s, Vector<T, N>& vector)
{
    return iostreamop_impl::read_sequence(s, vector, N);
}

template <typename T, size_t M, size_t N>
std::ostream& operator<<(std::ostream& s, const Matrix<T, M, N>& matrix)
{
    return iostreamop_impl::write_sequence(s, matrix, M * N);
}

template <typename T, size_t M, size_t N>
std::istream& operator>>(std::istream& s, Matrix<T, M, N>& matrix)
{
    return iostreamop_impl::read_sequence(s, matrix, M * N);
}

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& s, const Color<T, N>& color)
{
    return iostreamop_impl::write_sequence(s, color, N);
}

template <typename T, size_t N>
std::istream& operator>>(std::istream& s, Color<T, N>& color)
{
    return iostreamop_impl::read_sequence(s, color, N);
}

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& s, const RegularSpectrum<T, N>& spectrum)
{
    return iostreamop_impl::write_sequence(s, spectrum, N);
}

template <typename T, size_t N>
std::istream& operator>>(std::istream& s, RegularSpectrum<T, N>& spectrum)
{
    return iostreamop_impl::read_sequence(s, spectrum, N);
}

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& s, const Ray<T, N>& ray)
{
    s << ray.m_org  << ' ';
    s << ray.m_dir  << ' ';
    s << ray.m_tmin << ' ';
    s << ray.m_tmax;
    return s;
}

template <typename T, size_t N>
std::istream& operator>>(std::istream& s, Ray<T, N>& ray)
{
    s = iostreamop_impl::read_sequence(s, ray.m_org, N);
    s = iostreamop_impl::read_sequence(s, ray.m_dir, N);
    s >> ray.m_tmin;
    s >> ray.m_tmax;
    return s;
}

}       // namespace foundation

#endif  // !APPLESEED_FOUNDATION_UTILITY_IOSTREAMOP_H
