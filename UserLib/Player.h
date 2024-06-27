//
// Created by osama on 11/05/24.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <SDL_events.h>
#include <vector>
#include <string>
#include "../BaseLib/CollisionSphere.h"
#include "../BaseLib/CollisionPlane.h"
#include "../BaseLib/Camera.h"

#define JUMP_VELOCITY 5.0f

class CPlayer
{
public:
    CPlayer(const std::string &sName, CCollisionSphere playerSphere, float fSpeed, float fSprintSpd,
        float fMouseSpd);
    ~CPlayer();

    void UpdatePlayer(const std::vector<CCollisionPlane> &playerColPlane);
    void ShowPlayer();
    void SetPlayerPosition(const CVector3D &pos);

    [[nodiscard]]std::string GetName() const;
    void SetName(const std::string &sName);

    [[nodiscard]]float GetPlayerHealth() const;
    void SetPlayerHealth(float fHealth);

    [[nodiscard]]CCollisionSphere GetCollisionSphere() const;
    [[nodiscard]]int32_t GetPoints() const;
    void SetPoints(int32_t iAmount);

    [[nodiscard]]CCamera *GetPlayerCamera() const { return playerCamera; }

    void SetWindow(SDL_Window *hWnd);
    [[nodiscard]]SDL_Window* GetWindow() const;

    void MoveForward(bool bForward = true) const;
    void MoveRight(bool bRight = true) const;
    void Sprint(bool bSprint);

    [[nodiscard]]bool IsJumping() const;
    void Jump() const;

    void UpdatePlayerPosition();

private:
    CCollisionSphere playerColSphere;
    CVector3D vGravity; /* should be made on the level */
    CVector3D vDirection; /* pos = pos + direction, direction = direction + gravity */
    CVector3D vPosition; /* Current Player Position */
    CCamera* playerCamera;
    bool bIsCollision;
    bool bIsOnTheGround;
    bool bIsSprinting;
    bool bIsJumping;
    std::string sPlayerName;
    float fPlayerHp;
    float fPlayerEnergy;
    float fMouseSpeed;
    float fPlayerSpeed;
    float fSprintSpeed;
    int32_t iPoints; /* Player Points */
    CVector3D vGroundPosition;
    SDL_Window *playerWnd;
};



#endif //PLAYER_H
