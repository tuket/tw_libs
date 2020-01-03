#include <tgl/texture.hpp>

#include <glad/glad.h>
#include <stbi.h>
#include <tl/fmt.hpp>
#include <tl/swap.hpp>

namespace tgl
{

const FilterMode
FilterModes::NEAREST =
    {MinFilterMode::NEAREST, MagFilterMode::NEAREST};
const FilterMode
FilterModes::LINEAR =
    {MinFilterMode::LINEAR, MagFilterMode::LINEAR};
const FilterMode
FilterModes::MIPMAP_NEAREST =
    {MinFilterMode::LINEAR_MIP_NEAREST_TEXEL, MagFilterMode::NEAREST};
const FilterMode
FilterModes::MIPMAP_BILINEAR =
    {MinFilterMode::NEAREST_MIP_LINEAR_TEXEL, MagFilterMode::LINEAR};
const FilterMode
FilterModes::MIPMAP_TRILINEAR =
    {MinFilterMode::LINEAR_MIP_LINEAR_TEXEL, MagFilterMode::LINEAR};

static GLuint toGlMinFilter(MinFilterMode mode)
{
    static const GLuint lookUp[(int)MinFilterMode::COUNT] = {
        GL_NEAREST,
        GL_LINEAR,
        GL_NEAREST_MIPMAP_NEAREST,
        GL_LINEAR_MIPMAP_NEAREST,
        GL_NEAREST_MIPMAP_LINEAR,
        GL_LINEAR_MIPMAP_LINEAR
    };
    int i = (int)mode;
    return lookUp[i];
}

static GLuint toGlMagFilter(MagFilterMode mode)
{
    static const GLuint lookUp[(int)MagFilterMode::COUNT] = {
        GL_NEAREST,
        GL_LINEAR
    };
    int i = (int)mode;
    return lookUp[i];
}

static GLuint toGlWrap(WrapMode wrap)
{
    GLuint lookUp[(int)WrapMode::COUNT] = {
        GL_CLAMP_TO_EDGE,
        GL_REPEAT,
        GL_CLAMP_TO_BORDER,
        GL_MIRRORED_REPEAT
    };
    int i = (int) wrap;
    return lookUp[i];
}

static GLuint toGlTexelFormat(TexelFormat format)
{
    static const GLuint lookUp[(int)TexelFormat::COUNT] = {
        GL_DEPTH,
        GL_DEPTH_STENCIL,
        GL_RED,
        GL_RGB,
        GL_RGBA
    };
    int i = (int)format;
    return lookUp[i];
}

static GLuint toGlPixelFormat(PixelFormat format)
{
    static const GLuint lookUp[(int)PixelFormat::COUNT] = {
        GL_RED,
        GL_RG,
        GL_RGB,
        GL_RGBA
    };
    int i = (int)format;
    return lookUp[i];
}

static GLuint toGlPixelType(PixelFormat format)
{
    static const GLuint lookUp[(int)PixelFormat::COUNT] = {
        GL_UNSIGNED_BYTE,
        GL_UNSIGNED_BYTE,
        GL_UNSIGNED_BYTE,
        GL_UNSIGNED_BYTE
    };
    int i = (int)format;
    return lookUp[i];
}

static size_t getPixelFormatSize(PixelFormat format)
{
    static const size_t lookUp[(int)PixelFormat::COUNT] = {

    };
    int i = (int)format;
    return lookUp[i];
}

Texture::Texture()
    : id(0)
    , W(0)
    , H(0)
{}

Texture::Texture(Texture&& o)
    : id(o.id)
    , W(o.W)
    , H(o.H)
{
    o.id = 0;
    o.W = 0;
    o.H = 0;
}

Texture::~Texture()
{

}

void Texture::operator=(Texture&& o)
{
    id = o.id;
    W = o.W;
    H = o.H;
    o.id = 0;
    o.W = 0;
    o.H = 0;
}

void Texture::upload(unsigned w, unsigned h,
    TexelFormat texelFormat, void* data, PixelFormat pixelFormat)
{
    bindTemp();
    glTexImage2D(GL_TEXTURE_2D,
        0,  // mip level
        toGlTexelFormat(texelFormat), // GPU format
        w, h,
        0, // border
        toGlPixelFormat(pixelFormat), // CPU format
        toGlPixelType(pixelFormat), // CPU type
        data
    );
    W = w;
    H = h;
}

void Texture::uploadRectangle(unsigned x, unsigned y, unsigned w, unsigned h,
    void* data, PixelFormat pixelFormat)
{
    bindTemp();
    glTexSubImage2D(GL_TEXTURE_2D,
        0,  // mip level
        x, y, w, h,
        toGlPixelFormat(pixelFormat),
        toGlPixelType(pixelFormat), data);
}

void Texture::uploadRectangle(tl::urect& r, void* data, PixelFormat pixelFormat)
{
    uploadRectangle(r.x, r.y, r.w, r.h, data, pixelFormat);
}

void Texture::free()
{
    glDeleteTextures(1, &id);
}

tl::Table<uint32_t> Texture::getPixels(unsigned x, unsigned y, unsigned w, unsigned h)const
{
    tl::Table<uint32_t> pixels(w, h);
    glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_INT, &pixels(0, 0));
    return pixels;
}

