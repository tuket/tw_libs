#pragma once

#include <tgl/texture.hpp>
#include <tl/rect.hpp>
#include <glm/vec4.hpp>
#include <tl/containers/vector.hpp>

namespace tgl
{

enum class DepthOrStencil
{
    NONE,
    DEPTH,
    DEPTH_STENCIL
};

class RenderTarget
{
public:
    RenderTarget() : id(0) {}
    RenderTarget(RenderTarget&& o);
    void operator=(RenderTarget&& o);

    bool isValid()const { return id != 0; }
    void use();

    const Texture& getTexture(unsigned i)const;
    const Texture& getDepthStencilTexture()const { return depthStencilTexture; }

    unsigned getWidth()const { return W; }
    unsigned getHeight()const { return H; }
    glm::ivec2 getSize()const { return glm::vec2(W, H); }

    static void useDefault();
    static RenderTarget create(int W, int H,
        const std::initializer_list<TexelFormat>& formats,
        DepthOrStencil depthOrStencil = DepthOrStencil::NONE);
    
private:
    static void use(unsigned id);

    unsigned id;
    unsigned W, H;
    tl::Vector<Texture> textures;
    Texture depthStencilTexture;

    static unsigned current;
};

// --- clear ---
void clear();
void clear(const glm::vec4& color);
void clear(const glm::vec4& color, float depth);
void clear(const glm::vec4& color, float depth, unsigned stencil);
void clearColor(const glm::vec4& color);
void clearColor(float r, float g, float b, float a);
void clearDepth(float depth = 1);
void clearStencil(unsigned val = 0);

// --- viewport ---
const tl::urect& getViewport();
void viewport(const tl::urect& rectangle);
void viewport(const glm::uvec2& size);
void viewport(unsigned x, unsigned y, unsigned w, unsigned h);
void viewport(unsigned w, unsigned h);

// --- scissor ---
const tl::urect& getScissor();
void scissor(const tl::urect& rectangle);
void scissor(const glm::uvec2& size);
void scissor(unsigned x, unsigned y, unsigned w, unsigned h);
void scissor(unsigned w, unsigned h);
bool isScissorEnabled();
void enableScissor(bool enable = true);

}
