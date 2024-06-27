//
// Created by osama on 09/05/24.
//

#ifndef ENGINE_H
#define ENGINE_H

#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

#include "BaseLib/Camera.h"
#include "BaseLib/Map.h"
#include "BaseLib/ObjectLoader.h"
#include "UserLib/Player.h"

class CEngine
{
public:
    CEngine();
    ~CEngine();

    bool InitializeWindow(const std::string &title, int32_t iWidth, int32_t iHeight, bool bIsFullScreen);
    bool InitializeOpenGL();
    static void Destroy();

    bool Start();
    void UpdateEngine() const;
    void Show() const;
    static void ChangeSkyBox(int32_t skyNum);

protected:
    static SDL_Window *GetWindow() { return window; }
    static SDL_Renderer *GetRenderer() { return renderer; }
    [[nodiscard]] int32_t GetWidth() const { return width; }
    [[nodiscard]] int32_t GetHeight() const { return height; }

    void LoadAnimations(std::vector<uint32_t> &animsID, std::string fileName, int32_t frames);

private:
    std::string appTitle;
    int32_t width, height;
    static SDL_Window *window;
    static SDL_Renderer *renderer;
    static SDL_Texture *texture;
    static SDL_GLContext glContext;
    CObjectLoader objLoader;
    static std::vector<CMap*> myMaps;
    CPlayer *Player;
}; // CEngine


#endif //ENGINE_H
