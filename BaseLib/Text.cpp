//
// Created by osama on 05/07/24.
//

#include "Text.h"

#include <cstring>
#include <GL/gl.h>

CText::CText(const std::vector<uint32_t> &vCharacters, const float fWidth, const float fHeight)
{
	m_vCharacters = vCharacters;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_bIsLocked = false;
}

void CText::AssignFont(const std::vector<uint32_t> &vCharacters, float fWidth, float fHeight)
{
	m_vCharacters = vCharacters;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}

void CText::RenderText(const CVector3D v3Pos, const CVector3D v3Rot, const CVector3D v3Scale, const char *c_szText, const CVector3D v3Color)
{
	const bool bIsTexture2DEnabled = glIsEnabled(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_2D);
	glColor3f(v3Color.GetX(), v3Color.GetY(), v3Color.GetZ());
	glPushMatrix();
	int32_t g = 0;

	if (!m_bIsLocked)
	{
		m_v3Pos = v3Pos;
		m_v3Rot = v3Rot;
		m_v3Scale = v3Scale;
		m_bIsLocked = true;
	}

	glTranslatef(v3Pos.GetX(), v3Pos.GetY(), v3Pos.GetZ());
	glRotatef(v3Rot.GetX(), 1.0f, 0.0f, 0.0f);
	glRotatef(v3Rot.GetY(), 0.0f, 1.0f, 0.0f);
	glRotatef(v3Rot.GetZ(), 0.0f, 0.0f, 1.0f);

	for (int32_t i = 0; i < strlen(c_szText); i++)
	{
		if (c_szText[i] == '\n')
		{
			glTranslatef(static_cast<float>(-g) * m_fWidth, -m_fHeight, 0.0f);
			g = 0;
			continue;
		}

		if (c_szText[i] == ' ')
		{
			glTranslatef(m_fWidth * m_v3Scale.GetX(), 0.0f, 0.0f);
			g++;
			continue;
		}

		if (static_cast<int32_t>(c_szText[i]) < 33 || static_cast<int32_t>(c_szText[i]) > 126)
		{
			continue;
		}

		glPushMatrix();
		glScalef(v3Scale.GetX(), v3Scale.GetY(), v3Scale.GetZ());
		glCallList(m_vCharacters.at(c_szText[i] - 33));
		g++;
		glPopMatrix();
		glTranslatef(m_fWidth * m_v3Scale.GetX(), 0.0f, 0.0f);
	}
	glPopMatrix();
	if (bIsTexture2DEnabled)
	{
		glEnable(GL_TEXTURE_2D);
	}
}
