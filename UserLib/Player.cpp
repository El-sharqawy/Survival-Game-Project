#include "Player.h"

#include <SDL2/SDL.h>
#include "../BaseLib/Collision.h"
#include "../Debug/Log.h"

CPlayer::CPlayer(const std::string &sName, const CCollisionSphere playerSphere, const float fSpeed, const float fSprintSpd,
    const float fMouseSpd)
{
    sPlayerName = sName;
    playerColSphere = playerSphere;
    fPlayerSpeed = fSpeed;
    fSprintSpeed = fSprintSpd;
    fMouseSpeed = fMouseSpd;
    iPoints = 0;
    fPlayerHp = 100.0f;
    fPlayerEnergy = 100.0f;
    bIsCollision = false;
    bIsSprinting = false;
    bIsOnTheGround = false;
    bIsJumping = false;
    vDirection.Change(0.0,0.0,0.0);
    vGravity.Change(0.0, -0.3, 0.0);
    vPosition = playerSphere.GetCenter();
    playerCamera = new CCamera(vPosition);
    playerCamera->SetCameraSpeed(fSpeed, fMouseSpd);
    vGroundPosition.Change(0, 0, 0);
    SetPlayerPosition(CVector3D(0.0,10,0));
    playerWnd = nullptr;
}

CPlayer::~CPlayer()
{
    delete playerCamera;
    playerCamera = nullptr;
}

void CPlayer::SetPlayerPosition(const CVector3D &pos)
{
    // Update player's camera position
    playerColSphere.SetCenter(pos);
    playerCamera->SetCameraLocation(pos);
}

void CPlayer::UpdatePlayer(const std::vector<CCollisionPlane> &playerColPlane)
{
    for (const auto & playerCP : playerColPlane)
    {
        CCollision::SpherePlane(vPosition, playerCP.GetVectorNormal(), playerCP.GetFirstVectorPlane(),
                                playerCP.GetSecondVectorPlane(), playerCP.GetThirdVectorPlane(),
                                playerCP.GetFourthVectorPlane(), playerColSphere.GetRadius());
    }

    UpdatePlayerPosition();
}

void CPlayer::ShowPlayer()
{
    SetPlayerPosition(playerCamera->GetCameraLocation());
}

std::string CPlayer::GetName() const
{
    return(sPlayerName);
}

void CPlayer::SetName(const std::string &sName)
{
    sPlayerName = sName;
}

float CPlayer::GetPlayerHealth() const
{
    return (fPlayerHp);
}
void CPlayer::SetPlayerHealth(const float fHealth)
{
    fPlayerHp = fHealth;
}

CCollisionSphere CPlayer::GetCollisionSphere() const
{
    return (playerColSphere);
}

int32_t CPlayer::GetPoints() const
{
    return (iPoints);
}

void CPlayer::SetPoints(const int32_t iAmount)
{
    iPoints = iAmount;
}

void CPlayer::SetWindow(SDL_Window *hWnd)
{
    playerWnd = hWnd;
}

SDL_Window* CPlayer::GetWindow() const
{
    return (playerWnd);
}

void CPlayer::MoveForward(const bool bForward) const
{
    if (bForward)
    {
        if (GetPlayerCamera()->GetCameraPitch() != 90.0f && GetPlayerCamera()->GetCameraPitch() != -90.0f)
        {
            GetPlayerCamera()->MoveCameraRight(0.0);
        }
    }
    else
    {
        if (GetPlayerCamera()->GetCameraPitch() != 90.0f && GetPlayerCamera()->GetCameraPitch() != -90.0f)
        {
            GetPlayerCamera()->MoveCameraRight(180.0);
        }
    }
}

void CPlayer::MoveRight(const bool bRight) const
{
    if (bRight)
    {
        GetPlayerCamera()->MoveCameraRight(90.0f);
    }
    else
    {
        GetPlayerCamera()->MoveCameraRight(270.0f);
    }
}

void CPlayer::Sprint(const bool bSprint)
{
    bIsSprinting = bSprint;
}

bool CPlayer::IsJumping() const
{
    return (bIsJumping);
}

void CPlayer::Jump() const
{
    GetPlayerCamera()->SetIsJumping(true);
}

void CPlayer::UpdatePlayerPosition()
{
    bIsJumping = playerCamera->IsJumping();
    vPosition = playerCamera->GetCameraLocation();

    if (vDirection.GetY() >= vGravity.GetY())
    {
        vDirection += vGravity;
    }
    if (!bIsOnTheGround && !bIsJumping)
    {
        vPosition += vDirection;
    }

    if (bIsJumping)
    {
        vPosition -= vDirection;
    }

    SetPlayerPosition(vPosition);

    if (bIsSprinting)
    {
        playerCamera->SetCameraSpeed(fSprintSpeed, fMouseSpeed);
    }
    else
    {
        playerCamera->SetCameraSpeed(fPlayerSpeed, fMouseSpeed);
    }

    SetPlayerPosition(vPosition);

    if (vPosition.GetY() >= JUMP_VELOCITY)
    {
        playerCamera->SetIsJumping(false);
    }

    if (vPosition.GetY() < vGroundPosition.GetY())
    {
        bIsOnTheGround = true;
    }
    else
    {
        bIsOnTheGround = false;
    }
}
