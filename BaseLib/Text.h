//
// Created by osama on 05/07/24.
//

#ifndef TEXT_H
#define TEXT_H

#include <vector>
#include <cstdint>

#include "Vector3D.h"

class CText
{
public:
	CText(const std::vector<uint32_t>& vCharacters, float fWidth, float fHeight);
	~CText() = default;

	void AssignFont(const std::vector<uint32_t>& vCharacters, float fWidth, float fHeight);
	void RenderText(CVector3D v3Pos, CVector3D v3Rot, CVector3D v3Scale, const char * c_szText, CVector3D v3Color = CVector3D(1.0f, 1.0f, 1.0f));

private:
	std::vector<uint32_t> m_vCharacters;
	float m_fWidth;
	float m_fHeight;
	bool m_bIsLocked;
	CVector3D m_v3Pos;
	CVector3D m_v3Rot;
	CVector3D m_v3Scale;
};



#endif //TEXT_H
