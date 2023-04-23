#include "resource_manager.h"
#include "shaderc/shaderc.h"

#include <shaderc/shaderc.hpp>

#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstddef>
#include <stdexcept>

#include <fmt/core.h>

std::unordered_map<std::string, Shader> ResourceManager::shaders;

// Compiles a shader to SPIR-V assembly. Returns the assembly text
// as a string.
auto compileFileToAssembly(const std::string& source_name, shaderc_shader_kind kind, const std::string& source, bool optimize = false) -> std::string 
{
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    // Like -DMY_DEFINE=1
    options.AddMacroDefinition("MY_DEFINE", "1");
    if (optimize)
        options.SetOptimizationLevel(shaderc_optimization_level_size);

    shaderc::AssemblyCompilationResult result = compiler.CompileGlslToSpvAssembly(source, kind, source_name.c_str(), options);

    if (result.GetCompilationStatus() != shaderc_compilation_status_success) 
    {
        fmt::print("Shader compile fail: {}\n", result.GetErrorMessage());
        return "";
    }

    return {result.cbegin(), result.cend()};
}

// Compiles a shader to a SPIR-V binary. Returns the binary as
// a vector of 32-bit words.
auto compileFile(const std::string& source_name, shaderc_shader_kind kind, const std::string& source, bool optimize = false) -> std::vector<uint32_t>
{
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    // Like -DMY_DEFINE=1
    options.AddMacroDefinition("MY_DEFINE", "1");
    if (optimize)
        options.SetOptimizationLevel(shaderc_optimization_level_size);

    shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, kind, source_name.c_str(), options);

    if (module.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        fmt::print("Shader compile fail: {}\n", module.GetErrorMessage());
        return {};
    }

    return {module.cbegin(), module.cend()};
}

auto ResourceManager::loadShader(std::string_view vertShaderFile, std::string_view fragShaderFile, std::string_view name, bool isCompiled) -> Shader*
{
    std::vector<char> vertCode;
    std::vector<char> fragCode;

    if (!isCompiled)
    {
        std::string vertexCode;
        std::string fragmentCode;

        try
        {
            std::ifstream vertexFile;
            vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            vertexFile.open(vertShaderFile);

            std::ifstream fragFile;
            fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fragFile.open(fragShaderFile);

            std::stringstream vertexStream, fragStream;
            vertexStream << vertexFile.rdbuf();
            fragStream << fragFile.rdbuf();

            vertexFile.close();
            fragFile.close();

            vertexCode = vertexStream.str();
            fragmentCode = fragStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            fmt::print("Shader file not successfully read!\n");
        }
       
        auto vertCompiled = compileFile(vertShaderFile.data(), shaderc_glsl_vertex_shader, vertexCode.c_str());
        fmt::print("Compiled to a binary module with {} words.\n", vertCompiled.size());
        if (vertCompiled.size() == 0)
            return nullptr;

        auto fragCompiled = compileFile(fragShaderFile.data(), shaderc_glsl_fragment_shader, fragmentCode.c_str());
        fmt::print("Compiled to a binary module with {} words.\n", fragCompiled.size());
        if (fragCompiled.size() == 0)
            return nullptr;

        vertCode.reserve(vertCompiled.size() * 4);
        for (auto i : vertCompiled)
        {
            vertCode.push_back(i & 0x000000FF);
            vertCode.push_back((i & 0x0000FF00) >> 8);
            vertCode.push_back((i & 0x00FF0000) >> 16);
            vertCode.push_back((i & 0xFF000000) >> 24);
        }

        fragCode.reserve(fragCompiled.size() * 4);
        for (auto i : fragCompiled)
        {
            fragCode.push_back(i & 0x000000FF);
            fragCode.push_back((i & 0x0000FF00) >> 8);
            fragCode.push_back((i & 0x00FF0000) >> 16);
            fragCode.push_back((i & 0xFF000000) >> 24);
        }
    }
    else
    {
        vertCode = readShaderFile(vertShaderFile);
        fragCode = readShaderFile(fragShaderFile);

        fmt::print("vert code size: {}\n", vertCode.size());
        fmt::print("frag code size: {}\n", fragCode.size());
    }

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
            loadShader(shader.vertFilePath.c_str(), shader.fragFilePath.c_str(), name, false);
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
