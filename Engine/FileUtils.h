#pragma once
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <GL/glew.h>  // defines GLuint, glCreateProgram, etc.

std::string ReadFile(const std::string& path);
void CheckProgramLinkErrors(GLuint program);
void CheckShaderCompileErrors(GLuint shader);