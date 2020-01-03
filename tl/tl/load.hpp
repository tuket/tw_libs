#pragma once

#include "str.hpp"
#include "containers/vector.hpp"

namespace tl
{

Str loadString(CStr fileName);
tl::Vector<char> loadData(CStr fileName);

}