tl::Table<uint32_t> Texture::getPixels(tl::urect& r)const
{
    return getPixels(r.x, r.y, r. w, r.h);
}

uint32_t Texture::getPixel(unsigned x, unsigned y)const
{
    unsigned char p[4];
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, p);
    uint32_t id = 0;
    //id |= p[3];
    id <<= 8;
    id |= p[2];
    id <<= 8;
    id |= p[1];
    id <<= 8;
    id |= p[0];
    return id;
}

Texture Texture::create(unsigned W, unsigned H,
    TexelFormat texelFormat, const unsigned char* data, PixelFormat pixelFormat)
{
    tl::Vector<unsigned char> alignedData;
    size_t missAlignment = (size_t)data & 0x3;
    if(missAlignment)
    {
        // not 4-aligned
        missAlignment = (size_t)alignedData.data() & 0x3;
        const size_t shift = (4 - missAlignment) % 4;
        const unsigned size = W*H * getPixelFormatSize(pixelFormat);
        alignedData.resize(size + shift);
        for(unsigned i=0; i<size; i++)
        {
            alignedData[shift + i] = data[i];
        }
        data = &alignedData[shift];
    }
    Texture tex;
    glGenTextures(1, &tex.id);
    tex.bindTemp();
    glTexImage2D(GL_TEXTURE_2D,
        0,
        toGlTexelFormat(texelFormat), // GPU format
        W, H,
        0, // border
        toGlPixelFormat(pixelFormat), // CPU format
        toGlPixelType(pixelFormat), // CPU type
        data
    );
    tex.W = W;
    tex.H = H;
    return tex;
}

Texture Texture::load(CStr fileName)
{
    Texture tex;
    int W, H, C;
    unsigned char* data = stbi_load(fileName.c_str(), &W, &H, &C, 4);
    struct Pixel {char p[4];};
    Pixel* pixels = (Pixel*)data;
    for(int y1=0, y2=H-1; y1<y2; y1++, y2--)
    for(int x=0; x<W; x++)
    {
        tl::swap(pixels[x + W * y1], pixels[x + W * y2]);
    }
    if(!data)
        tl::printError("error loading texture '", fileName, "'");
    glGenTextures(1, &tex.id);
    tex.bindTemp();
    glTexImage2D(GL_TEXTURE_2D,
        0, // mip level
        GL_RGBA, // GPU format
        W, H,
        0, // border
        GL_RGBA, // CPU format
        GL_UNSIGNED_BYTE, // CPU type 
        data
    );
    stbi_image_free(data);
    tex.W = W;
    tex.H = H;
    return tex;
}

