#pragma once

#include <initializer_list>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <tl/fmt.hpp>
#include <tl/containers/array.hpp>
#include <tl/containers/vector.hpp>

namespace tgl
{

// --- all_and_at_least_one ---
template <bool...>
struct all_and_at_least_one : std::false_type {};
template <bool... b>
struct all_and_at_least_one<false, b...> : std::false_type {};
template <bool... b>
struct all_and_at_least_one<true, b...> : all_and_at_least_one<b...> {};
template <>
struct all_and_at_least_one<true> : std::true_type {};

// --- supported vertex types ---
template <typename T>
constexpr bool isSupportedVertexType()
{
    return
        std::is_same<T, float>() ||
        std::is_same<T, glm::vec2>() ||
        std::is_same<T, glm::vec3>() ||
        std::is_same<T, glm::vec4>();
};

template <typename... Ts>
constexpr bool areSupportedVertexTypes() {
    return all_and_at_least_one<isSupportedVertexType<Ts>()...>();
}

// --- numElems ---
template <typename T> static unsigned numElems();
template <> unsigned numElems<float>() { return 1; };
template <> unsigned numElems<glm::vec2>() { return 2; };
template <> unsigned numElems<glm::vec3>() { return 3; };
template <> unsigned numElems<glm::vec4>() { return 4; };

// --- getVertexElemOffset ---
template <unsigned i, unsigned coffset, typename... Ts>
struct VertexElemOffsetStruct;

template <unsigned i, unsigned coffset, typename T, typename... Ts>
struct VertexElemOffsetStruct<i, coffset, T, Ts...>
{
    static const unsigned offset = VertexElemOffsetStruct<i-1, coffset + sizeof(T), Ts...>::offset;
};

template <unsigned coffset, typename T, typename... Ts>
struct VertexElemOffsetStruct<0, coffset, T, Ts...>
{
    static const unsigned offset = coffset;
};

template <unsigned i, typename... Ts>
constexpr unsigned getVertexElemOffset()
{
    static_assert(i < sizeof...(Ts), "invalid index");
    static_assert(areSupportedVertexTypes<Ts...>(), "not supported type");
    return VertexElemOffsetStruct<i, 0, Ts...>::offset;
}

// --- totalSize ---
template <typename... Ts>
struct total_size_struct { static const unsigned size = 0; };
template <typename T, typename... Ts>
struct total_size_struct<T, Ts...> { static const unsigned size = sizeof(T) + total_size_struct<Ts...>::size; };
template <typename... Ts>
static unsigned total_size() { return total_size_struct<Ts...>::size; }

// --- type_at_index ---
template <unsigned i, typename T, typename... Ts>
struct type_at_index_struct{ using type = typename type_at_index_struct<i-1, Ts...>::type; };

template <typename T, typename... Ts>
struct type_at_index_struct<0, T, Ts...>{ using type = T; };

template <unsigned i, typename... Ts>
using type_at_index = typename type_at_index_struct<i, Ts...>::type;

// --- Primitive ---
enum class Primitive
{
    POINTS,
    LINES,
    LINE_STRIP,
    LINE_LOOP,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,

    COUNT
};

// --- Vbo ---
class Vbo
{
public:
    static Vbo create();

    Vbo() : id(0) {}
    Vbo(const Vbo& o) : id(o.id) {}
    unsigned getId()const{ return id; }
    bool isValid()const{ return id != 0; }
    void setInvalidWithoutFree();
    void uploadData(const char* data, unsigned size);
    void reUploadData(const char* data, unsigned size);
    void free();

    static size_t getNumCreated();
    static size_t getNumFreed();
protected:
    unsigned id;

    static void createId(Vbo& vbo);
#ifdef MEMORY_DEBUG_MODE
    static size_t s_numCreated;
    static size_t s_numFreed;
#endif
};

template <typename... Ts>
class VboT;

template <unsigned i, typename... Ts>
struct VboTAttribRef {
    VboT<Ts...>& vbo;
};

template<typename... Ts>
class VboT : public Vbo
{
    static_assert(areSupportedVertexTypes<Ts...>(), "not supported type");
public:
    static VboT<Ts...> create() {
        VboT<Ts...> vbo;
        createId(vbo);
        return vbo;
    }

    VboT() : Vbo() {}
    VboT(const VboT& o) : Vbo(o) {}

    template <typename T>
    void upload(const tl::Vector<T>& v)
    {
        const unsigned n = sizeof(T) * v.size();
        if(sizeof(T) != total_size<Ts...>())
            tl::printError("the vector element type doesn't match the vertex size");
        uploadData((const char*)v.data(), n);
    }

    template <typename T, size_t N>
    void upload(const tl::Array<T, N>& v)
    {
        const unsigned n = sizeof(T) * N;
        if(sizeof(T) != total_size<Ts...>())
            tl::printError("the vector element type doesn't match the vertex size");
        uploadData((const char*)v.data(), n);
    }

    template <typename T>
    void reUpload(const tl::Vector<T>& v)
    {
        const unsigned n = sizeof(T) * v.size();
        if(sizeof(T) != total_size<Ts...>())
            tl::printError("the vector element type doesn't match the vertex size");
        reUploadData((const char*)v.data(), n);
    }

    template <typename T, size_t N>
    void reUpload(const tl::Array<T, N>& v)
    {
        const unsigned n = sizeof(T) * v.size();
        if(sizeof(T) != total_size<Ts...>())
            tl::printError("the vector element type doesn't match the vertex size");
        reUploadData((const char*)v.data(), n);
    }

    template <unsigned i>
    using AttribRef = VboTAttribRef<i, Ts...>;

    template <unsigned i>
    AttribRef<i> attribRef()
    {
        return AttribRef<i> {*this};
    }
};

class Ebo
{

}; 

// --- Vao ---
class Vao
{
public:
    Vao() : id(0) {}
    Vao(const Vao& o) : id(o.id) {}

    static Vao create();
    unsigned getId()const { return id; }
    void setInvalidWithoutFree();
    bool isValid()const { return id != 0; }
    void bind();

    void link(unsigned index,
        unsigned vbo, unsigned numVals, unsigned stride, unsigned offset);
    
    template <typename T>
    void link(unsigned index,
        unsigned vbo, unsigned stride, unsigned offset)
    {
        return link(index, vbo, numElems<T>(), stride, offset);
    }

    template <unsigned i, typename... Ts>
    void link(unsigned index, VboTAttribRef<i, Ts...> attribRef)
    {
        return link<type_at_index<i, Ts...>>(index,
            attribRef.vbo.getId(), total_size<Ts...>(), getVertexElemOffset<i, Ts...>());
    }
    
    void free();

    static size_t getNumCreated();
    static size_t getNumFreed();
private:
    unsigned id;

protected:
    static unsigned bound;
#ifdef MEMORY_DEBUG_MODE
    static size_t s_numCreated;
    static size_t s_numFreed;
#endif
};

// --- draw ---
void draw(Vao vao, Primitive primitive, unsigned firstVert, unsigned numVerts);
void draw(Vao vao, Primitive primitive, unsigned numVerts);

// --- line stuff ---
class line
{
public:
    // thickness / width
    static float getThickness();
    static void setThickness(float thickness);
    // smooth: anti-aliasing
    static bool isSmoothEnabled();
    static void enableSmooth(bool enable);
};

}
