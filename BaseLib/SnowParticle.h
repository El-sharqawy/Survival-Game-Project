//
// Created by osama on 03/07/24.
//

#ifndef SNOW_PARTICLE_H
#define SNOW_PARTICLE_H

#include "Vector3D.h"
#include <cstdint>
#include <vector>

struct SParticleVertex
{
	CVector3D v3Pos;
	float u, v;
};

struct SBlurVertex
{
	CVector3D v3Pos;
	float rhw;		// Reciprocal of homogeneous W
	uint32_t color;
	float tu, tv;	// Texture coordinates (u, v)

	static const uint32_t FVF;	// Flexible vertex format

	SBlurVertex(CVector3D pos, float r, uint32_t c, float u, float v);
	~SBlurVertex() = default;
};

class CSnowParticle
{
public:
	CSnowParticle();
	~CSnowParticle() = default;

	static CSnowParticle * New();
	static void Delete(CSnowParticle * pSnowParticle);
	static void DestroyPool();

	void Init(const CVector3D& c_rv3Pos);
	void SetCameraVertex(const CVector3D& rv3Up, const CVector3D& rv3Cross);
	[[nodiscard]]bool IsActivate() const;

	void Update(float fElapsedTime, const CVector3D& c_rv3Pos);
	void GetVertices(SParticleVertex& rv3Vertex1, SParticleVertex& rv3Vertex2,
					 SParticleVertex& rv3Vertex3, SParticleVertex& rv3Vertex4);

protected:
	bool m_bActivate;
	bool m_bChangedSize;
	float m_fHalfWidth;
	float m_fHalfHeight;

	CVector3D m_v3Velocity;
	CVector3D m_v3Position;

	CVector3D m_v3Up;
	CVector3D m_v3Cross;

	float m_fPeriod;
	float m_fcurRadian;
	float m_fAmplitude;

public:
	static std::vector<CSnowParticle *> ms_kVct_SnowParticePool;
};

#endif //SNOW_PARTICLE_H
