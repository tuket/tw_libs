#pragma once

#include <tl/containers/vector.hpp>
#include <tl/containers/array.hpp>
#include <tl/str.hpp>
#include <tl/table.hpp>
#include <tl/rect.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "image.hpp"

namespace tgl
{

enum class WrapMode {
    CLAMP,
    REPEAT,
    USE_BORDER_COLOR,
    MIRRORED_REAPEAT,
    COUNT
};

enum class MinFilterMode {
    NEAREST,
    LINEAR,
    NEAREST_MIP_NEAREST_TEXEL,
    NEAREST_MIP_LINEAR_TEXEL,
    LINEAR_MIP_NEAREST_TEXEL,
    LINEAR_MIP_LINEAR_TEXEL,
    COUNT
};

enum class MagFilterMode {
    NEAREST,
    LINEAR,
    COUNT
};

struct FilterMode {
    MinFilterMode min;
    MagFilterMode mag;
};

struct FilterModes {
    static const FilterMode NEAREST;
    static const FilterMode LINEAR;

    static const FilterMode MIPMAP_NEAREST;
    static const FilterMode MIPMAP_BILINEAR;
    static const FilterMode MIPMAP_TRILINEAR;
};

enum class PixelFormat
{
    R,
    RG,
    RGB,
    RGBA,

    COUNT
};

enum class TexelFormat
{
    DEPTH,
    DEPTH_STENCIL,

    R8,

    RGB,
    RGBA,

    COUNT
};

class Texture
{
public:
    Texture();
    Texture(Texture&& o);
    ~Texture();
    void operator=(Texture&& o);

    unsigned getId()const{ return id; }
    bool isValid()const{ return id != 0; }
    operator bool()const{ return id != 0; }

    unsigned getW()const{ return W; }
    unsigned getH()const{ return H; }

    // specify the color when using WrapMode::USE_BORDER_COLOR
    void setBorderColor(const glm::vec4& color);

    // uploads the image data
    void upload(unsigned w, unsigned h,
        TexelFormat texelFormat = TexelFormat::RGBA,
        void* data = nullptr, PixelFormat pixelFormat = PixelFormat::RGBA);

    // uploads data to a sub-rectangle in the texture
    void uploadRectangle(unsigned x, unsigned y, unsigned w, unsigned h,
        void* data = nullptr, PixelFormat pixelFormat = PixelFormat::RGBA);
    void uploadRectangle(tl::urect& rect,
        void* data = nullptr, PixelFormat pixelFormat = PixelFormat::RGBA);

    void free();

    tl::Table<uint32_t> getPixels(unsigned x, unsigned y, unsigned w, unsigned h)const;
    tl::Table<uint32_t> getPixels(tl::urect& rect)const;
    uint32_t getPixel(unsigned x, unsigned y)const;

    static Texture create(unsigned W, unsigned H,
        TexelFormat texelFormat = TexelFormat::RGBA,
        const unsigned char* data = nullptr, PixelFormat pixelFormat = PixelFormat::RGBA);
    static Texture load(CStr fileName);

private:
    void bind();
    void bindTemp();
    void setWrapMode(int axis, WrapMode mode);

    unsigned id;
    unsigned W, H;
};

struct BasicTextures
{
    static const Texture& black();
    static const Texture& white();
    static const Texture& semiTransparentMagenta();
};

class Sampler
{
public:
    Sampler();
    Sampler(WrapMode wrap, const FilterMode& filter);
    Sampler(const Sampler& o);
    Sampler(Sampler&& o);
    void operator=(Sampler&& o);
    ~Sampler();

    WrapMode getWrap(unsigned axis)const { return wrap[axis]; }
    WrapMode getWrapX()const { return wrap[0]; }
    WrapMode getWrapY()const { return wrap[1]; }
    WrapMode getWrapZ()const { return wrap[2]; }
    MinFilterMode getMinFilter()const { return minFilter; }
    MagFilterMode getMagFilter()const { return magFilter; }
    glm::vec4 getBorderColor()const { return borderColor; }

    void setWrapX(WrapMode wrapX);
    void setWrapY(WrapMode wrapY);
    void setWrapXY(WrapMode wrapXY);
    void setMinFilter(MinFilterMode minFilter);
    void setMagFilter(MagFilterMode magFilter);
    void setFilter(FilterMode filter);
    void setBorderColor(const glm::vec4& color);

private:
    friend class TextureSlots;
    void createIfNotAlready();

    mutable bool dirty; // maybe have one dirty per param?
    mutable unsigned id;
    mutable int boundSlot; // negative means not bound to any slot, when destroyed we need to crear the bound slot
    tl::Array<WrapMode, 2> wrap;   // XY
    MinFilterMode minFilter;
    MagFilterMode magFilter;
    glm::vec4 borderColor;
};

class TextureSlots
{
public:
    static void set(unsigned i, const Texture& texture, const Sampler& sampler);
    static unsigned count();
private:
    friend Sampler::~Sampler();
    static void bindSampler(unsigned slot, const Sampler& sampler);
    static void clearSampler(unsigned slot);

    static Sampler defaultSampler;
};

}
