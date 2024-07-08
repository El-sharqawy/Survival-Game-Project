//
// Created by osama on 03/07/24.
//

#include "SnowParticle.h"

#include <SDL_timer.h>
#include <GL/gl.h>

#include "../Debug/Utils.h"
#include <cmath>

#include "Random.h"

constexpr float c_fSnowDistance = 70000.0f;

std::vector<CSnowParticle*> CSnowParticle::ms_kVct_SnowParticePool;

SBlurVertex::SBlurVertex(const CVector3D pos, const float r, const uint32_t c, const float u, const float v)
{
	v3Pos = pos;
	rhw = r;
	color = c;
	tu = u;
	tv = v;
}

CSnowParticle::CSnowParticle()
{
	m_bActivate = false;
	m_bChangedSize = false;
	m_fHalfWidth = 0.0f;
	m_fHalfHeight = 0.0f;
	m_fPeriod = 0.0f;
	m_fcurRadian = 0.0f;
	m_fAmplitude = 0.0f;
}

CSnowParticle *CSnowParticle::New()
{
	if (ms_kVct_SnowParticePool.empty())
	{
		return new CSnowParticle;
	}

	CSnowParticle *pParticle = ms_kVct_SnowParticePool.back();
	ms_kVct_SnowParticePool.pop_back();
	return (pParticle);
}

void CSnowParticle::Delete(CSnowParticle *pSnowParticle)
{
	ms_kVct_SnowParticePool.emplace_back(pSnowParticle);
}

void CSnowParticle::DestroyPool()
{
	stl_wipe(ms_kVct_SnowParticePool);
}

void CSnowParticle::Init(const CVector3D &c_rv3Pos)
{
	float fRot = frandom(0.0f, 36000.0f) / 100.0f;
	float fDistance = frandom(0.0f, c_fSnowDistance)/ 10.0f;

	m_v3Position.SetX(c_rv3Pos.GetX() + fDistance * std::sin(DegreeToRadian(fRot)));
	m_v3Position.SetY(c_rv3Pos.GetY() + fDistance * std::cos(DegreeToRadian(fRot)));
	m_v3Position.SetX(c_rv3Pos.GetZ() + frandom(1500.0f, 2000.0f));

	m_v3Velocity = CVector3D(0.0f, 0.0f, frandom(-50.0f, -200.0f));
	m_fHalfWidth = frandom(2.0f, 7.0f);
	m_fHalfHeight = m_fHalfWidth;
	m_bActivate = true;
	m_bChangedSize = false;

	m_fPeriod = frandom(1.5f, 5.0f);
	m_fcurRadian = frandom(-1.6, 1.6f);
	m_fAmplitude = frandom(1.0f, 3.0f);
}

void CSnowParticle::SetCameraVertex(const CVector3D &rv3Up, const CVector3D &rv3Cross)
{
	m_v3Up = rv3Up * m_fHalfWidth;
	m_v3Cross = rv3Cross * m_fHalfHeight;
}

bool CSnowParticle::IsActivate() const
{
	return (m_bActivate);
}

void CSnowParticle::Update(const float fElapsedTime, const CVector3D &c_rv3Pos)
{
	m_v3Position += m_v3Velocity * fElapsedTime;

	m_v3Position.SetX(m_v3Cross.GetX() * std::sin(m_fcurRadian) / 10.0f);
	m_v3Position.SetY(m_v3Cross.GetY() * std::sin(m_fcurRadian) / 10.0f);

	m_fcurRadian += m_fPeriod * fElapsedTime;

	if ((m_v3Position.GetZ() < c_rv3Pos.GetZ() - 500.0f) ||
		(std::abs(m_v3Position.GetX() - c_rv3Pos.GetX()) > c_fSnowDistance) ||
		(std::abs(m_v3Position.GetY() - c_rv3Pos.GetY()) > c_fSnowDistance))
	{
		m_bActivate = false;
	}
}

void CSnowParticle::GetVertices(SParticleVertex &rv3Vertex1, SParticleVertex &rv3Vertex2, SParticleVertex &rv3Vertex3, SParticleVertex &rv3Vertex4)
{
	rv3Vertex1.v3Pos = m_v3Position - m_v3Cross - m_v3Up;
	rv3Vertex1.u = 0.0f;
	rv3Vertex1.v = 0.0f;

	rv3Vertex2.v3Pos = m_v3Position + m_v3Cross - m_v3Up;
	rv3Vertex2.u = 0.0f;
	rv3Vertex2.v = 0.0f;

	rv3Vertex3.v3Pos = m_v3Position - m_v3Cross + m_v3Up;
	rv3Vertex3.u = 0.0f;
	rv3Vertex3.v = 0.0f;

	rv3Vertex4.v3Pos = m_v3Position + m_v3Cross + m_v3Up;
	rv3Vertex4.u = 0.0f;
	rv3Vertex4.v = 0.0f;
}
