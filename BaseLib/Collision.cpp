#include "Collision.h"
#include "Vector3D.h"
#include "../game.h"
#include <cmath>

bool CCollision::RaySphere(const float xc, const float yc, const float zc, const float xd, const float yd, const float zd,
    const float xs, const float ys, const float zs, const float rad, float *distance, CVector3D *collPoint)
{
    const float b = 2 * (xd * (xs-xc) + yd * (ys-yc) + zd * (zs - zc));
    const float c = xs * xs - 2 * xc + xc * xc + ys * ys - 2 * ys * yc + yc * yc + zs * zs - 2 * zs * zc + zc * zc - rad * rad;
    const float disc = (b * b - 4 * c);
    /* if the discriminant is less than 0, there is no intersection */
    if (disc < 0)
    {
        return false;
    }
    else
    {
        if (distance != nullptr)
        {
            (*distance) = (-b + std::sqrt(disc)) / 2;
            if (collPoint != nullptr)
            {
                const float x = xs + (*distance) * xd;
                const float y = ys + (*distance) * yd;
                const float z = zs + (*distance) * zd;
                collPoint->Change(x , y , z);
            }
        }
    }
    return (true);
}

/** TriangleArea - Function to calculate Triangle Data
 * Description: Calculate the Area of the triangle with the Heron Formula
 * with given 3 Points Vectors (x, y, z)
 *
 * @p1: Point 1 (a)
 * @p2: point 2 (b)
 * @p3: point 3 (c)
 *
 * Return: The Area of the Triangle
*/
float CCollision::TriangleArea(const CVector3D p1, const CVector3D p2, const CVector3D p3)
{
    /* Calculate distance between p1 and p2, returns the length of A */
    const float a = std::sqrt((p2.GetX()-p1.GetX()) * (p2.GetX()-p1.GetX()) +
                        (p2.GetY()-p1.GetY()) * (p2.GetY()-p1.GetY()) +
                        (p2.GetZ()-p1.GetZ()) * (p2.GetZ()-p1.GetZ()));

    /* Calculate distance between p2 and p3, returns the length of B */
    const float b = std::sqrt((p3.GetX()-p2.GetX()) * (p3.GetX()-p2.GetX()) +
                        (p3.GetY()-p2.GetY()) * (p3.GetY()-p2.GetY()) +
                        (p3.GetZ()-p2.GetZ()) * (p3.GetZ()-p2.GetZ()));

    /* Calculate distance between p3 and p1, returns the length of C */
    const float c = std::sqrt((p1.GetX()-p3.GetX()) * (p1.GetX()-p3.GetX()) +
                        (p1.GetY()-p3.GetY()) * (p1.GetY()-p3.GetY()) +
                        (p1.GetZ()-p3.GetZ()) * (p1.GetZ()-p3.GetZ()));

    /* Calcualte semi-preimeter which is the half the sum of three sides lengths */
    const float semiPremiter = (a + b + c) / 2.0f;

    /* Calcualte the Area using Heron Formula which as it looks now */
    return (std::sqrt(semiPremiter * ((semiPremiter-a) * (semiPremiter-b) * (semiPremiter-c))));
}

