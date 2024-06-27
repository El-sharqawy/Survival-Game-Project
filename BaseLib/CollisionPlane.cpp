//
// Created by osama on 09/05/24.
//

#include "CollisionPlane.h"

CCollisionPlane::CCollisionPlane(const float normX, const float normY, const float normZ, const float Plane1X, const float Plane1Y, const float Plane1Z,
                    const float Plane2X, const float Plane2Y, const float Plane2Z, const float Plane3X, const float Plane3Y, const float Plane3Z,
                    const float Plane4X, const float Plane4Y, const float Plane4Z)

{
    vNormal.Change(normX, normY, normZ);
    vPlane[0].Change(Plane1X, Plane1Y, Plane1Z);
    vPlane[1].Change(Plane2X, Plane2Y, Plane2Z);
    vPlane[2].Change(Plane3X, Plane3Y, Plane3Z);
    vPlane[3].Change(Plane4X, Plane4Y, Plane4Z);
} // CCollisionPlane

CVector3D CCollisionPlane::GetVectorNormal() const
{
    return (vNormal);
}

const CVector3D *CCollisionPlane::GetVectorPlane() const
{
    return vPlane;
}

CVector3D CCollisionPlane::GetFirstVectorPlane() const
{
    return vPlane[0];
};

CVector3D CCollisionPlane::GetSecondVectorPlane() const
{
    return vPlane[1];
};

CVector3D CCollisionPlane::GetThirdVectorPlane() const
{
    return vPlane[2];
};

CVector3D CCollisionPlane::GetFourthVectorPlane() const
{
    return vPlane[3];
};
