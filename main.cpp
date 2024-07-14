#include <SDL2/SDL.h>
#include <memory>
#include "Debug/Log.h"
#include "Debug/Config.h"
#include "Engine.h"
#include "game.h"

int main()
{
    OpenLogFile(true);
    const std::string cfgFile("game.cfg");
    const std::map<std::string, int32_t> GameDimensionPair = LoadConfigFile(cfgFile);

    const int32_t width = GameDimensionPair.find("width")->second;
    const int32_t height = GameDimensionPair.find("height")->second;
    const bool bIsFullScreen = GameDimensionPair.find("full_screen")->second;

    const auto engine = std::make_unique<CEngine>();
    if (engine->InitializeWindow(APP_TITLE, width, height, bIsFullScreen) == false)
    {
        TraceError("Failed To Initialize Engine Window!");
        return (EXIT_FAILURE);
    }

   if (engine->InitializeOpenGL() == false)
    {
        TraceError("Failed To Initialize Engine OpenGL!");
        return (EXIT_FAILURE);
    }

    engine->InitializeWorld();

    engine->ShowMenu();
    while(engine->Start())
    return (EXIT_SUCCESS);
}
