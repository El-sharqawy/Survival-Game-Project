//
// Created by osama on 24/05/24.
//

#ifndef VECTOR2D_H
#define VECTOR2D_H



class CVector2D
{
public:
	CVector2D();
	CVector2D(float a);
	CVector2D(float a, float b);
	~CVector2D() = default;

	[[nodiscard]] float dotProduct(const CVector2D& vec) const;

	static CVector2D crossProduct(const CVector2D& vec);
	[[nodiscard]] float GetLength() const;
	void Normalize();

	void Change(float a, float b);
	void Change(CVector2D vec2);

	void SetX(float a);
	void SetY(float b);

	CVector2D operator + (const CVector2D& vec2) const;
	CVector2D operator - (const CVector2D& vec2) const;
	CVector2D operator * (float num) const;
	CVector2D operator / (float num) const;

	CVector2D& operator += (const CVector2D& vec2);
	CVector2D& operator -= (const CVector2D& vec2);
	CVector2D& operator *= (float num);
	CVector2D& operator /= (float num);

	bool operator == (const CVector2D& vec2) const;
	bool operator != (const CVector2D& vec2) const;

public:
	[[nodiscard]]float GetX() const { return x; }
	[[nodiscard]]float GetY() const { return y; }

private:
	float x, y;
};



#endif //VECTOR2D_H
