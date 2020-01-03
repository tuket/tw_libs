#pragma once

#include <glad/glad.h>

namespace tgl
{
namespace strings
{
	static const char* vendor() {
		return (const char*)glGetString(GL_VENDOR);
	}
	static const char* renderer() {
		return (const char*)glGetString(GL_RENDERER);
	}
	static const char* version() {
		return (const char*)glGetString(GL_VERSION);
	}
	static const char* shadingLanguageVersion() {
		return (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	}
	static const char* extensions() {
		return (const char*)glGetString(GL_EXTENSIONS);
	}
#ifndef glGetStringi
	static const char* extension(unsigned i) {
		return (const char*)glGetStringi(GL_EXTENSIONS, GLuint);
	}
#endif
}
}