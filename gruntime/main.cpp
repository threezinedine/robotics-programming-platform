#include <cstdio>
#include "core/core.h"

using namespace rpp;

int main(void)
{
    Graphics graphics;

    graphics.Init();

    Scope<Window> window = graphics.CreateWindow(800, 600, "RPP Window");
    String title = "RPP Window - \n";

    while (!window->ShouldWindowClose())
    {
        // Main loop
        window->PollEvents();

        // Clear color command
        ClearColorCommandData clearColorData = {{0.1f, 0.2f, 0.3f, 1.0f}};
        GraphicsCommandData commandData = {GraphicsCommandType::CLEAR_COLOR, &clearColorData};
        window->ExecuteCommand(commandData);

        // Swap buffers command
        GraphicsCommandData swapBuffersCommand = {GraphicsCommandType::SWAP_BUFFERS, nullptr};
        window->ExecuteCommand(swapBuffersCommand);
    }

    graphics.Shutdown();

    return 0;
}