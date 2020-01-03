#pragma once

#include "move.hpp"

namespace tl
{

template<typename T>
void swap(T& a, T& b)
{
    T x = move(a);
    a = move(b);
    b = move(x);
}

}