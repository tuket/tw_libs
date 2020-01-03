#pragma once

#include <tl/containers/vector.hpp>
#include <glm/vec4.hpp>
#include <tl/str.hpp>
#include <tl/int_types.hpp>

class Image
{
public:
    Image() : W(0), H(0) {}
    Image(const Image& o);
    Image(Image&& o);

    ~Image();

    const glm::vec4& operator()(int x, int y)const;
    glm::vec4& operator()(int x, int y);

    // load an image from a file
    static Image load(CStr fileName);
    // creates an empty image with all pixels set to 0 (black)
    static Image create(int W, int H);

private:
    i32 W, H;
    tl::Vector<glm::vec4> pixels;
};
