#pragma once

#include <assert.h>

namespace tl
{

template <typename T>
class RingBuffer
{
    const int CAP;
    T* a;
    int i, n;
public:
    RingBuffer(int CAPACITY = 32)
        : CAP(CAPACITY)
    {
        a = new T[CAP];
        i = n = 0;
    }
    int size()const { return n; }
    T peekFront()const { return a[i]; }
    T peekBack()const { return a[(i+CAP-1) % CAP]; }
    T popFront() {
        T e = peekFront();
        i = (i+1) % CAP;
        return e;
    }
    T popBack() {
        T e = peekBack();
        n -= 1;
        assert(n >= 0);
    }
    void pushFront(const T& t) {
        assert(n < CAP);
        i = (i+CAP-1) % CAP;
        a[i] = t;
    }
    void pushBack(const T& t) {
        assert(n < CAP);
        a[(i+n)%CAP] = t;
        n++;
    }
    ~RingBuffer() {
        delete[] a;
        a = nullptr;
        i = n = 0;
    }
};

}
