#include <tgl/blending.hpp>

#include <glad/glad.h>

using namespace glm;

namespace tgl
{
namespace blending
{

// --- helpers -------------------------------------------------------------------------------

GLenum toGlBlendFactor(EFactor factor)
{
    switch(factor)
    {
        case EFactor::ZERO:
            return GL_ZERO;
        case EFactor::ONE:
            return GL_ONE;
        case EFactor::SRC_COLOR:
            return GL_SRC_COLOR;
        case EFactor::ONE_MINUS_SRC_COLOR:
            return GL_ONE_MINUS_SRC_COLOR;
        case EFactor::DST_COLOR:
            return GL_DST_COLOR;
        case EFactor::ONE_MINUS_DST_COLOR:
            return GL_ONE_MINUS_DST_COLOR;
        case EFactor::SRC_ALPHA:
            return GL_SRC_ALPHA;
        case EFactor::ONE_MINUS_SRC_ALPHA:
            return GL_ONE_MINUS_SRC_ALPHA;
        case EFactor::DST_ALPHA:
            return GL_DST_ALPHA;
        case EFactor::ONE_MINUS_DST_ALPHA:
            return GL_ONE_MINUS_DST_ALPHA;
        case EFactor::CONSTANT_COLOR:
            return GL_CONSTANT_COLOR;
        case EFactor::ONE_MINUS_CONSTANT_COLOR:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case EFactor::CONSTANT_ALPHA:
            return GL_CONSTANT_ALPHA;
        case EFactor::ONE_MINUS_CONSTANT_ALPHA:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
    }
    assert(false);
}

GLenum toGlBlendEquation(EOperator op)
{
    switch(op)
    {
        case EOperator::PLUS:
            return GL_FUNC_ADD;
        case EOperator::MINUS:
            return GL_FUNC_SUBTRACT;
        case EOperator::REVERSE_MINUS:
            return GL_FUNC_REVERSE_SUBTRACT;
    }
    assert(false);
}

// -- global state ---------------------------------------------------------------------------

bool g_enabled = false;

Func g_func = Func(
    EFactor::ONE,
    EOperator::PLUS,
    EFactor::ZERO,
    vec4(0)
);

bool isEnabled()
{
    return g_enabled;
}

void enable(bool enable)
{
    g_enabled = enable;
    if(enable)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
}

const Func& getFunc()
{
    return g_func;
}

void setFunc(const Func& func)
{
    g_func = func;
    if(func.hasIndependentFuncAlpha())
    {
        glBlendFuncSeparate(
            toGlBlendFactor(func.getSrcFactor()),
            toGlBlendFactor(func.getDstFactor()),
            toGlBlendFactor(func.getAlphaSrcFactor()),
            toGlBlendFactor(func.getAlphaDstFactor()));
        glBlendEquationSeparate(
            toGlBlendEquation(func.getOperator()),
            toGlBlendEquation(func.getAlphaOperator()));
    }
    else{
        glBlendFunc(
            toGlBlendFactor(func.getSrcFactor()),
            toGlBlendFactor(func.getDstFactor()));
        glBlendEquation(
            toGlBlendEquation(func.getOperator()));
    }
    vec4 c = func.getColorConstant();
    glBlendColor(c.r, c.g, c.b, c.a);
}

// --- Func -------------------------------------------------------------------

Func::Func()
    : Func(EFactor::SRC_ALPHA, EOperator::PLUS, EFactor::ONE_MINUS_SRC_ALPHA)
{}

Func::Func(EFactor srcFactor, EOperator op, EFactor dstFactor,
    const vec4& colorConstant
)
    : independentFuncAlpha(false)
    , srcFactor(srcFactor)
    , dstFactor(dstFactor)
    , op(op)
    , colorConstant(colorConstant)
{
}

Func::Func(EFactor srcFactorColor, EOperator opColor, EFactor dstFactorColor,
    EFactor alphaSrcFactor, EOperator alphaOp, EFactor alphaDstFactor,
    const vec4& colorConstant
)
    : independentFuncAlpha(true)
    , srcFactor(srcFactorColor)
    , dstFactor(dstFactorColor)
    , op(opColor)
    , alphaSrcFactor(alphaSrcFactor)
    , alphaDstFactor(alphaDstFactor)
    , alphaOp(alphaOp)
    , colorConstant(colorConstant)
{
}

EFactor Func::getSrcFactor()const
{
    return srcFactor;
}

void Func::setSrcFactor(EFactor factor)
{
    srcFactor = factor;
}

EFactor Func::getDstFactor()const
{
    return dstFactor;
}

void Func::setDstFactor(EFactor factor)
{
    dstFactor = factor;
}

bool Func::hasIndependentFuncAlpha()const
{
    return independentFuncAlpha;
}

void Func::setIndependentFuncAlpha(bool b)
{
    independentFuncAlpha = b;
}


EFactor Func::getAlphaSrcFactor()const
{
    return alphaSrcFactor;
}

void Func::setAlphaSrcFactor(EFactor factor)
{
    alphaSrcFactor = factor;
}

EFactor Func::getAlphaDstFactor()const
{
    return alphaDstFactor;
}

void Func::setAlphaDstFactor(EFactor factor)
{
    alphaDstFactor = factor;
}

EOperator Func::getOperator()const
{
    return op;
}

void Func::setOperator(EOperator op)
{
    this->op = op;
}

EOperator Func::getAlphaOperator()const
{
    return alphaOp;
}

void Func::getAlphaOperator(EOperator op)
{
    alphaOp = op;
}


glm::vec4 Func::getColorConstant()const
{
    return colorConstant;
}

void Func::setColorConstant(const glm::vec4& color)
{
    colorConstant = color;
}


}
}