// PRIVATE

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::bindTemp()
{
    static unsigned lastSlot = GL_TEXTURE0 + TextureSlots::count() - 1;
    glActiveTexture(lastSlot);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::setWrapMode(int axis, WrapMode mode)
{
    bindTemp();
    GLuint lookUpMode[(int)WrapMode::COUNT] = {
        GL_CLAMP_TO_EDGE,
        GL_REPEAT,
        GL_CLAMP_TO_BORDER,
        GL_MIRRORED_REPEAT
    };
    GLuint lookUpAxis[3] = {
        GL_TEXTURE_WRAP_S,
        GL_TEXTURE_WRAP_T,
        GL_TEXTURE_WRAP_R
    };
    GLuint modeGl = lookUpMode[(int)mode];
    GLuint axisGl = lookUpAxis[axis];
    glTexParameteri(GL_TEXTURE_2D, axisGl, modeGl);
}

// --- BasicTextures ---

const Texture& BasicTextures::black()
{
    static const Texture tex = [] {
        unsigned char data[4] = {0, 0, 0, 255};
        return Texture::create(1, 1, TexelFormat::RGBA, data);
    }();
    return tex;
};

const Texture& BasicTextures::white()
{
    static const Texture tex = [] {
        unsigned char data[4] = {255, 255, 255, 255};
        return Texture::create(1, 1, TexelFormat::RGBA, data);
    }();
    return tex;
};

const Texture& BasicTextures::semiTransparentMagenta()
{
    static const Texture tex = [] {
        unsigned char data[4] = {255, 0, 255, 130};
        return Texture::create(1, 1, TexelFormat::RGBA, data);
    }();
    return tex;
}

// --- Sampler ---

Sampler::Sampler()
    : id(0)
    , dirty(true)
    , boundSlot(-1)
    , wrap{WrapMode::CLAMP, WrapMode::CLAMP}
    , minFilter(MinFilterMode::NEAREST)
    , magFilter(MagFilterMode::NEAREST)
{}

Sampler::Sampler(WrapMode wrap, const FilterMode& filter)
    : id(0)
    , dirty(true)
    , boundSlot(-1)
    , wrap{wrap, wrap}
    , minFilter(filter.min)
    , magFilter(filter.mag)
{}

Sampler::Sampler(const Sampler& o)
    : id(0)
    , dirty(true)
    , boundSlot(-1)
    , wrap(o.wrap)
    , minFilter(o.minFilter)
    , magFilter(o.magFilter)
{}

Sampler::Sampler(Sampler&& o)
    : id(o.id)
    , dirty(o.dirty)
    , boundSlot(o.boundSlot)
    , wrap(o.wrap)
    , minFilter(o.minFilter)
    , magFilter(o.magFilter)
{
    o.id = 0;
}

void Sampler::operator=(Sampler&& o)
{
    id = o.id;
    dirty = o.dirty;
    boundSlot = o.boundSlot;
    wrap = o.wrap;
    minFilter = o.minFilter;
    magFilter = o.magFilter;
    o.id = 0;
}

Sampler::~Sampler()
{
    if(boundSlot >= 0)
        TextureSlots::clearSampler(boundSlot);
}

void Sampler::setWrapX(WrapMode wrapX)
{
    dirty = true;
    wrap[0] = wrapX;
}

void Sampler::setWrapY(WrapMode wrapY)
{
    dirty = true;
    wrap[1] = wrapY;
}

void Sampler::setWrapXY(WrapMode wrapXY)
{
    dirty = true;
    wrap[0] = wrapXY;
    wrap[1] = wrapXY;
}

void Sampler::setMinFilter(MinFilterMode minFilter)
{
    dirty = true;
    this->minFilter= minFilter;
}

void Sampler::setMagFilter(MagFilterMode magFilter)
{
    dirty = true;
    this->magFilter = magFilter;
}

void Sampler::setFilter(FilterMode filter)
{
    dirty = true;
    minFilter = filter.min;
    magFilter = filter.mag;
}

void Sampler::setBorderColor(const glm::vec4& color)
{
    dirty = true;
    borderColor = color;
}

// --- TextureSlots ---

void TextureSlots::set(unsigned i, const Texture& texture, const Sampler& sampler)
{
    if(i >= count()-1) { // -1 because the last one is reserved as temp scratch
        tl::printError("TextureSlots::set: slot index '", i, "' is too big");
    }
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, texture.getId());
    bindSampler(i, sampler);
}

unsigned TextureSlots::count()
{
    static unsigned N =
    []() -> unsigned
    {
        int N;
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &N);
        return N;
    }();
    return N;
}

void TextureSlots::bindSampler(unsigned slot, const Sampler& sampler)
{
    if(sampler.id == 0)
    {
        glGenSamplers(1, &sampler.id);
        sampler.dirty = true;
    }
    if(sampler.dirty)
    {
        glSamplerParameteri(sampler.id, GL_TEXTURE_WRAP_S, toGlWrap(sampler.wrap[0]));
        glSamplerParameteri(sampler.id, GL_TEXTURE_WRAP_T, toGlWrap(sampler.wrap[1]));
        glSamplerParameteri(sampler.id, GL_TEXTURE_MIN_FILTER, toGlMinFilter(sampler.minFilter));
        glSamplerParameteri(sampler.id, GL_TEXTURE_MAG_FILTER, toGlMagFilter(sampler.magFilter));
        glSamplerParameterfv(sampler.id, GL_TEXTURE_BORDER_COLOR, &sampler.borderColor[0]);
    }
    glBindSampler(slot, sampler.id);
}

void TextureSlots::clearSampler(unsigned slot)
{
    bindSampler(slot, defaultSampler);
}

Sampler TextureSlots::defaultSampler(WrapMode::CLAMP, FilterModes::NEAREST);

}
