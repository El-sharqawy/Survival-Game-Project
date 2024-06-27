//
// Created by osama on 09/05/24.
//

#ifndef COLLISIONSPHERE_H
#define COLLISIONSPHERE_H

#include "Vector3D.h"

class CCollisionSphere
{
public:
    CCollisionSphere();
    CCollisionSphere(const CVector3D& vec, float rad);
    ~CCollisionSphere() = default;

    [[nodiscard]]float GetRadius() const;
    [[nodiscard]]CVector3D GetCenter() const;

    void SetRadius(float fAmount);
    void SetCenter(CVector3D vPos);

private:
    float fRadius;
    CVector3D vCenter;
}; // CCollisionSphere

#endif //COLLISIONSPHERE_H
