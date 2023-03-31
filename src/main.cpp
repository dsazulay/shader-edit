#include "app.h"

#include <exception>
#include <fmt/core.h>

auto main() -> int
{
    App app{};

    try
    {
        app.run();
    }
    catch (const std::exception& e)
    {
        fmt::print("{}\n", e.what());
        return -1;
    }

    return 0;
}
