//
// Created by osama on 14/07/24.
//

#include "Knife.h"

CKnife::CKnife(const std::vector<uint32_t> &Frames, const int32_t iStr, const int32_t iDel, const int32_t iWepDelay,
	const CVector3D v3Pos, Mix_Chunk *Hit, Mix_Chunk *Norm)
{
	m_vFrames = Frames;
	m_iStrength = iStr;
	m_iDelay = iDel;
	m_iWeaponDelay = iWepDelay;
	m_v3Position = v3Pos;
	m_HitSound = Hit;
	m_NormalSound = Norm;

	m_bIsKnifing = false;
	m_iLastTimeHit = 100;
	m_iCurFrame = 0;
}

int32_t CKnife::SetKnife(CPlayer *pPlayer, CZombie* pZombie)
{
	if (m_iLastTimeHit > m_iDelay)
	{
		pPlayer->SetHavingWeapon(false);
		m_iLastTimeHit = 0;
		m_bIsKnifing = true;
		m_iCurFrame = 0;
		const CVector3D v3Dir = pPlayer->GetPlayerCamera()->GetVector();
		float fDist;

		if (CCollision::RaySphere(pZombie->GetCollisionSphere()->GetCenter().GetX(), pZombie->GetCollisionSphere()->GetCenter().GetY(), pZombie->GetCollisionSphere()->GetCenter().GetZ(),
		v3Dir.GetX(), v3Dir.GetY(), v3Dir.GetZ(), pPlayer->GetPlayerCamera()->GetCameraLocation().GetX(), pPlayer->GetPlayerCamera()->GetCameraLocation().GetY(),
		pPlayer->GetPlayerCamera()->GetCameraLocation().GetZ(), pZombie->GetCollisionSphere()->GetRadius(), &fDist))
		{
			if (fDist < 9.0 && pZombie->IsDead() == false)
			{
				Mix_PlayChannel(-1, m_HitSound, 0);
				return (1);
			}
		}
		Mix_PlayChannel(-1, m_NormalSound, 0);
		return (-1);
	}
	return (-1);
}

void CKnife::Update(CPlayer *pPlayer)
{
	m_iLastTimeHit++;
	if (m_iLastTimeHit > m_iWeaponDelay)
	{
		pPlayer->SetHavingWeapon(true);
	}

	if (m_bIsKnifing)
	{
		m_iCurFrame++;
		if (m_iCurFrame >= m_vFrames.size())
		{
			m_bIsKnifing = false;
			m_iCurFrame = 0;
		}
	}
}

void CKnife::Show() const
{
	if (m_bIsKnifing)
	{
		glTranslatef(m_v3Position.GetX(), m_v3Position.GetY(), m_v3Position.GetZ());
		glCallList(m_vFrames[m_iCurFrame]);
	}
}

int32_t CKnife::GetStrength() const
{
	return (m_iStrength);
}

bool CKnife::IsKnifing() const
{
	return (m_bIsKnifing);
}


