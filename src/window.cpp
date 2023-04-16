#include "window.h"
#include "GLFW/glfw3.h"
#include "fmt/core.h"

#include <stdexcept>

bool Window::recompileShader = false;

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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_glfwWindow = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_glfwWindow, this);
    glfwSetKeyCallback(m_glfwWindow, keyboardCallback);
    glfwSetFramebufferSizeCallback(m_glfwWindow, framebufferResizeCallback);
}

auto Window::shouldClose() -> bool
{
    return glfwWindowShouldClose(m_glfwWindow);
}

auto Window::pollEvents() -> void
{
    glfwPollEvents();
}

auto Window::getExtent() -> VkExtent2D
{
    return {static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)};
}

auto Window::wasWindowResized() -> bool 
{
    return m_framebufferResized;
}

auto Window::resetWindowResizedFlag() -> void
{
    m_framebufferResized = false;
}

auto Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) -> void
{
    if (glfwCreateWindowSurface(instance, m_glfwWindow, nullptr, surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create surface window");
    }
}


auto Window::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        recompileShader = true;
    }
}


auto Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) -> void
{
    auto appWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    appWindow->m_width = width;
    appWindow->m_height = height;
    appWindow->m_framebufferResized = true;
}
