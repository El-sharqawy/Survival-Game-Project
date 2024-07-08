#include "Player.h"

#include "../BaseLib/Collision.h"

CPlayer::CPlayer(const std::string &sName, CCollisionSphere playerSphere, const float fSpeed, const float fSprintSpd,
    const float fMouseSpd, CWeapon* weapon)
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
    m_vWeapons.emplace_back(weapon);
    m_iCurWeapon = 0;
    m_bIsWeapon = true;
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

void CPlayer::ShowPlayer()
{
    SetPlayerPosition(playerCamera->GetCameraLocation());
    if (m_bIsWeapon)
    {
        m_vWeapons[m_iCurWeapon]->Show();
    }
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

void CPlayer::AddPoints(const int32_t iAmount)
{
    iPoints += iAmount;
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

    if (vPosition.GetY() <= vGroundPosition.GetY())
    {
        bIsOnTheGround = true;
    }
    else
    {
        bIsOnTheGround = false;
    }

    if (m_bIsWeapon)
    {
        m_vWeapons[m_iCurWeapon]->Update();
    }
}

void CPlayer::AddWeapon(CWeapon *newWeapon)
{
    m_vWeapons.emplace_back(newWeapon);
}

void CPlayer::ChangeWeapon(const int32_t iWeaponNum)
{
    if (m_vWeapons.size() > iWeaponNum)
    {
        m_iCurWeapon = iWeaponNum;
    }
}

void CPlayer::ChangeWeaponUp()
{
    m_iCurWeapon++;
    if (m_iCurWeapon >= m_vWeapons.size())
    {
        m_iCurWeapon = 0;
    }
}
void CPlayer::ChangeWeaponDown()
{
    m_iCurWeapon--;
    if (m_iCurWeapon < 0)
    {
        m_iCurWeapon = static_cast<int32_t>(m_vWeapons.size()) - 1;
    }
}

CWeapon *CPlayer::GetCurrentWeapon()
{
    return (m_vWeapons[m_iCurWeapon]);
}

void CPlayer::IsHavingWeapon(const bool bIsHavingWeapon)
{
        m_bIsWeapon = bIsHavingWeapon;
}