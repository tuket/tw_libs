#pragma once

namespace tl
{
    typedef unsigned long long ULL;
}

constexpr tl::ULL operator"" _K (tl::ULL x) {
    return x * 1024;
}

constexpr tl::ULL operator"" _M (tl::ULL x) {
    return x * 1024 * 1024;
}

constexpr tl::ULL operator"" _G (tl::ULL x) {
    return x * 1024 * 1024 * 1024;
}

constexpr tl::ULL operator"" _T (tl::ULL x) {
    return x * 1024 * 1024 * 1024 * 1024;
}

constexpr long double operator"" _rad (long double x) {
    return x;
}

constexpr long double operator"" _deg (long double x) {
    return 3.14159265359L * x / 180.0L;
}
