//
// Created by osama on 14/07/24.
//

#include "Menu.h"

#include <GL/gl.h>
#include <SDL2/SDL.h>

#include "../game.h"

CMenu::CMenu(const uint32_t uiBack, const std::vector<CCollisionPlane> &colPlanes, const std::vector<std::string> &stText,
             const CCollisionPlane& backPos, CText *text, SDL_Window* hWindow)
{
	m_uiBackGround = uiBack;
	m_vColPlanes = colPlanes;
	m_sStrings = stText;
	m_cBackPos = new CCollisionPlane(backPos);
	m_tText = text;
	m_iCurSelection = 0;
	m_hWindow = hWindow;
}

CMenu::~CMenu()
{
	Destroy();
}

void CMenu::Destroy()
{
	delete m_cBackPos;
	m_cBackPos = nullptr;
}
int32_t CMenu::Show()
{
	const bool bIsLighting = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	m_iCurSelection = 0;
	SDL_Event event;

	while (true)
	{
		const uint32_t start = SDL_GetTicks();
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT);
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
						{
							if (bIsLighting)
							{
								glEnable(GL_LIGHTING);
							}
							glColor3f(1.0f, 1.0f, 1.0f);
							return (1);
						}
						case SDLK_UP:
						{
							m_iCurSelection--;
							if (m_iCurSelection < 0)
							{
								m_iCurSelection = static_cast<int32_t>(m_vColPlanes.size() - 1);
							}
						}
						break;
						case SDLK_DOWN:
						{
							m_iCurSelection++;
							if (m_iCurSelection > static_cast<int32_t>(m_vColPlanes.size() - 1))
							{
								m_iCurSelection = 0;
							}
						}
						break;
						case SDLK_RETURN:
						{
							if (bIsLighting)
							{
								glEnable(GL_LIGHTING);
							}
							glColor3f(1.0f, 1.0f, 1.0f);
							return (m_iCurSelection);
						}
						default:
							break;
					}
					default:
						break;
				}
			}
		}
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_uiBackGround);
		glColor3f(1.0f, 1.0f, 1.0f);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(m_cBackPos->GetFirstVectorPlane().GetX(), m_cBackPos->GetFirstVectorPlane().GetY(), m_cBackPos->GetFirstVectorPlane().GetZ());
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(m_cBackPos->GetSecondVectorPlane().GetX(), m_cBackPos->GetSecondVectorPlane().GetY(), m_cBackPos->GetSecondVectorPlane().GetZ());
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(m_cBackPos->GetThirdVectorPlane().GetX(), m_cBackPos->GetThirdVectorPlane().GetY(), m_cBackPos->GetThirdVectorPlane().GetZ());
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(m_cBackPos->GetFourthVectorPlane().GetX(), m_cBackPos->GetFourthVectorPlane().GetY(), m_cBackPos->GetFourthVectorPlane().GetZ());
		glEnd();

		glDisable(GL_TEXTURE_2D);

		for(int32_t i = 0; i < m_vColPlanes.size(); i++)
		{
			if (i == m_iCurSelection)
			{
				glBegin(GL_QUADS);
					glColor3f(1.0f, 0.0f, 0.0f);
					glVertex3f(m_vColPlanes[i].GetFirstVectorPlane().GetX(), m_vColPlanes[i].GetFirstVectorPlane().GetY() + 0.032f,
						m_vColPlanes[i].GetFirstVectorPlane().GetZ());

					glColor3f(0.0f, 0.0f, 0.0f);
					glVertex3f(m_vColPlanes[i].GetFirstVectorPlane().GetX(), m_vColPlanes[i].GetFirstVectorPlane().GetY(), m_vColPlanes[i].GetFirstVectorPlane().GetZ());

					glVertex3f(m_vColPlanes[i].GetFirstVectorPlane().GetX() + (static_cast<float>(m_sStrings[i].length()) * 0.032f),
						m_vColPlanes[i].GetFirstVectorPlane().GetY(), m_vColPlanes[i].GetFirstVectorPlane().GetZ());

					glColor3f(1.0f, 0.0f, 0.0f);
					glVertex3f(m_vColPlanes[i].GetFirstVectorPlane().GetX() + static_cast<float>(m_sStrings[i].length()) * 0.032f,
						m_vColPlanes[i].GetFirstVectorPlane().GetY() + 0.032f, m_vColPlanes[i].GetFirstVectorPlane().GetZ());
				glEnd();
			}
			glColor3f(0.0f, 0.0f, 0.0f);
			m_tText->RenderText(m_vColPlanes[i].GetFirstVectorPlane(),CVector3D(0,0,0),CVector3D(0.030,0.030,0.030),m_sStrings[i].c_str());
		}

		m_tText->RenderText(CVector3D(-0.20f,0.32f,-1.0f),CVector3D(0,0,0),CVector3D(0.030,0.030,0.030),"Survival Game Project");
		SDL_GL_SwapWindow(m_hWindow);

		if(1000 / GAME_FPS > (SDL_GetTicks() - start))
			SDL_Delay(1000 / GAME_FPS - (SDL_GetTicks() - start));
	}
	return 0;
}
