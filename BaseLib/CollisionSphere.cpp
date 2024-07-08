//
// Created by osama on 09/05/24.
//

#include "CollisionSphere.h"

CCollisionSphere::CCollisionSphere()
{
    fRadius = 0.0f;
}

CCollisionSphere::CCollisionSphere(const CVector3D& vec, const float rad)
{
    vCenter = vec;
    fRadius = rad;
}

float CCollisionSphere::GetRadius() const
{
    return (fRadius);
}

CVector3D &CCollisionSphere::GetCenter()
{
    return (vCenter);
}

void CCollisionSphere::SetRadius(const float fAmount)
{
    fRadius = fAmount;
}

void CCollisionSphere::SetCenter(const CVector3D vPos)
{
    vCenter.Change(vPos);
}
