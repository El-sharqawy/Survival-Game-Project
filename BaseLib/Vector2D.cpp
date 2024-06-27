//
// Created by osama on 24/05/24.
//

#include "Vector2D.h"
#include <cmath>

CVector2D::CVector2D()
{
	x = y = 0;
}

CVector2D::CVector2D(const float a)
{
	x = a;
	y = 0;
}

CVector2D::CVector2D(const float a, const float b)
{
	x = a;
	y = b;
}

float CVector2D::dotProduct(const CVector2D &vec) const
{
	return (x * vec.x + y * vec.y);
}

/* Later Implementation */
CVector2D CVector2D::crossProduct(const CVector2D & vec)
{
	const CVector2D newVec2D(vec.x, vec.y);
	return (newVec2D);
}

float CVector2D::GetLength() const
{
	const float square = std::sqrt(x*x + y*y);
	return (square);
}

void CVector2D::Normalize()
{
	if (GetLength() != 0.0f)
	{
		x /= GetLength();
		y /= GetLength();
	}
}

void CVector2D::Change(const float a, const float b)
{
	x = a;
	y = b;
}

void CVector2D::Change(const CVector2D vec2)
{
	x = vec2.x;
	y = vec2.y;
}

void CVector2D::SetX(const float a)
{
	x = a;
}

void CVector2D::SetY(const float b)
{
	y = b;
}

CVector2D CVector2D::operator + (const CVector2D& vec2) const
{
	const CVector2D newVec(x + vec2.x, y + vec2.y);
	return (newVec);
}

CVector2D CVector2D::operator - (const CVector2D& vec2) const
{
	const CVector2D newVec(x - vec2.x, y - vec2.y);
	return (newVec);
}

CVector2D CVector2D::operator * (const float num) const
{
	const CVector2D newVec2D(x * num, y * num);
	return (newVec2D);

}

CVector2D CVector2D::operator / (const float num) const
{
	if (num != 0)
	{
		const CVector2D newVec2D(x * num, y * num);
		return (newVec2D);
	}
	else
	{
		const CVector2D newVec2D(0.0f, 0.0f);
		return (newVec2D);
	}
}

CVector2D& CVector2D::operator += (const CVector2D& vec2)
{
	x += vec2.x;
	y += vec2.y;
	return (*this);
}

CVector2D& CVector2D::operator -= (const CVector2D& vec2)
{
	x -= vec2.x;
	y -= vec2.y;
	return (*this);
}

CVector2D& CVector2D::operator *= (const float num)
{
	x *= num;
	y *= num;
	return (*this);
}

CVector2D& CVector2D::operator /= (const float num)
{
	if (num != 0.0f)
	{
		x /= num;
		y /= num;
	}
	return (*this);
}

bool CVector2D::operator == (const CVector2D& vec2) const
{
	return (x == vec2.x && y == vec2.y);
}

bool CVector2D::operator != (const CVector2D& vec2) const
{
	return (!(*this == vec2));
}

