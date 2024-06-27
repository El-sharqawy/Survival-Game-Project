//
// Created by osama on 10/05/24.
//

#ifndef SKYBOX_H
#define SKYBOX_H

#include "../game.h"

class CSkyBox
{
public:
    enum ESkyBoxes
    {
        SKY_LEFT,
        SKY_BACK,
        SKY_RIGHT,
        SKY_FRONT,
        SKY_TOP,
        SKY_BOTTOM
    };

    explicit CSkyBox();
    explicit CSkyBox(const char* left, const char* back, const char* right, const char* front, const char* top, const char* bottom);
    ~CSkyBox();

public:
    void Change(const char* left, const char* back, const char* right, const char* front, const char* top, const char* bottom);
    void DrawSkyBox(float fSize);

private:
    uint32_t skyboxes[6];
    std::string m_stSkyName;
    std::string m_stSkyTextureName;
    float fSkyBoxSize;
};



#endif //SKYBOX_H
