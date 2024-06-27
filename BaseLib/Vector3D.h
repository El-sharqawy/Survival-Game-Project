//
// Created by osama on 09/05/24.
//

#ifndef VECTOR3D_H
#define VECTOR3D_H

class CVector3D
{
public:
    CVector3D();
    CVector3D(float a, float b);
    CVector3D(float a, float b, float c);
    ~CVector3D() = default;

    [[nodiscard]] float dotProduct(const CVector3D& vec) const;

    static CVector3D crossProduct(const CVector3D& vec);
    [[nodiscard]] float GetLength() const;
    void Normalize();

    void Change(float a, float b, float c);
    void Change(CVector3D vec2);

    void SetX(float a);
    void SetY(float b);
    void SetZ(float c);

    CVector3D operator + (const CVector3D& vec2) const;
    CVector3D operator - (const CVector3D& vec2) const;
    CVector3D operator * (float num) const;
    CVector3D operator / (float num) const;

    CVector3D& operator += (const CVector3D& vec2);
    CVector3D& operator -= (const CVector3D& vec2);
    CVector3D& operator *= (float num);
    CVector3D& operator /= (float num);

    bool operator == (const CVector3D& vec2) const;
    bool operator != (const CVector3D& vec2) const;

public:
    [[nodiscard]]float GetX() const { return x; }
    [[nodiscard]]float GetY() const { return y; }
    [[nodiscard]]float GetZ() const { return z; }

private:
    float x, y, z;
}; // CVector3D

#endif //VECTOR3D_H
