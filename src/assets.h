#pragma once

#include <string>
#include <string_view>
#include <filesystem>

class Shader
{
public:
    auto compile(std::string_view vertexSrc, std::string_view fragSrc) -> void;
    auto use() const -> void;
    auto setUniformBlock(std::string_view blockName, int blockID) -> void;
    auto setInt(std::string_view name, int value) const -> void;
    auto setVec2(std::string_view name, float x, float y) const -> void;

    std::string vertFilePath;
    std::string fragFilePath;
    std::filesystem::file_time_type vertLastWriteTime;
    std::filesystem::file_time_type fragLastWriteTime;

private:
    unsigned int ID;

    constexpr static int logBufferSize = 512;
};
