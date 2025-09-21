#include "platforms/memory.h"
#include "core/core.h"

RPP_ENABLE_MEMORY_TRACKING;

using namespace rpp;

int main(void)
{
    SingletonManager::Initialize();
    Logging::GetInstance()->SetupHandler(CreateScope<ConsoleHandler>());

    Graphics graphics;

    graphics.Init();
    RPP_LOG_INFO("Graphics initialized successfully.");
    RPP_LOG_ERROR("Graphics initialization failed.");

    Scope<Window> window = graphics.CreateWindow(800, 600, "RPP Window");
    String title = "RPP Window - \n";

	String msg = Format("Memory allocated: {} bytes\n", GetMemoryAllocated());

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

    SingletonManager::Shutdown();
    return 0;
}