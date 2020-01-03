#include <tgl/init.hpp>

#include <glad/glad.h>
#include <tl/fmt.hpp>
#include <tl/rect.hpp>

namespace tgl
{

static bool g_isInit = false;
extern tl::urect g_currentViewport;
extern bool g_scissorEnabled;
extern tl::urect g_currentScissor;

static const char* errorCodeToStr(GLenum ec)
{
    switch (ec) {
        case GL_NO_ERROR: return "NO_ERROR";
        case GL_INVALID_ENUM: return "INVALID_ENUM";
        case GL_INVALID_VALUE: return "INVALID_VALUE";
        case GL_INVALID_OPERATION: return "INVALID_OPERATION";
        case 0x0503: return "STACK_OVERFLOW";
        case 0x0504: return "STACK_UNDERFLOW"; // gotta use the code because there is a bug in glad
        //case GL_STACK_OVERFLOW: return "STACK_OVERFLOW";
        //case GL_STACK_UNDERFLOW: return "STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY: return  "OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION";
    }
    return "UNKNOWN_ERROR_CODE";
}

static void preCallback(const char* name, void* /*funcptr*/, int /*len_args*/, ...)
{
    GLenum error_code;
    error_code = glad_glGetError();
    if (error_code != GL_NO_ERROR) {
        tl::printError("ERROR ", error_code, " in ", name);
    }
}

static void postCallback(const char* name, void* /*funcptr*/, int /*len_args*/, ...)
{
    GLenum ec;
    ec = glad_glGetError();
    if (ec != GL_NO_ERROR) {
        tl::printError("ERROR ", errorCodeToStr(ec), " in ", name);
    }
}

void init()
{
    if(isInit())
    {
        tl::printError("Trying to initialize tgl when it is already");
        return;
    }

    if(!gladLoadGL())
        tl::printError("gladLoadGl failed");

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLint v[4];
    glGetIntegerv(GL_VIEWPORT, v);
    g_currentViewport = {(unsigned)v[0], (unsigned)v[1], (unsigned)v[2], (unsigned)v[3]};
    glGetIntegerv(GL_SCISSOR_BOX, v);
    g_currentScissor = {(unsigned)v[0], (unsigned)v[1], (unsigned)v[2], (unsigned)v[3]};
    g_isInit = true;

    glad_set_pre_callback(preCallback);
    glad_set_post_callback(postCallback);
}

bool isInit()
{
    return g_isInit;
}

}
