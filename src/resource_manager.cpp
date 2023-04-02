#include "resource_manager.h"

#include <fstream>
#include <filesystem>
#include <cstddef>
#include <stdexcept>

#include <fmt/core.h>

std::unordered_map<std::string, Shader> ResourceManager::shaders;

auto ResourceManager::loadShader(std::string_view vertShaderFile, std::string_view fragShaderFile, std::string_view name) -> Shader*
{
    std::vector<char> vertCode = readShaderFile(vertShaderFile);
    std::vector<char> fragCode = readShaderFile(fragShaderFile);

    fmt::print("vert code size: {}\n", vertCode.size());
    fmt::print("frag code size: {}\n", fragCode.size());

/*
    if (shaders.contains(name))
    {
        shaders[name].compile(vertexCode.c_str(), fragCode.c_str());
        shaders[name].vertLastWriteTime = std::filesystem::last_write_time(vertShaderFile);
        shaders[name].fragLastWriteTime = std::filesystem::last_write_time(fragShaderFile);
    }
    else
    {}*/
    Shader shader{};
    shader.m_vertCode = vertCode;
    shader.m_fragCode = fragCode;
    shader.vertFilePath = vertShaderFile;
    shader.fragFilePath = fragShaderFile;
    shader.vertLastWriteTime = std::filesystem::last_write_time(vertShaderFile);
    shader.fragLastWriteTime = std::filesystem::last_write_time(fragShaderFile);
    shaders[std::string(name)] = shader;
    
    return &shaders[std::string(name)];
}

auto ResourceManager::recompileShaders() -> void
{
    for (const auto& [name, shader] : shaders)
    {
        if (shader.vertLastWriteTime != std::filesystem::last_write_time(shader.vertFilePath) ||
            shader.fragLastWriteTime != std::filesystem::last_write_time(shader.fragFilePath))
        {
            loadShader(shader.vertFilePath.c_str(), shader.fragFilePath.c_str(), name);
            fmt::print("{} shader recompiled\n", name);
        }
    }
}

auto ResourceManager::readShaderFile(std::string_view path) -> std::vector<char>
{
    std::ifstream file{path, std::ios::ate | std::ios::binary};
    if (!file.is_open())
    {
        throw std::runtime_error(fmt::format("Failed to open file: {}", path));
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}
