#include "FileUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

std::string ReadFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void CheckShaderCompileErrors(GLuint shader)
{
    GLint success;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);

    if (!success)
    {
        GLint logLength = 0;
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&logLength);

        std::vector<GLchar> infoLog(static_cast<size_t>(logLength));
        glGetShaderInfoLog(shader,logLength,nullptr,infoLog.data());

        std::cerr << "ERROR::SHADER_COMPILATION_ERROR:\n" << infoLog.data() << std::endl;
    }
}

void CheckProgramLinkErrors(GLuint program)
{
    GLint success;
    glGetProgramiv(program,GL_LINK_STATUS,&success); // check link status

    if (!success)
    {
        GLint logLength = 0;
        glGetProgramiv(program,GL_INFO_LOG_LENGTH,&logLength); // get error log length

        std::vector<GLchar> infoLog(static_cast<size_t>(logLength));
        glGetProgramInfoLog(program,logLength,nullptr,infoLog.data());

        std::cerr << "ERROR::PROGRAM_LINKING_ERROR:\n" << infoLog.data() << std::endl;
    }
}
