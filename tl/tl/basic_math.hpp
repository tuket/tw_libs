#pragma once

#include "int_types.hpp"

namespace tl
{

template <typename T>
static constexpr T min(const T& a, const T& b) noexcept
{
    // return a if equal
    return (a > b) ? b : a;
}

template <typename T>
static constexpr T max(const T& a, const T& b) noexcept
{
    // return a if equal
    return (b > a) ? b : a;
}

template <typename T>
static constexpr T clamp(const T& x, const T& min, const T& max) noexcept
{
    return (x > min) ? (x < max ? x : max) : min;
}

// computes the next power of two, unless it's already a power of two
static constexpr u32 nextPowerOf2(u32 x) noexcept
{
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

// computes the next power of two, unless it's already a power of two
static constexpr u64 nextPowerOf2(u64 x) noexcept
{
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    x++;
    return x;
}

}
