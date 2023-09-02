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
    auto wasWindowResized() -> bool;
    auto resetWindowResizedFlag() -> void;

    auto createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) -> void;

    static bool recompileShader;
    static bool saveImage;

private:
    auto init() -> void;
    static auto keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void;
    static auto framebufferResizeCallback(GLFWwindow* window, int width, int height) -> void;

    GLFWwindow* m_glfwWindow{};
    int m_width;
    int m_height;
    bool m_framebufferResized = false;
    std::string m_name;
};
