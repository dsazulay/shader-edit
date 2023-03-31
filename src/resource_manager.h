#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>

#include "assets.h"

class ResourceManager
{
public:
    static auto loadShader(std::string_view vertShaderFile, std::string_view fragShaderFile, std::string_view name) -> Shader*;
    static auto recompileShaders() -> void;

    static std::unordered_map<std::string, Shader> shaders;

private:
    ResourceManager() = default;
    static auto readShaderFile(std::string_view path) -> std::vector<char>;
};
