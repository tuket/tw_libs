#include "tl/base64.hpp"

// https://stackoverflow.com/a/41094722/1754322

namespace tl
{
namespace base64
{

// --- encode ----------------------------------------------------------------------

static const unsigned char base64_table[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

Str encode(const unsigned char* data, i32 len)
{
    unsigned char *out, *pos;
    const unsigned char *end, *in;

    i32 olen;

    olen = 4*((len + 2) / 3); /* 3-byte blocks to 4-byte */

    if (olen < len)
        return Str(); /* integer overflow */

    Str outStr;
    outStr.resize(olen);
    out = (unsigned char*)&outStr[0];

    end = data + len;
    in = data;
    pos = out;
    while (end - in >= 3) {
        *pos++ = base64_table[in[0] >> 2];
        *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = base64_table[in[2] & 0x3f];
        in += 3;
    }

    if (end - in) {
        *pos++ = base64_table[in[0] >> 2];
        if (end - in == 1) {
            *pos++ = base64_table[(in[0] & 0x03) << 4];
            *pos++ = '=';
        }
        else {
            *pos++ = base64_table[((in[0] & 0x03) << 4) |
                (in[1] >> 4)];
            *pos++ = base64_table[(in[1] & 0x0f) << 2];
        }
        *pos++ = '=';
    }

    return outStr;
}

Str encode(const tl::Vector<unsigned char> data)
{
    //assert(data.size() <= numeric_limits<i32>::max());
    return encode(data.data(), (i32)data.size());
}

Str encode(CStr data)
{
    return encode((const unsigned char*)data.c_str(), data.size());
}

// --- decode -------------------------------------------------------------------------------

static const i8 B64index[256] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

tl::Vector<unsigned char> decode(const char* str, i32 len)
{
    auto p = str;
    i32 pad = len > 0 && (len % 4 || p[len - 1] == '=');
    const i32 L = ((len + 3) / 4 - pad) * 4;
    tl::Vector<unsigned char> out((size_t) L / 4 * 3 + pad);

    for (i32 i = 0, j = 0; i < L; i += 4)
    {
        i32 n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
        out[j++] = n >> 16;
        out[j++] = n >> 8 & 0xFF;
        out[j++] = n & 0xFF;
    }
    if (pad)
    {
        i32 n = B64index[p[L]] << 18 | B64index[p[L + 1]] << 12;
        out[out.size() - 1] = n >> 16;

        if (len > L + 2 && p[L + 2] != '=')
        {
            n |= B64index[p[L + 2]] << 6;
            out.push_back(n >> 8 & 0xFF);
        }
    }
    return out;
}

tl::Vector<unsigned char> decode(CStr str)
{
    return decode(str.c_str(), str.size());
}

}
}
