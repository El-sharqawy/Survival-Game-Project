#include "Camera.h"
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <cmath>
#include "../game.h"

CCamera::CCamera()
{
    vCamLocation.Change(0.0f, 0.0f, 0.0f);
    camPitch = 0.0f;
    camYaw = 0.0f;
    camSpeed = 0.2f;
    mouseSpeed = 0.2f;
    bIsMouseInsideWindow = false;
    camWnd = nullptr;
    bIsJumping = false;
}

CCamera::CCamera(const CVector3D& vLoc)
{
    vCamLocation.Change(vLoc);
    camPitch = 0.0f;
    camYaw = 0.0f;
    camSpeed = 0.2f;
    mouseSpeed = 0.2f;
    bIsMouseInsideWindow = false;
    camWnd = nullptr;
    bIsJumping = false;
}

CCamera::CCamera(const CVector3D& vLoc, const float fPitch, const float fYaw)
{
    vCamLocation.Change(vLoc);
    camPitch = fPitch;
    camYaw = fYaw;
    camSpeed = 0.2f;
    mouseSpeed = 0.2f;
    bIsMouseInsideWindow = false;
    camWnd = nullptr;
    bIsJumping = false;
}

CCamera::CCamera(const CVector3D& vLoc, const float fPitch, const float fYaw, const float fCamSpeed, const float fMouseSpeed)
{
    vCamLocation.Change(vLoc);
    camPitch = fPitch;
    camYaw = fYaw;
    camSpeed = fCamSpeed;
    mouseSpeed = fMouseSpeed;
    bIsMouseInsideWindow = false;
    camWnd = nullptr;
    bIsJumping = false;
}

void CCamera::UpdateCamera() const
{
    glTranslatef(-vCamLocation.GetX(), -vCamLocation.GetY(), -vCamLocation.GetZ());
}

void CCamera::UpdateCamera(const CVector3D& vLocation)
{
    glTranslatef(-vLocation.GetX(), -vLocation.GetY(), -vLocation.GetZ());
}

SDL_Window *CCamera::GetWindow() const
{
    return (camWnd);
}

void CCamera::SetWindow(SDL_Window* window)
{
    camWnd = window;
}

void CCamera::Control()
{
    if (bIsMouseInsideWindow)
    {
        constexpr int32_t midX = 320;
        constexpr int32_t midY = 240;
        SDL_ShowCursor(SDL_DISABLE);
        int32_t tempX, tempY;
        SDL_GetMouseState(&tempX, &tempY);
        camYaw += mouseSpeed * static_cast<float>((midX - tempX));
        camPitch += mouseSpeed * static_cast<float>((midY - tempY));
        LockCamera();
        SDL_WarpMouseInWindow(GetWindow(), midX, midY);
    }
    glRotatef(-camPitch, 1.0f, 0.0, 0.0);
    glRotatef(-camYaw, 0.0f, 1.0, 0.0);
    UpdateCamera();
}

/* Change the Spherical coordinate system to cartesian */
CVector3D CCamera::GetVector() const
{
    CVector3D tempVec;
    const auto x = -static_cast<float>(std::cos(camPitch * M_PI / 180.0f) * std::sin(camYaw * M_PI / 180.0f));
    const auto y = static_cast<float>(std::sin(camPitch * M_PI / 180.0f));
    const auto z = -static_cast<float>(std::cos(camPitch * M_PI / 180.0f) * std::cos(camYaw * M_PI / 180.0f));
    tempVec.Change(x, y, z);
    return (tempVec);
}

CVector3D CCamera::GetCameraLocation() const
{
    return (vCamLocation);
}

float CCamera::GetCameraPitch() const
{
    return (camPitch);
}

float CCamera::GetCameraYaw() const
{
    return (camYaw);
}

float CCamera::GetCameraSpeed() const
{
    return (camSpeed);
}

float CCamera::GetMouseSpeed() const
{
    return (mouseSpeed);
}

void CCamera::SetCameraLocation(const CVector3D& vLocation)
{
    vCamLocation = vLocation;
}

void CCamera::LookAt(const float& fPitch,const float& fYaw)
{
    camPitch = fPitch;
    camYaw = fYaw;
}

void CCamera::SetMouseIn()
{
    bIsMouseInsideWindow = true;
}

void CCamera::SetMouseOut()
{
    bIsMouseInsideWindow = false;
}

void CCamera::SetCameraSpeed(const float fCamSpeed, const float fMouseSpeed)
{
    camSpeed = fCamSpeed;
    mouseSpeed = fMouseSpeed;
}

bool CCamera::IsMouseIn() const
{
    return (bIsMouseInsideWindow);
}

void CCamera::LockCamera()
{
    if (camPitch > 90.0f)
    {
        camPitch = 90.0f;
    }
    if (camPitch < -90.0f)
    {
        camPitch = -90.0f;
    }

    if (camYaw < 0.0f)
    {
        camYaw += 360.0f;
    }
    if (camYaw > 360.0f)
    {
        camYaw -= 360.0f;
    }
}

void CCamera::MoveCameraRight(const float& fDir)
{
    const float rad = (camYaw + fDir) * static_cast<float>(M_PI) / 180.0f;
    vCamLocation.SetX(vCamLocation.GetX()-(std::sin(rad) * camSpeed));
    vCamLocation.SetZ(vCamLocation.GetZ()-(std::cos(rad) * camSpeed));
}

void CCamera::MoveCameraUp(const float& fDir)
{
    const float rad = (camPitch + fDir) * static_cast<float>(M_PI) / 180.0f;
    vCamLocation.SetY(vCamLocation.GetY()-(std::sin(rad) * camSpeed));
}

bool CCamera::IsJumping() const
{
    return (bIsJumping);
}

void CCamera::SetIsJumping(bool bIsJump)
{
    bIsJumping = bIsJump;
}