/** RayPlane - Ray Casting for Plane in 3D World Intersection Test
 *
 * @normalVectorX: the X Orientation of the plane in the 3D Space.
 * @normalVectorY: the Y Orientation of the plane in the 3D Space.
 * @normalVectorZ: the Z Orientation of the plane in the 3D Space.
 *
 * @xPosition: the X coordinates of a point on the Plane, defines Plane Position in 3D Space.
 * @yPosition: the Y coordinates of a point on the Plane, defines Plane Position in 3D Space.
 * @zPosition: the Z coordinates of a point on the Plane, defines Plane Position in 3D Space.
 *
 * @xStartPoint: The coordinates of the starting point X of the Ray.
 * @yStartPoint: The coordinates of the starting point Y of the Ray.
 * @zStartPoint: The coordinates of the starting point Z of the Ray.
 *
 * @xDirection: the X of Direction Vector of the Ray, Defines the direction in which ray cast from start point.
 * @yDirection: the Y of Direction Vector of the Ray, Defines the direction in which ray cast from start point.
 * @zDirection: the Z of Direction Vector of the Ray, Defines the direction in which ray cast from start point.
 *
 * @p1: CVector3D Object represents the first vertices of a Quadrilateral (a 4-sided Polygon) on the plane, Defines the shape of the Plane.
 * @p2: CVector3D Object represents the second vertices of a Quadrilateral (a 4-sided Polygon) on the plane, Defines the shape of the Plane.
 * @p3: CVector3D Object represents the third vertices of a Quadrilateral (a 4-sided Polygon) on the plane, Defines the shape of the Plane.
 * @p4: CVector3D Object represents the fourth vertices of a Quadrilateral (a 4-sided Polygon) on the plane, Defines the shape of the Plane.
 *
 * @distance: A pointer to a float var where he distance from starting point of the ray to the intersection will be stored if an intersection occurs.
 * @point: a pointer to CVector3D object where the coordinates of the Intersection point will be stored if an Intersection occurs.
 *
 * Return: true on success to find Intersection, otherwise false.
*/
bool CCollision::RayPlane(const float &normalVectorX, const float normalVectorY, const float normalVectorZ, const float xPosition,
                          const float yPosition, const float zPosition, const float xStartPoint, const float yStartPoint, const float zStartPoint,
                          const float xDirection, const float yDirection, const float zDirection, const CVector3D p1, const CVector3D p2,
                          const CVector3D p3, const CVector3D p4, float *distance, CVector3D *point)
{
    /* if the two vector dot product is 0, then there is no intersection (we don't like to divide by 0) */
    if ((xDirection * normalVectorX + yDirection * normalVectorY + zDirection * normalVectorZ) == 0)
    {
        /* The Plane is Parallel to the Plane which means no intersection occurs */
        if (normalVectorX == 0 && normalVectorY == 1 && normalVectorZ == 0)
        {
            TraceError("CCollision::RayPlane Parallel Detected, Return false, normalVectorX: %f, normalVectorY: %f, normalVectorZ: %f.", normalVectorX, normalVectorY, normalVectorZ);
        }
        return (false);
    }

    const float fInterSectionPoint = (xPosition * normalVectorX + yPosition * normalVectorY + zPosition * normalVectorZ - normalVectorX * xStartPoint - normalVectorY * yStartPoint - normalVectorZ * zStartPoint) / (xDirection * normalVectorX + yDirection * normalVectorY + zDirection * normalVectorZ);

    /* if fInterSectionPoint is less than 0 that means Intersection Point in the opposite directoin */
    if (fInterSectionPoint < 0.0f)
    {
        //TraceError("CCollision::RayPlane fInterSectionPoint less than 0 ! normalVectorX: %f, normalVectorY: %f, normalVectorZ: %f.", normalVectorX, normalVectorY, normalVectorZ);
        return (false);
    }

    /* Calculate the 3 points of CVector3D, The coordinates for Intersection Point */
    const float x = xStartPoint + fInterSectionPoint * xDirection;
    const float y = yStartPoint + fInterSectionPoint * yDirection;
    const float z = zStartPoint + fInterSectionPoint * zDirection;
    const CVector3D vInterSectionPoint(x, y, z);

    /* we divide the quadrilateral to 2 triangles, we divide one triangle to 3 (one point is intersection point),
    * and if the area of the 3 Triangles is Equal to the main Triangle, then the point is inside the Triangle,
    * we will do the same to the other Triangle, and if one is true, then the point is in the quadrilateral.
    */
    const float fFirstMainTriangle = (TriangleArea(p1, p2, p3));
    const float fSecondMainTriangle = (TriangleArea(p1, p3, p4));
    if ((std::abs(fFirstMainTriangle - TriangleArea(p1, p2, vInterSectionPoint) + TriangleArea(p2, p3, vInterSectionPoint) + TriangleArea(p1, p3, vInterSectionPoint)) < 0.3f) || (std::abs(fSecondMainTriangle - TriangleArea(p1, p3, vInterSectionPoint) + TriangleArea(p3, p4, vInterSectionPoint) + TriangleArea(p1, p4, vInterSectionPoint)) < 0.3f))
    {
        if (distance != nullptr)
        {
            (*distance) = fInterSectionPoint;
            if (point != nullptr)
            {
                point->SetX(x);
                point->SetY(y);
                point->SetZ(z);
            }
            return (true);
        }
    }
    return (false);
}

