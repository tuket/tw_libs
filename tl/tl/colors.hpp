#pragma once

#include <glm/vec4.hpp>

namespace tl
{
namespace colors
{

static inline 
glm::vec4 black(float alpha = 1)
    { return {0, 0, 0, alpha}; }

static inline
glm::vec4 white(float alpha = 1)
    { return {1, 1, 1, alpha}; }

static inline
glm::vec4 red(float alpha = 1)
    { return {1, 0, 0, alpha}; }

static inline
glm::vec4 green(float alpha = 1)
    { return {0, 1, 0, alpha}; }

static inline
glm::vec4 blue(float alpha = 1)
    { return {0, 0, 1, alpha}; }

static inline
glm::vec4 mangenta(float alpha = 1)
    { return {1, 0, 1, alpha}; }

static inline
glm::vec4 cyan(float alpha = 1)
    { return {0, 1, 1, alpha}; }

static inline
glm::vec4 yellow(float alpha = 1)
    { return {1, 1, 0, alpha}; }

}
}