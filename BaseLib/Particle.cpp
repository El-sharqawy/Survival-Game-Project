//
// Created by osama on 03/07/24.
//

#include "Particle.h"

#include <SDL_timer.h>
#include <GL/gl.h>

CParticle::CParticle(const CVector3D v3Pos, const CVector3D v3Velocity, int32_t iLife)
{
	m_v3Pos = v3Pos;
	m_v3Dir = v3Velocity;
	m_uiStartTime = SDL_GetTicks();
	m_uiEndTime = SDL_GetTicks() + iLife;
}

void CParticle::Move()
{
	m_v3Pos += m_v3Dir;
}

void CParticle::Show() const
{
	glVertex3f(m_v3Pos.GetX(), m_v3Pos.GetY(), m_v3Pos.GetZ());
}

bool CParticle::IsDead() const
{
	return (SDL_GetTicks() >= m_uiEndTime);
}

CParticleSystem::CParticleSystem(const int32_t iPartNum, CVector3D pos)
{
	m_iParticlesNum = iPartNum;
	m_v3Pos = pos;

	for (int32_t i = 0; i < iPartNum; i++)
	{
		m_vParticles.emplace_back(new CParticle(m_v3Pos, CVector3D(rand()%10+(float)rand()/(float)RAND_MAX-5, rand()%10+(float)rand()/(float)RAND_MAX-5, rand()%10+(float)rand()/(float)RAND_MAX-5), 50+(rand()%70)));
	}
}

CParticleSystem::~CParticleSystem()
{
	Destroy();
}

void CParticleSystem::Refresh()
{
	bool bIsLighting = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glBegin(GL_POINTS);

	for (int32_t i = 0; i < m_iParticlesNum; i++)
	{
		if (m_vParticles.at(i)->IsDead())
		{
			delete m_vParticles.at(i);
			m_vParticles.at(i) = new CParticle(CVector3D(m_v3Pos.GetX() + (rand()%6-3)/100.0, m_v3Pos.GetY() + (rand()%6-3)/100.0, m_v3Pos.GetZ() + (rand()%6-3)/100.0),
				CVector3D((rand()%10+(float)rand()/(float)RAND_MAX-5)/500.0,(rand()%10+(float)rand()/(float)RAND_MAX-3)/400.0,(rand()%10+(float)rand()/(float)RAND_MAX-5)/500.0), 30+(rand()%7));
		}
		else
		{
			m_vParticles.at(i)->Move();
			m_vParticles.at(i)->Show();
		}
	}
	glEnd();
	if (bIsLighting)
	{
		glEnable(GL_LIGHTING);
	}
	glDisable(GL_BLEND);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void CParticleSystem::Destroy()
{
	for (auto & particle : m_vParticles)
	{
		delete particle;
	}
}
