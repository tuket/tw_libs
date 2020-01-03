#include <tgl/shader.hpp>

#include <glad/glad.h>
#include <tl/fmt.hpp>
#include <tl/load.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <tgl/texture.hpp>

using namespace std;
using namespace glm;

namespace tgl
{

static const unsigned LOG_CAPACITY = 4*1024;
static char logBuffer[LOG_CAPACITY];

// --- ShaderObj ---
ShaderObj ShaderObj::create(CStr src, unsigned type)
{
    VertShader shad;
    shad.id = glCreateShader(type);
    const char* srcPtr = src.c_str();
    glShaderSource(shad.id, 1, &srcPtr, nullptr);
    glCompileShader(shad.id);
    GLint ok;
    glGetShaderiv(shad.id, GL_COMPILE_STATUS, &ok);
    if(!ok) {
        GLsizei len;
        glGetShaderInfoLog(shad.id, LOG_CAPACITY, &len, logBuffer);
        tl::printError(logBuffer);
    }

    return shad;
}

void ShaderObj::free()
{
    if(id == 0)
        tl::printError("this shader object is invalid");
    glDeleteShader(id);
    id = 0;
}

ShaderObj::~ShaderObj()
{
    /*if(id != 0)
    {
        glDeleteShader(id);
        id = 0;
    }*/
}

// --- vertShader ---
VertShader VertShader::create(CStr src)
{
    VertShader vs;
    ShaderObj obj = ShaderObj::create(src, GL_VERTEX_SHADER);
    vs.id = obj.getId();
    return vs;
}

VertShader VertShader::load(CStr fileName)
{
    Str src = tl::loadString(fileName);
    return create(src);
}

// --- FragShader ---
FragShader FragShader::create(CStr src)
{
    FragShader fs;
    ShaderObj obj = ShaderObj::create(src, GL_FRAGMENT_SHADER);
    fs.id = obj.getId();
    return fs;
}

FragShader FragShader::load(CStr fileName)
{
    Str src = tl::loadString(fileName);
    return create(src);
}

// --- ShaderProgram ---
unsigned ShaderProgram::s_used = 0;

ShaderProgram ShaderProgram::create(CStr vertSrc, CStr fragSrc)
{
    ShaderProgram prog;
    VertShader vs = VertShader::create(vertSrc);
    FragShader fs = FragShader::create(fragSrc);
    prog._id = glCreateProgram();
    glAttachShader(prog._id, vs.getId());
    glAttachShader(prog._id, fs.getId());
    glLinkProgram(prog._id);
    // delete shaders
    glDetachShader(prog._id, vs.getId());
    glDetachShader(prog._id, fs.getId());
    glDeleteShader(vs.getId());
    glDeleteShader(fs.getId());
    // check for link errors
    GLint ok;
    glGetProgramiv(prog._id, GL_LINK_STATUS, &ok);
    if(!ok) {
        GLsizei len;
        glGetProgramInfoLog(prog._id, LOG_CAPACITY, &len, logBuffer);
        glDeleteProgram(prog._id);
        tl::printError("Error linking:\n", logBuffer);
    }
    return prog;
}

ShaderProgram ShaderProgram::create(const VertShader& vs, const FragShader& fs)
{
    ShaderProgram prog;
    prog._id = glCreateProgram();
    glAttachShader(prog._id, vs.getId());
    glAttachShader(prog._id, fs.getId());
    glLinkProgram(prog._id);
    // check for link errors
    GLint ok;
    glGetProgramiv(prog._id, GL_LINK_STATUS, &ok);
    if(!ok) {
        GLsizei len;
        glGetProgramInfoLog(prog._id, LOG_CAPACITY, &len, logBuffer);
        glDeleteProgram(prog._id);
        tl::printError("Error linking:\n", logBuffer);
    }
    return prog;
}

ShaderProgram ShaderProgram::load(CStr vertFile, CStr fragFile)
{
    Str vertSrc = tl::loadString(vertFile);
    Str fragSrc = tl::loadString(fragFile);
    return ShaderProgram::create(vertSrc, fragSrc);
}

void ShaderProgram::use()const
{
    // note: could try to use glGetIntergerv(GL_CURRENT_PROGRAM)
    if(s_used != _id) {
        glUseProgram(_id);
        s_used = _id;
    }
}

template <>
void ShaderProgram::set(CStr name, const float& x)const
{
    use();
    GLint loc = getLocation(name);
    glUniform1f(loc, x);
}

template <>
void ShaderProgram::set(CStr name, const vec2& x)const
{
    use();
    GLint loc = getLocation(name);
    glUniform2fv(loc, 1, &x[0]);
}

template <>
void ShaderProgram::set(CStr name, const vec3& x)const
{
    use();
    GLint loc = getLocation(name);
    glUniform3fv(loc, 1, &x[0]);
}

template <>
void ShaderProgram::set(CStr name, const vec4& x)const
{
    use();
    GLint loc = getLocation(name);
    glUniform4fv(loc, 1, &x[0]);
}

template <>
void ShaderProgram::set(CStr name, const int& x)const
{
    use();
    GLint loc = getLocation(name);
    // TODO: check float
    glUniform1i(loc, x);
}

bool ShaderProgram::hasUnif(CStr name)const
{
    GLint loc = glGetUniformLocation(_id, name.c_str());
    return loc != -1;
}

void ShaderProgram::free()
{
    glDeleteProgram(_id);
    if(s_used == _id)
    {
        s_used = 0;
    }
    _id = 0;
}

int ShaderProgram::getLocation(CStr name)const
{
    GLint loc = glGetUniformLocation(_id, name.c_str());
    if(loc == -1)
        tl::printError("invalid uniform: ", name);
    return loc;
}

}