/** SpherePlane - Checks for collision between a sphere and a Plane
 * defined by four points.
 *
 * @vSpherePos: The Sphere Position (will be Modified)
 * @vNormalPlane: The Plane Vector.
 * @p1: First point defining the Plane.
 * @p2: Second point defining the Plane.
 * @p3: Second point defining the Plane.
 * @p4: Second point defining the Plane.
 * @fRadius: Sphere Radius
*/
bool CCollision::SpherePlane(CVector3D& vSpherePos, const CVector3D vNormalPlane, const CVector3D p1, const CVector3D p2,
    const CVector3D p3, const CVector3D p4, const float fRadius)
{
    /* Distances from Sphere Center */
    float fFirstDist = 0.0, fSecondDist = 0.0;

    /* Check Intersection between two rays */
    if (RayPlane(vNormalPlane.GetX(), vNormalPlane.GetY(), vNormalPlane.GetZ(),
                p1.GetX(), p1.GetY(), p1.GetZ(),
                vSpherePos.GetX(), vSpherePos.GetY(), vSpherePos.GetZ(),
                -vNormalPlane.GetX(), -vNormalPlane.GetY(), -vNormalPlane.GetZ(),
                p1, p2, p3, p4, &fFirstDist)
                || RayPlane(-vNormalPlane.GetX(), -vNormalPlane.GetY(), -vNormalPlane.GetZ(),
                        p1.GetX(), p1.GetY(), p1.GetZ(),
                        vSpherePos.GetX(), vSpherePos.GetY(), vSpherePos.GetZ(),
                        vNormalPlane.GetX(), vNormalPlane.GetY(), vNormalPlane.GetZ(),
                        p1, p2, p3, p4, &fSecondDist))
    {
        if (fFirstDist > fRadius || fSecondDist > fRadius)
        {
            return (false);
        }

        /* if Distance is not Zero */
        if (fFirstDist > 0)
        {
            /* Calculate the point, and Calculate our new Location */
            vSpherePos.SetX((vSpherePos.GetX() + vNormalPlane.GetX()) * (fRadius - fFirstDist));
            vSpherePos.SetY((vSpherePos.GetY() + vNormalPlane.GetY()) * (fRadius - fFirstDist));
            vSpherePos.SetZ((vSpherePos.GetZ() + vNormalPlane.GetZ()) * (fRadius - fFirstDist));
        }
        /* else, means we are on the opposite side, then the normal vector*/
        else
        {
            vSpherePos.SetX((vSpherePos.GetX() - vNormalPlane.GetX()) * (fRadius - fSecondDist));
            vSpherePos.SetY((vSpherePos.GetY() - vNormalPlane.GetY()) * (fRadius - fSecondDist));
            vSpherePos.SetZ((vSpherePos.GetZ() - vNormalPlane.GetZ()) * (fRadius - fSecondDist));
        }
        /* we return true to indicate the collision, but the main thing is that we changed the Sphere Position to new Position */
        return (true);
    }
    return (false);
}

/** PointDistance - Gets the Distance between two points (with 3D Pythagoras Formula)
 *
 * @p1: First Point Coordinates
 * @p2: Second Point Cordinates
 *
 * Return: the Distance between the two Points.
*/
float CCollision::PointDistance(const CVector3D p1, const CVector3D p2)
{
    const CVector3D distVec(p2.GetX() - p1.GetX(), p2.GetY() - p1.GetY(), p2.GetZ() - p1.GetZ());
    const float xMul = distVec.GetX() * distVec.GetX();
    const float yMul = distVec.GetY() * distVec.GetY();
    const float zMul = distVec.GetZ() * distVec.GetZ();

    const float total = xMul + yMul + zMul;
    const float fDist = std::sqrt(total);
    return (fDist);
}

