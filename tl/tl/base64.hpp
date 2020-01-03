#pragma once

#include <tl/str.hpp>
#include <tl/containers/vector.hpp>

namespace tl
{
namespace base64
{

Str encode(const unsigned char* data, i32 length);
Str encode(const tl::Vector<unsigned char> data);
Str encode(CStr data);


tl::Vector<unsigned char> decode(const char* str, i32 length);
tl::Vector<unsigned char> decode(CStr str);

}
}
