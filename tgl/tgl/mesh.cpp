#include <tgl/mesh.hpp>

#include <glad/glad.h>

namespace tgl
{

// --- Vbo ---
#ifdef MEMORY_DEBUG_MODE
size_t Vbo::s_numCreated = 0;
size_t Vbo::s_numFreed = 0;
#endif

Vbo Vbo::create()
{
    Vbo vbo;
    createId(vbo);
    return vbo;
}

void Vbo::setInvalidWithoutFree()
{
    id = 0;
}

void Vbo::uploadData(const char* data, unsigned size)
{
    assert(id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void Vbo::reUploadData(const char* data, unsigned size)
{
    unsigned offset = 0;
    assert(id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void Vbo::free()
{
    glDeleteBuffers(1, &id);
#ifdef MEMORY_DEBUG_MODE
    if(id != 0)
        s_numFreed++;
#endif
}

void Vbo::createId(Vbo& vbo)
{
    glGenBuffers(1, &vbo.id);
#ifdef MEMORY_DEBUG_MODE
    s_numCreated++;
#endif
}

size_t Vbo::getNumCreated()
{
#ifdef MEMORY_DEBUG_MODE
    return s_numCreated;
#else
    return 0;
#endif
}
size_t Vbo::getNumFreed()
{
#ifdef MEMORY_DEBUG_MODE
    return s_numFreed;
#else
    return 0;
#endif
}

// --- Vao ---
unsigned Vao::bound = 0;
#ifdef MEMORY_DEBUG_MODE
size_t Vao::s_numCreated = 0;
size_t Vao::s_numFreed = 0;
#endif

Vao Vao::create()
{
    Vao vao;
    glGenVertexArrays(1, &vao.id);
#ifdef MEMORY_DEBUG_MODE
    s_numCreated++;
#endif
    return vao;
}

void Vao::setInvalidWithoutFree()
{
    id = 0;
}

void Vao::bind()
{
    if(id == bound)
        bound = 0;
    glBindVertexArray(id);
    bound = id;
}

void Vao::link(unsigned index,
    unsigned vbo, unsigned numVals, unsigned stride, unsigned offset)
{
    bind();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, numVals, GL_FLOAT, false, stride, (void*)(uintptr_t)offset);
}

void Vao::free()
{
    if(bound == id)
        bound = 0;
    glDeleteVertexArrays(1, &id);
#ifdef MEMORY_DEBUG_MODE
    if(id != 0)
        s_numFreed++;
#endif
}

size_t Vao::getNumCreated()
{
#ifdef MEMORY_DEBUG_MODE
    return s_numCreated;
#else
    return 0;
#endif
}
size_t Vao::getNumFreed()
{
#ifdef MEMORY_DEBUG_MODE
    return s_numFreed;
#else
    return 0;
#endif
}


// -- draw ---
static GLenum toGlPrimitive(Primitive primitive)
{
    static const GLenum lookUp[(int)Primitive::COUNT] =
    {
        GL_POINTS,
        GL_LINES,
        GL_LINE_STRIP,
        GL_LINE_LOOP,
        GL_TRIANGLES,
        GL_TRIANGLE_STRIP,
        GL_TRIANGLE_FAN
    };
    int i = (int)primitive;
    return lookUp[i];
}


void draw(Vao vao, Primitive primitive, unsigned firstVert, unsigned numVerts)
{
    GLenum prim = toGlPrimitive(primitive);
    glBindVertexArray(vao.getId());
    glDrawArrays(prim, firstVert, numVerts);
}

void draw(Vao vao, Primitive primitive, unsigned numVerts)
{
    draw(vao, primitive, 0, numVerts);
}

float line::getThickness()
{
    float thickness;
    glGetFloatv(GL_LINE_WIDTH, &thickness);
    return thickness;
}

void line::setThickness(float thickness)
{
    glLineWidth(thickness);
}

bool line::isSmoothEnabled()
{
    GLboolean enabled;
    glGetBooleanv(GL_LINE_SMOOTH, &enabled);
    return enabled;
}

void line::enableSmooth(bool enable)
{
    if(enable)
        glEnable(GL_LINE_SMOOTH);
    else {
        glDisable(GL_LINE_SMOOTH);
    }
}

}
