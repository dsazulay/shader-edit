#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <string_view>

class Window
{
public:
    Window(int width, int height, std::string_view name);
    ~Window();
    Window(const Window& other) = delete;
    Window(Window&& other) = delete;
    auto operator=(const Window& other) -> Window& = delete;
    auto operator=(Window&& other) -> Window& = delete;

    auto shouldClose() -> bool;
    auto pollEvents() -> void;
    auto getExtent() -> VkExtent2D;

    auto createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) -> void;

private:
    auto init() -> void;

    GLFWwindow* m_glfwWindow{};
    int m_width;
    int m_height;
    std::string m_name;
};
