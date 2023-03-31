#include "app.h"
#include "resource_manager.h"

auto App::run() -> void
{
    ResourceManager::loadShader("../resources/unlit.vert.spv", "../resources/unlit.frag.spv", "Unlit");
    while(!m_window.shouldClose())
    {
        m_window.pollEvents();
    }
}
