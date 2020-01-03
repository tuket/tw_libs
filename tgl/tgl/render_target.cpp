#include <tgl/render_target.hpp>

#include <glad/glad.h>
#include <tl/fmt.hpp>

namespace tgl
{

unsigned RenderTarget::current = 0;

RenderTarget::RenderTarget(RenderTarget&& o)
    : id(o.id)
    , W(o.W)
    , H(o.H)
    , textures(std::move(o.textures))
    , depthStencilTexture(std::move(o.depthStencilTexture))
{
    o.id = 0;
    o.W = 0;
    o.H = 0;
}

void RenderTarget::operator=(RenderTarget&& o)
{
    id = o.id;
    W = o.W;
    H = o.H;
    textures = std::move(o.textures);
    depthStencilTexture = std::move(o.depthStencilTexture);
    o.id = 0;
    o.W = 0;
    o.H = 0;
}

void RenderTarget::use(unsigned id)
{
    if(current == id)
        return;
    current = id;
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void RenderTarget::use()
{
    use(id);
}

const Texture& RenderTarget::getTexture(unsigned i)const
{
    if(i >= textures.size())
        tl::printError("RenderTarget does not have texture ", i);
    return textures[i];
}

void RenderTarget::useDefault()
{
    use(0);
}

RenderTarget RenderTarget::create(int W, int H,
    const std::initializer_list<TexelFormat>& formats,
    DepthOrStencil depthOrStencil)
{
    RenderTarget rt;
    rt.W = W;
    rt.H = H;
    glGenFramebuffers(1, &rt.id);
    glBindFramebuffer(GL_FRAMEBUFFER, rt.id);
    current = rt.id;
    const int n = formats.size();
    int extraTexture = depthOrStencil == DepthOrStencil::NONE ? 0 : 1;
    rt.textures.resize(n + extraTexture);
    for(int i=0; i<n; i++)
    {
        rt.textures[i] = std::move(Texture::create(W, H, formats.begin()[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, rt.textures[i].getId(), 0);
    }
    if(extraTexture == 1)
    {
        Texture& tex = rt.textures.back();
        if(depthOrStencil == DepthOrStencil::DEPTH_STENCIL)
        {
            tex = std::move(Texture::create(W, H, TexelFormat::DEPTH_STENCIL));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex.getId(), 0);
        }
        else// if(DepthOrStencil == DepthOrStencil::DEPTH)
        {
            tex = std::move(Texture::create(W, H, TexelFormat::DEPTH));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.getId(), 0);
        }
    }
    return rt;
}

// --- clear ---
void clear()
{
    glClearColor(0, 0, 0, 0);
    glClearDepth(1.f);
    glClearStencil(0u);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void clear(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClearDepth(1.f);
    glClearStencil(0u);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void clear(const glm::vec4& color, float depth)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClearDepth(depth);
    glClearStencil(0u);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void clear(const glm::vec4& color, float depth, unsigned stencil)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClearDepth(depth);
    glClearStencil(stencil);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void clearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void clearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void clearDepth(float depth)
{
    glClearDepth(depth);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void clearStencil(unsigned val)
{
    glClearStencil(val);
    glClear(GL_STENCIL_BUFFER_BIT);
}

// --- viewport ---
tl::urect g_currentViewport = {0, 0, 1, 1};

const tl::urect& getViewport()
{
    return g_currentViewport;
}

void g_viewport(const tl::urect& rectangle)
{
    g_currentViewport = rectangle;
    glViewport(rectangle.x, rectangle.y, rectangle.w, rectangle.h);
}

void viewport(const glm::uvec2& size)
{
    g_currentViewport = {{0, 0}, size};
    glViewport(0, 0, size.x, size.y);
}

void viewport(unsigned x, unsigned y, unsigned w, unsigned h)
{
    g_currentViewport = {x, y, w, h};
    glViewport(x, y, w, h);
}

void viewport(unsigned w, unsigned h)
{
    g_currentViewport = {0, 0, w, h};
    glViewport(0, 0, w, h);
}

// --- scisor ---
tl::urect g_currentScissor = {0, 0, 1, 1};
bool g_scissorEnabled = false;

const tl::urect& getScissor()
{
    return g_currentScissor;
}

void scissor(const tl::urect& rectangle)
{
    g_currentScissor = rectangle;
    glScissor(rectangle.x, rectangle.y, rectangle.w, rectangle.h);
}

void scissor(const glm::uvec2& size)
{
    g_currentScissor = {{0, 0}, size};
    glScissor(0, 0, size.x, size.y);
}

void scissor(unsigned x, unsigned y, unsigned w, unsigned h)
{
    g_currentScissor = {x, y, w, h};
    glScissor(x, y, w, h);
}

void scissor(unsigned w, unsigned h)
{
    g_currentScissor = {0, 0, w, h};
    glScissor(0, 0, w, h);
}

bool isScissorEnabled()
{
    return g_scissorEnabled;
}

void enableScissor(bool enable)
{
    if(enable)
        glEnable(GL_SCISSOR_TEST);
    else
        glDisable(GL_SCISSOR_TEST);
    g_scissorEnabled = enable;
}

}
