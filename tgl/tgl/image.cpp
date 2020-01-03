#include <tgl/image.hpp>

#include <tl/fmt.hpp>
#include <stbi.h>

Image::Image(const Image& o)
    : W(o.W)
    , H(o.H)
    , pixels(o.pixels)
{}

Image::Image(Image&& o)
    : W(o.W)
    , H(o.H)
    , pixels(std::move(o.pixels))
{}

Image::~Image()
{}

const glm::vec4& Image::operator()(i32 x, i32 y)const
{
    if (x >= W || y >= H)
        tl::printError("Image pixel out of bounds");
    return pixels[x + W * y];
}
glm::vec4& Image::operator()(i32 x, i32 y)
{
    if (x >= W || y >= H)
        tl::printError("Image pixel out of bounds");
    return pixels[x + W * y];
}

// load an image from a file
Image Image::load(CStr fileName)
{
    Image img;
    i32 C;
    i32& W = img.W;
    i32& H = img.H;
    unsigned char* data = stbi_load(fileName.c_str(),
        &W, &H, &C, 4);
    if (!data)
        tl::printError("couldn't load file ", fileName);
    img.pixels.resize(W * H);
    i32 i = 0;
    for (i32 y = 0; y < H; y++)
        for (i32 x = 0; x < W; x++)
            for (i32 c = 0; c < 4; c++)
            {
                img(x, y)[0] = data[i];
                i++;
            }
    return img;
}

// creates an empty image with all pixels set to 0 (black)
Image Image::create(i32 W, i32 H)
{
    Image img;
    img.W = W;
    img.H = H;
    img.pixels = tl::Vector<glm::vec4>(W * H, glm::vec4(0));
    return img;
}
