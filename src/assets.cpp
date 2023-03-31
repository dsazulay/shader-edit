#include "assets.h"

#include <string_view>

auto Shader::compile(std::string_view vertexSrc, std::string_view fragSrc) -> void
{
    /*
    unsigned int vertexID{}, fragID{};
    int success{};
    std::string infoLog{};


    vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexID, 1, &vertexSrc, nullptr);
    glCompileShader(vertexID);

    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexID, Shader::logBufferSize, nullptr, infoLog.data());
        fmt::print("Vertex shader compilation failed: {}\n", infoLog);
    }

    fragID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragID, 1, &fragSrc, nullptr);
    glCompileShader(fragID);

    glGetShaderiv(fragID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragID, Shader::logBufferSize, nullptr, infoLog.data());
        fmt::print("Fragment shader compilation failed: {}\n", infoLog);
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexID);
    glAttachShader(ID, fragID);

    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, Shader::logBufferSize, nullptr, infoLog.data());
        fmt::print("Shader program linking failed: {}\n", infoLog);
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragID);
    */
}

auto Shader::use() const -> void
{
    //glUseProgram(ID);
}

auto Shader::setUniformBlock(std::string_view blockName, int blockID) -> void
{
    //glUniformBlockBinding(ID, glGetUniformBlockIndex(ID, blockName), blockID);
}

auto Shader::setInt(std::string_view name, int value) const -> void
{
    //glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

auto Shader::setVec2(std::string_view name, float x, float y) const -> void
{
    //glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