/** PointDistanceSquare - Gets the Square Distance between two points (with 3D Pythagoras Formula)
 *
 * @p1: First Point Coordinates
 * @p2: Second Point Cordinates
 *
 * Return: the Sqiare of Distance between the two Points.
*/
float CCollision::PointDistanceSquare(const CVector3D p1, const CVector3D p2)
{
    const CVector3D distVec(p2.GetX() - p1.GetX(), p2.GetY() - p1.GetY(), p2.GetZ() - p1.GetZ());
    const float xMul = distVec.GetX() * distVec.GetX();
    const float yMul = distVec.GetY() * distVec.GetY();
    const float zMul = distVec.GetZ() * distVec.GetZ();

    const float fDist = xMul + yMul + zMul;
    return (fDist);
}

/** SphereSphere - Check for Collision between two Spheres in 3D Space
 *
 * @p1: Center Point Coordinates of First Sphere
 * @fRadius1: First Sphere Radius
 * @p2: Center Point Coordinates of Second Sphere
 * @fRadius2: Second Sphere Radius
 *
 * Return: True if collision has occured,
 * and been resolved by Adjusting Position of first sphere, otherwise False.
*/
bool CCollision::SphereSphere(CVector3D& p1, const float fRadius1, const CVector3D& p2, const float fRadius2)
{
    const float fDist = PointDistanceSquare(p1, p2);

    if (fDist < ((fRadius1 + fRadius2) * (fRadius1 + fRadius2)))
    {
        const float a = std::sqrt(fDist) - (fRadius1 + fRadius2);
        CVector3D newVec(p2-p1);
        newVec.Normalize();
        p1 = p1 + newVec * a;
        return (true);
    }
    return (false);
}

/** RayPlaneDistance - Calcualtes and Returns the Distance of Plane Ray
 *
 * @normalVectorX: the X Orientation of the plane in the 3D Space.
 * @normalVectorY: the Y Orientation of the plane in the 3D Space.
 * @normalVectorZ: the Z Orientation of the plane in the 3D Space.
 *
 * @xPosition: the X coordinates of a point on the Plane, defines Plane Position in 3D Space.
 * @yPosition: the Y coordinates of a point on the Plane, defines Plane Position in 3D Space.
 * @zPosition: the Z coordinates of a point on the Plane, defines Plane Position in 3D Space.
 *
 * @xStartPoint: The coordinates of the starting point X of the Ray.
 * @yStartPoint: The coordinates of the starting point Y of the Ray.
 * @zStartPoint: The coordinates of the starting point Z of the Ray.
 *
 * @xDirection: the X of Direction Vector of the Ray, Defines the direction in which ray cast from start point.
 * @yDirection: the Y of Direction Vector of the Ray, Defines the direction in which ray cast from start point.
 * @zDirection: the Z of Direction Vector of the Ray, Defines the direction in which ray cast from start point.
 *
 * Return: The Distance of Plane Ray.
*/
float CCollision::RayPlaneDistance(const float normalVectorX, const float normalVectorY, const float normalVectorZ, const float xPosition,
                                   const float yPosition, const float zPosition, const float xStartPoint, const float yStartPoint,
                                   const float zStartPoint, const float xDirection, const float yDirection, const float zDirection)
{
    if ((normalVectorX*  xDirection + normalVectorY * yDirection + normalVectorZ * zDirection) == 0.0f)
    {
        return (0.0f);
    }

    const float fInterSectionPoint = (xPosition * normalVectorX + yPosition * normalVectorY + zPosition * normalVectorZ - normalVectorX * xStartPoint - normalVectorY * yStartPoint - normalVectorZ * zStartPoint) / (xDirection * normalVectorX + yDirection * normalVectorY + zDirection * normalVectorZ);

    if (fInterSectionPoint < 0.0f)
    {
        return (0.0f);
    }

    return (fInterSectionPoint);
}