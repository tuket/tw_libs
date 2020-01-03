#pragma once

#include <glm/vec2.hpp>

namespace tl
{

template <typename T, typename TV, typename TB>
struct RectCommonStuff
{
    TV corner;
    TV size;
    TB& x = corner.x;
    TB& y = corner.y;
    TB& w = size.x;
    TB& h = size.y;
    
    RectCommonStuff() {}
    RectCommonStuff(const RectCommonStuff& o)
        : corner(o.x, o.y), size(o.w, o.h) {}
    RectCommonStuff(TB w, TB h)
        : corner(0, 0), size(w, h) {}
    RectCommonStuff(TB x, TB y, TB w, TB h)
        : corner(x, y), size(w, h) {}
    RectCommonStuff(const TV& size)
        : corner(0, 0), size(size) {}
    RectCommonStuff(const TV& corner, const TV& size)
        : corner(corner), size(size) {}

    const T& operator=(const T& o)
    {
        corner = o.corner;
        size = o.size;
        return *static_cast<T*>(this);
    }

    bool operator==(const T& o)const
    {
        return corner == o.corner && size == o.size;
    }

    bool operator!=(const T& o)const
    {
        return corner != o.corner || size != o.size;
    }

    T operator*(const TV& v)const
    {
        T r(v * corner, v * size);
        return r;
    }

    void operator*=(const TV& v)
    {
        corner *= v;
        size *= v;
    }
};

struct rect : RectCommonStuff<rect, glm::vec2, float>
{
    rect() {}
    rect(const rect& o) : RectCommonStuff(o) {}
    rect(float w, float h) : RectCommonStuff(w, h) {}
    rect(float x, float y, float w, float h) : RectCommonStuff(x, y, w, h) {}
    rect(const glm::vec2& size) : RectCommonStuff(size) {}
    rect(const glm::vec2& corner, const glm::vec2& size) : RectCommonStuff(corner, size) {}
    const rect& operator=(const rect& o) { return RectCommonStuff::operator=(o); }
};

struct irect : RectCommonStuff<irect, glm::ivec2, int>
{
    irect() {}
    irect(const irect& o) : RectCommonStuff(o) {}
    irect(int w, int h) : RectCommonStuff(w, h) {}
    irect(int x, int y, int w, int h) : RectCommonStuff(x, y, w, h) {}
    irect(const glm::ivec2& size) : RectCommonStuff(size) {}
    irect(const glm::ivec2& corner, const glm::ivec2& size) : RectCommonStuff(corner, size) {}
    const irect& operator=(const irect& o) { return RectCommonStuff::operator=(o); }
};

struct urect : RectCommonStuff<urect, glm::uvec2, unsigned>
{
    urect() {}
    urect(const urect& o) : RectCommonStuff(o) {}
    urect(unsigned w, unsigned h) : RectCommonStuff(w, h) {}
    urect(unsigned x, unsigned y, unsigned w, unsigned h) : RectCommonStuff(x, y, w, h) {}
    urect(const glm::uvec2& size) : RectCommonStuff(size) {}
    urect(const glm::uvec2& corner, const glm::uvec2& size) : RectCommonStuff(corner, size) {}
    const urect& operator=(const urect& o) { return RectCommonStuff::operator=(o); }
};

static rect toFloat(const irect& r)
{
    return rect{(float)r.x, (float)r.y, (float)r.w, (float)r.h};
}

}
