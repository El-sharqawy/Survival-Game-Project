//
// Created by osama on 09/05/24.
//

#ifndef COLLISIONPLANE_H
#define COLLISIONPLANE_H

#include "Vector3D.h"

class CCollisionPlane
{
public:
    CCollisionPlane(float normX, float normY, float normZ, float Plane1X, float Plane1Y, float Plane1Z,
                    float Plane2X, float Plane2Y, float Plane2Z, float Plane3X, float Plane3Y, float Plane3Z,
                    float Plane4X, float Plane4Y, float Plane4Z);
    ~CCollisionPlane() = default;

    [[nodiscard]]CVector3D GetVectorNormal() const;
    [[nodiscard]]const CVector3D *GetVectorPlane() const;
    [[nodiscard]]CVector3D GetFirstVectorPlane() const;
    [[nodiscard]]CVector3D GetSecondVectorPlane() const;
    [[nodiscard]]CVector3D GetThirdVectorPlane() const;
    [[nodiscard]]CVector3D GetFourthVectorPlane() const;

private:
    CVector3D vPlane[4];
    CVector3D vNormal;

}; // CCollisionPlane

#endif //COLLISIONPLANE_H
