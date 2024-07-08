//
// Created by osama on 09/05/24.
//

#ifndef ENGINE_H
#define ENGINE_H

#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

#include "BaseLib/Map.h"
#include "BaseLib/ObjectLoader.h"
#include "BaseLib/SnowParticle.h"
#include "BaseLib/Text.h"
#include "UserLib/Player.h"
#include "UserLib/Zombie.h"

class CEngine
{
public:
    CEngine();
    ~CEngine();

    bool InitializeWindow(const std::string &title, int32_t iWidth, int32_t iHeight, bool bIsFullScreen);
    [[nodiscard]]bool InitializeOpenGL() const;
    void InitializeWorld();
    void Destroy();

    bool Start();

protected:
    static SDL_Window *GetWindow() { return m_hWindow; }
    [[nodiscard]] int32_t GetWidth() const { return m_iWidth; }
    [[nodiscard]] int32_t GetHeight() const { return m_iHeight; }
    [[nodiscard]] CPlayer *GetPlayer() const { return m_pPlayer; }
    [[nodiscard]] CWeapon *GetWeapon() const { return m_pPlayer->GetCurrentWeapon(); }

    void LoadAnimations(std::vector<uint32_t> &framesID, const std::string& fileName, uint32_t uiNum);
    void ChangeSkyBox(int32_t skyNum) const;
    void DrawPlayerStatus(float fHealth, uint32_t iAmmo, uint32_t iAllAmmo, int32_t iPoints, const std::string& stWeaponName) const;

    void UpdateEngine();
    void Show();

    Mix_Chunk *LoadSound(const char *c_szFileName);

    void initParticles(int i);
    void initPartSystem();
    void drawRain();
    void drawScene();

private:
    std::string m_stAppTitle;
    int32_t m_iWidth, m_iHeight;
    bool m_bIsRunning;
    bool m_bIsFiring;
    static SDL_Window *m_hWindow;
    static SDL_GLContext m_glContext;
    CObjectLoader m_ObjLoader;
    std::vector<CMap> m_vMyMaps;
    std::vector<CWeapon> m_vMyWeapons;
    std::vector<CZombie *> m_vMyZombies;
    std::vector<uint32_t> m_vModels;
    std::vector<Mix_Chunk*> m_vSounds;
    Mix_Music *m_Music;
    CPlayer *m_pPlayer;
    CText *m_pText;
    CZombie *m_pZombie;
}; // CEngine


#endif //ENGINE_H
