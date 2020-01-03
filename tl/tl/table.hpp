#pragma once

#include <tl/containers/vector.hpp>
#include <tl/int_types.hpp>

namespace tl
{

template <typename T>
class Table
{
public:
    Table() : W(0), H(0) {}
    Table(i32 W, i32 H) : W(0), H(0) { resize(W, H); }

    i32 getW()const { return W; }
    i32 getH()const { return H; }

    const T& operator()(i32 x, i32 y)const { return t[x + W * y]; }
    T& operator()(i32 x, i32 y) { return t[x + W * y]; }

    void resize(i32 W, i32 H)
    {
        i32 prevW = this->W;
        i32 prevH = this->H;
        tl::Vector<T> nt(W * H);
        /*
        i32 y;
        for(y=0; y < std::min(prevH, H); y++)
        {
            i32 x;
            for(x=0; x < std::min(prevW, W); x++)
            {
                nt[x + W*y] = t[x + prevW*y];
            }
            for( ; x < W; x++)
            {
                nt[x + W*y] = 0;
            }
        }
        for( ;y < H; y++)
        {
            for(i32 x=0 ; x < W; x++)
            {
                nt[x + W*y] = 0;
            }
        }*/
        for (i32 y = 0; y < H; y++)
            for (i32 x = 0; x < W; x++)
            {
                if (x < prevW && y < prevH)
                    nt[x + y * W] = t[x + y * prevW];
                else
                    nt[x + y * W] = T();
            }

        this->W = W;
        this->H = H;
        t = tl::move(nt);
    }

    void fill(const T& val)
    {
        for (T& x : t)
            t = val;
    }

    T* data() { return t.data(); }

private:
    i32 W, H;
    tl::Vector<T> t;
};

}
