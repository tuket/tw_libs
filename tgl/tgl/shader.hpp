#pragma once

#include <tl/str.hpp>

namespace tgl
{

class ShaderObj
{
public:
    ShaderObj() : id(0) {}
    unsigned getId()const { return id; }
    bool isValid()const { return id != 0; }
    void free();
    ~ShaderObj();

protected:
    static ShaderObj create(CStr src, unsigned type);

    unsigned id;
};

class VertShader : public ShaderObj
{
public:
    static VertShader create(CStr src);
    static VertShader load(CStr fileName);
};

class FragShader : public ShaderObj
{
public:
    static FragShader create(CStr src);
    static FragShader load(CStr fileName);
};

// --- ShaderProgram ---
class ShaderProgram
{
public:
    ShaderProgram() : _id(0) {}

    static ShaderProgram create(CStr vertSrc, const CStr fragSrc);
    static ShaderProgram create(const VertShader& vertShad, const FragShader& fragShad);
    static ShaderProgram load(CStr vertFile, CStr fragFile);

    bool isValid()const{ return _id; }
    void use()const;

    template <typename T>
    void set(CStr unifName, const T& x)const;

    bool hasUnif(CStr unifName)const;

    void free();

private:
    unsigned _id;

    int getLocation(CStr unifName)const;

    static unsigned s_used;
};

}
