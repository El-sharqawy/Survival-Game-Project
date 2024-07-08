//
// Created by osama on 03/07/24.
//

#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vector3D.h"
#include <cstdint>
#include <vector>

class CParticle
{
public:
	CParticle(CVector3D v3Pos, CVector3D v3Velocity, int32_t iLife);
	void Move();
	void Show() const;
	[[nodiscard]]bool IsDead() const;

private:
	CVector3D m_v3Pos;
	CVector3D m_v3Dir;
	CVector3D m_v3Clone;
	uint32_t m_uiStartTime;
	uint32_t m_uiEndTime;
};

class CParticleSystem
{
public:
	CParticleSystem(int32_t iPartNum, CVector3D pos);
	~CParticleSystem();
	void Refresh();
	void Destroy();

private:
	std::vector<CParticle *> m_vParticles;
	CVector3D m_v3Pos;
	int32_t m_iParticlesNum;
};

#endif //PARTICLE_H
