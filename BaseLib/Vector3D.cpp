#include "Vector3D.h"
#include <cmath>

CVector3D::CVector3D()
{
    x = y = z = 0;
}

CVector3D::CVector3D(const float a, const float b)
{
    x = a;
    y = b;
    z = 0.0f;
}

CVector3D::CVector3D(const float a, const float b, const float c)
{
    x = a;
    y = b;
    z = c;
}

float CVector3D::dotProduct(const CVector3D &vec) const
{
    return (x * vec.x + y * vec.y + z * vec.z);
}

/* Later Implementation */
CVector3D CVector3D::crossProduct(const CVector3D & vec)
{
    const CVector3D newVec(vec.x, vec.y, vec.z);
    return (newVec);
}

float CVector3D::GetLength() const
{
    const float square = std::sqrt(x*x + y*y + z*z);
    return (square);
}

void CVector3D::Normalize()
{
    if (GetLength() != 0.0f)
    {
        x /= GetLength();
        y /= GetLength();
        z /= GetLength();
    }
}

void CVector3D::Change(const float a, const float b, const float c)
{
    x = a;
    y = b;
    z = c;
}

void CVector3D::Change(const CVector3D vec2)
{
    x = vec2.x;
    y = vec2.y;
    z = vec2.z;
}

void CVector3D::SetX(const float a)
{
    x = a;
}

void CVector3D::SetY(const float b)
{
    y = b;
}

void CVector3D::SetZ(const float c)
{
    z = c;
}


CVector3D CVector3D::operator + (const CVector3D& vec2) const
{
    const CVector3D newVec(x + vec2.x, y + vec2.y, z + vec2.z);
    return (newVec);
}

CVector3D CVector3D::operator - (const CVector3D& vec2) const
{
    const CVector3D newVec(x - vec2.x, y - vec2.y, z - vec2.z);
    return (newVec);
}

CVector3D CVector3D::operator * (const float num) const
{
    const CVector3D newVec(x * num, y * num, z * num);
    return (newVec);

}

CVector3D CVector3D::operator / (const float num) const
{
    if (num != 0)
    {
        const CVector3D newVec(x * num, y * num, z * num);
        return (newVec);
    }
    else
    {
        const CVector3D newVec(0.0f, 0.0f, 0.0f);
        return (newVec);
    }
}

CVector3D& CVector3D::operator += (const CVector3D& vec2)
{
    x += vec2.x;
    y += vec2.y;
    z += vec2.z;
    return (*this);
}

CVector3D& CVector3D::operator -= (const CVector3D& vec2)
{
    x -= vec2.x;
    y -= vec2.y;
    z -= vec2.z;
    return (*this);
}

CVector3D& CVector3D::operator *= (const float num)
{
    x *= num;
    y *= num;
    z *= num;
    return (*this);
}

CVector3D& CVector3D::operator /= (const float num)
{
    if (num != 0.0f)
    {
        x /= num;
        y /= num;
        z /= num;
    }
    return (*this);
}

bool CVector3D::operator == (const CVector3D& vec2) const
{
    return (x == vec2.x && y == vec2.y && z == vec2.z);
}

bool CVector3D::operator != (const CVector3D& vec2) const
{
    return (!(*this == vec2));
}
