//
// Created by osama on 09/05/24.
//

#ifndef COLLISION_H
#define COLLISION_H

#include "CollisionPlane.h"

class CCollision
{
public:
    CCollision() = default;
    ~CCollision() = default;
public:
    static bool RaySphere(float xc, float yc, float zc, float xd, float yd, float zd, float xs, float ys, float zs,
                          float rad, float *distance = nullptr, CVector3D *collPoint = nullptr);
    static bool RayPlane(const float &normalVectorX, float normalVectorY, float normalVectorZ, float xPosition,
                         float yPosition, float zPosition, float xStartPoint, float yStartPoint, float zStartPoint,
                         float xDirection, float yDirection, float zDirection, CVector3D p1, CVector3D p2, CVector3D p3,
                         CVector3D p4, float *distance, CVector3D *point = nullptr);
    static float TriangleArea(CVector3D p1, CVector3D p2, CVector3D p3);
    static bool SphereSphere(CVector3D& p1, float fRadius1, const CVector3D& p2, float fRadius2);
    static float PointDistance(CVector3D p1, CVector3D p2);
    static float PointDistanceSquare(CVector3D p1, CVector3D p2);
    static bool SpherePlane(CVector3D &vSpherePos, CVector3D vNormalPlane, CVector3D p1, CVector3D p2, CVector3D p3,
                            CVector3D p4, float fRadius);
    static float RayPlaneDistance(float normalVectorX, float normalVectorY, float normalVectorZ, float xPosition,
                                  float yPosition, float zPosition, float xStartPoint, float yStartPoint,
                                  float zStartPoint, float xDirection, float yDirection, float zDirection);
}; // CCollision

#endif //COLLISION_H
