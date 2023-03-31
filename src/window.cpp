#include "window.h"

#include <stdexcept>

Window::Window(int width, int height, std::string_view name)
    : m_width{width}, m_height{height}, m_name{name}
{
    init();
}

Window::~Window()
{
    glfwDestroyWindow(m_glfwWindow);
    glfwTerminate();
}

auto Window::init() -> void
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_glfwWindow = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);
}

auto Window::shouldClose() -> bool
{
    return glfwWindowShouldClose(m_glfwWindow);
}

auto Window::pollEvents() -> void
{
    glfwPollEvents();
}

auto Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) -> void
{
    if (glfwCreateWindowSurface(instance, m_glfwWindow, nullptr, surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create surface window");
    }
}
