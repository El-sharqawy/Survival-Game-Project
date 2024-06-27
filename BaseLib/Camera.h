//
// Created by osama on 09/05/24.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <SDL_video.h>

#include "Vector3D.h"

class CCamera
{
public:
    CCamera();
    explicit CCamera(const CVector3D& vLoc);
    CCamera(const CVector3D& vLoc, float fPitch, float fYaw);
    CCamera(const CVector3D& vLoc, float fPitch, float fYaw, float fCamSpeed, float fMouseSpeed);
    ~CCamera() = default;

    void UpdateCamera() const;
    void UpdateCamera(const CVector3D& vLocation);
    void Control();

    [[nodiscard]]CVector3D GetVector() const;
    [[nodiscard]]CVector3D GetCameraLocation() const;

    [[nodiscard]]float GetCameraPitch() const;
    [[nodiscard]]float GetCameraYaw() const;
    [[nodiscard]]float GetCameraSpeed() const;
    [[nodiscard]]float GetMouseSpeed() const;

    void SetCameraLocation(const CVector3D& vLocation);
    void LookAt(const float& fPitch, const float& fYaw);

    void SetWindow(SDL_Window *window);
    [[nodiscard]]SDL_Window *GetWindow() const;

    [[nodiscard]]bool IsMouseIn() const;
    void SetMouseIn();
    void SetMouseOut();

    void SetCameraSpeed(float fCamSpeed, float fMouseSpeed);
    void MoveCameraRight(const float& fDir);
    void MoveCameraUp(const float& fDir);

    [[nodiscard]]bool IsJumping() const;
    void SetIsJumping(bool bIsJump);

protected:
    void LockCamera();

private:
    CVector3D vCamLocation;
    float camPitch, camYaw;
    float camSpeed;
    float mouseSpeed;
    bool bIsMouseInsideWindow;
    bool bIsJumping;
    SDL_Window *camWnd;
}; // CCamera

#endif //CAMERA_H
