#pragma once

#include <glm/vec4.hpp>

namespace tgl
{
namespace blending
{

class Func;

bool isEnabled();
void enable(bool enable = true);

const Func& getFunc();
void setFunc(const Func& func);

enum class EFactor
{
    ZERO,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA
};

enum class EOperator
{
    PLUS,
    MINUS,
    REVERSE_MINUS
};

class Func
{
public:
    // alpha * src + (1-alpha) * dst
    Func();

    // srcFactor * src <op> dstFactor * dst
    Func(EFactor srcFactor, EOperator op, EFactor dstFactor,
        const glm::vec4& constantColor = glm::vec4(1));

    // dstColor = srcFactorColor * srcColor <opColor> dstFactorColor * dstColor
    // dstAlpha = srcFactorAlpha * srcAlpha <opAlpha> dstFactorAlpha * dstAlpha
    Func(EFactor srcFactorColor, EOperator opColor, EFactor dstFactorColor,
        EFactor srcFactorAlpha, EOperator opAlha, EFactor dstFactorAlpha,
        const glm::vec4& constColor = glm::vec4(1));

    EFactor getSrcFactor()const;
    void setSrcFactor(EFactor factor);
    EFactor getDstFactor()const;
    void setDstFactor(EFactor factor);

    bool hasIndependentFuncAlpha()const;
    void setIndependentFuncAlpha(bool b);

    EFactor getAlphaSrcFactor()const;
    void setAlphaSrcFactor(EFactor factor);
    EFactor getAlphaDstFactor()const;
    void setAlphaDstFactor(EFactor factor);

    EOperator getOperator()const;
    void setOperator(EOperator op);
    EOperator getAlphaOperator()const;
    void getAlphaOperator(EOperator op);

    glm::vec4 getColorConstant()const;
    void setColorConstant(const glm::vec4& color);

private:
    EFactor srcFactor;
    EFactor dstFactor;
    EOperator op;
    bool independentFuncAlpha;  // there is an independent  function for alpha
    EFactor alphaSrcFactor;
    EFactor alphaDstFactor;
    EOperator alphaOp;
    glm::vec4 colorConstant;
};

}
}