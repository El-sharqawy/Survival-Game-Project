//
// Created by osama on 02/07/24.
//

#include "Zombie.h"
#include <GL/gl.h>
#include <complex>
#include <SDL_timer.h>

std::vector<uint32_t> CZombie::m_vFrames;
uint32_t CZombie::m_uiWalk = 0;
uint32_t CZombie::m_uiAttack = 0;
uint32_t CZombie::m_uiDie = 0;

CZombie::CZombie(const std::vector<uint32_t>& vAnimFrames, const uint32_t &uiWalk, const uint32_t &uiAttack, const uint32_t &uiDie,
		const int32_t iHealth, const int32_t iStrength, const float fSpeed, const CCollisionSphere ZCollSphere,
		const CVector3D v3Rotation, const CVector3D v3PlayerLoc)
{
	m_vFrames = vAnimFrames;
	m_uiWalk = uiWalk;
	m_uiAttack = uiAttack,
	m_uiDie = uiDie;
	m_iHealth = iHealth;
	m_iStrength = iStrength;
	m_fSpeed = fSpeed;
	m_csZombieSphere = ZCollSphere;

	m_bIsWalking = true;
	m_bIsAttacking = false;
	m_bIsDead = false;
	m_uiCurFrame = 0;
	m_v3Rotation = v3Rotation;
	m_v3Position.Change(v3PlayerLoc - m_csZombieSphere.GetCenter() + CVector3D(50.f, 5.0f, 5.0f));
	m_v3Position.Normalize();
	m_iLastAttackTime = SDL_GetTicks() + 3000;
	m_v3StartPos = m_v3Position;
}

CZombie::CZombie(const int32_t iHealth, const float fSpeed, const int32_t iStrength, const CCollisionSphere colSphere,
	const CVector3D Rot, const CVector3D playerLoc)
{
	m_iHealth = iHealth;
	m_fSpeed = fSpeed;
	m_iStrength = iStrength;
	m_bIsAttacking = false;
	m_bIsWalking = true;
	m_bIsDead = false;
	m_csZombieSphere = colSphere;
	m_v3Rotation = Rot;
	m_v3Position.Change(playerLoc - m_csZombieSphere.GetCenter());
	m_v3Position.Normalize();
	m_uiCurFrame = 0;
	m_iLastAttackTime = SDL_GetTicks();
	m_v3StartPos = m_v3Position;
}

bool CZombie::Update(const std::vector<CCollisionPlane>& vColPlanes, const CVector3D playerLocation)
{
	if (GetHealth() <= 0)
	{
		SetIsDead(true);
		SetIsWalking(false);
		SetIsAttacking(false);
		m_uiCurFrame = m_uiWalk + m_uiAttack;
		return (true);
	}

	if (!IsDead())
	{
		m_v3Position.Change(playerLocation-GetCollisionSphere()->GetCenter());
		m_v3Position.Normalize();

		m_v3Rotation.SetY(std::acos(m_v3Position.GetZ()));
		if (m_v3Position.GetX() > 0)
		{
			m_v3Rotation.SetY(-m_v3Rotation.GetY());
		}
	}

	m_uiCurFrame++;
	if(IsWalking() && m_uiCurFrame > m_uiWalk)
	{
		m_uiCurFrame = 0;
	}

	if (IsAttacking() && m_uiCurFrame > m_uiWalk + m_uiAttack)
	{
		m_uiCurFrame = m_uiWalk;
	}

	if (IsDead() && m_uiCurFrame > m_uiWalk + m_uiAttack + m_uiDie)
	{
		m_uiCurFrame = m_uiWalk + m_uiAttack + m_uiDie - 1;
	}

	if (!IsDead())
	{

		CVector3D newPos(GetCollisionSphere()->GetCenter());
		newPos += m_v3Position * GetSpeed();
		if (newPos.GetY() >= -3.0)
		{
			newPos.SetY(newPos.GetY() - 0.3f);
		}

		for (auto & colPlane : vColPlanes)
		{
			SpherePlane(newPos, colPlane.GetVectorNormal(), colPlane.GetFirstVectorPlane(), colPlane.GetSecondVectorPlane(),
				colPlane.GetThirdVectorPlane(), colPlane.GetFourthVectorPlane(), GetCollisionSphere()->GetRadius());
		}
		SetPosition(newPos);
	}
	return (false);
}

void CZombie::Show()
{
	glEnable(GL_NORMALIZE);
	glPushMatrix();
		glTranslatef(GetCollisionSphere()->GetCenter().GetX(), GetCollisionSphere()->GetCenter().GetY() - 0.1f, GetCollisionSphere()->GetCenter().GetZ());
		glRotatef(GetRotation().GetX(), 1.0, 0.0, 0.0);
		glRotatef(static_cast<float>(-GetRotation().GetY() * (180/M_PI)), 0.0, 1.0, 0.0);
		glRotatef(GetRotation().GetZ(), 0.0, 0.0, 1.0);
		glScalef(3.5, 3.5, 3.5);
		glCallList(m_vFrames[m_uiCurFrame]);
	glPopMatrix();
	glDisable(GL_NORMALIZE);
}

bool CZombie::SetAttack(CCollisionSphere playerSphere)
{
	const bool bCanAttack = SDL_TICKS_PASSED(SDL_GetTicks() , m_iLastAttackTime);

	if (SphereSphere(GetCollisionSphere()->GetCenter(), GetCollisionSphere()->GetRadius(),
		playerSphere.GetCenter(), playerSphere.GetRadius()) && !IsDead() && bCanAttack)
	{
		m_iLastAttackTime = SDL_GetTicks() + 3000;
		SetIsAttacking(true);
		SetIsWalking(false);
		return(true);
	}
	else if (!IsDead())
	{
		SetIsWalking(true);
		SetIsAttacking(false);
		return (false);
	}
	return (false);
}

CCollisionSphere *CZombie::GetCollisionSphere()
{
	return (&m_csZombieSphere);
}

void CZombie::SetPosition(const CVector3D &newLocation)
{
	m_csZombieSphere.SetCenter(newLocation);
}

void CZombie::SetHealth(const int32_t iHealth)
{
	m_iHealth = iHealth;
}

int32_t CZombie::GetHealth() const
{
	return (m_iHealth);
}

int32_t CZombie::GetStrength() const
{
	return (m_iStrength);
}

bool CZombie::IsDead() const
{
	return (m_bIsDead);
}

bool CZombie::IsWalking() const
{
	return (m_bIsWalking);
}

bool CZombie::IsAttacking() const
{
	return (m_bIsAttacking);
}

float CZombie::GetSpeed() const
{
	return (m_fSpeed);
}

void CZombie::SetIsWalking(const bool bIsWalking)
{
	m_bIsWalking = bIsWalking;
}

void CZombie::SetIsAttacking(const bool bIsAttacking)
{
	m_bIsAttacking = bIsAttacking;
}

void CZombie::SetIsDead(const bool bIsDead)
{
	m_bIsDead = bIsDead;
}

CVector3D CZombie::GetPosition() const
{
	return (m_v3Position);
}

CVector3D CZombie::GetRotation() const
{
	return (m_v3Rotation);
}

void CZombie::TakeDamage(const int32_t iDam)
{
	m_iHealth -= iDam;
}

CVector3D CZombie::GetStartPos() const
{
	return (m_v3StartPos);
}
