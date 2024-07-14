//
// Created by osama on 30/06/24.
//

#include "Weapon.h"

#include "../Debug/Log.h"

CWeapon::CWeapon(std::vector<uint32_t>& vAnimFrames,uint32_t uiNormAnim,uint32_t uiFireAnim, uint32_t uiReloadAnim,
                 Mix_Chunk *fireSound,Mix_Chunk* reloadSound,Mix_Chunk* normalSound,Mix_Chunk* emptySound,CVector3D v3Pos,CVector3D v3Rot,CVector3D v3Apos,CVector3D v3Arot,
                 float fPrec,float fAprec, uint32_t uiBulletsNum, float Length, uint32_t iSpeed,bool bIsAuto, int32_t iStr, const char* c_szName, uint32_t uiOutView)
{
	m_vFrames = vAnimFrames;
	m_uiOuterView = uiOutView;
	m_uiNormalAnimation = uiNormAnim;
	m_uiFireAnimation = uiFireAnim;
	m_uiReloadAnimation = uiReloadAnim;

	m_v3Pos = v3Pos;
	m_v3Rot = v3Rot;
	m_v3AimPos = v3Apos;
	m_v3AimRot = v3Arot;

	m_fPrecision = (fPrec != 0 ? fPrec : static_cast<float>(0.000001));
	m_fAimPrecision = (fAprec != 0 ? fAprec : static_cast<float>(0.000001));

	m_iStrength = iStr;
	m_uiMaxMagCapacity = uiBulletsNum;
	m_uiSpeed = iSpeed;

	m_stName = std::string(c_szName);

	m_v3ExpectedPos = m_v3Pos;
	m_v3ExpectedRot = m_v3Rot;

	m_v3CurPos = m_v3Pos;
	m_v3CurRot = m_v3Rot;

	m_bIsAiming = false;
	m_bIsReloading = false;
	m_bIsAutomatic = bIsAuto;
	m_bIsFiring = false;
	m_bIsTest = true;
	m_uiCurBullets = m_uiMaxMagCapacity;
	m_uiAllBullets = 300;

	m_uiLastShot = 1000;
	m_uiCurFrame = 0;
	m_uiCurMode = 1;
	srand(time(nullptr));

	m_sFireSound = fireSound;
	m_sReloadSound = reloadSound;
	m_sNormalSound = normalSound;
	m_sEmptySound = emptySound;
}

void CWeapon::Update()
{
	m_uiLastShot++;
	m_uiCurFrame++;
	if (m_uiCurMode == 1) /*** Normal ***/
	{
		if (m_uiCurFrame >= m_uiNormalAnimation)
		{
			m_uiCurFrame = 0;
		}
	}
	else if (m_uiCurMode == 2)
	{
		if (m_uiCurFrame >= m_uiNormalAnimation + m_uiFireAnimation)
		{
			if (m_bIsAutomatic && m_bIsFiring) /*** Need to Check ***/
			{
				m_uiCurFrame = m_uiNormalAnimation;
			}
			else
			{
				m_uiCurMode = 1;
				m_uiCurFrame = 0;
			}
		}
	}
	else if (m_uiCurMode == 3)
	{
		if (m_uiCurFrame >= m_uiNormalAnimation + m_uiFireAnimation + m_uiReloadAnimation)
		{
			m_uiCurMode = 1;
			m_uiCurFrame =0;
			m_bIsReloading = false;
		}
	}

	CVector3D vPosTemp(m_v3ExpectedPos - m_v3CurPos);
	vPosTemp.Normalize();
	vPosTemp *= 0.3;

	if (std::abs(m_v3ExpectedPos.GetX() - m_v3CurPos.GetX()) < 0.3 &&
		std::abs(m_v3ExpectedPos.GetY() - m_v3CurPos.GetY()) < 0.3 &&
		std::abs(m_v3ExpectedPos.GetZ() - m_v3CurPos.GetZ()) < 0.3)
	{
		m_v3CurPos = m_v3ExpectedPos;
	}

	CVector3D vRotTemp(m_v3ExpectedRot - m_v3CurRot);
	vRotTemp.Normalize();
	vRotTemp *= 0.3;

	if (std::abs(m_v3ExpectedRot.GetX() - m_v3CurRot.GetX()) < 0.3 &&
		std::abs(m_v3ExpectedRot.GetY() - m_v3CurRot.GetY()) < 0.3 &&
		std::abs(m_v3ExpectedRot.GetZ() - m_v3CurRot.GetZ()) < 0.3)
	{
		m_v3CurRot = m_v3ExpectedRot;
	}

}

void CWeapon::Show()
{
	glPushMatrix();

	glDepthMask(GL_FALSE); // or glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	// r, g, b constant components are unused below, only constant 0.5f alpha
	glBlendColor(0.0f, 0.0f, 0.0f, 0.5f);
	// set source and destination factors to 0.5f (50%) defined by above call
	glBlendFunc(GL_CONSTANT_ALPHA, GL_CONSTANT_ALPHA);
		glTranslatef(m_v3CurPos.GetX(), m_v3CurPos.GetY(), m_v3CurPos.GetZ());
		glRotatef(m_v3CurRot.GetX(), 1.0f, 0.0f, 0.0f);
		glRotatef(m_v3CurRot.GetY(), 0.0f, 1.0f, 0.0f);
		glRotatef(m_v3CurRot.GetZ(), 0.0f, 0.0f, 1.0f);
		glCallList(m_vFrames[m_uiCurFrame]);
	glDepthMask( GL_TRUE );
	glDisable( GL_BLEND );

	glPopMatrix();
}

bool CWeapon::Fire(CVector3D &vDir, const CVector3D vCamDir)
{
	if (m_bIsReloading)
	{
		return (false);
	}

	if ((!m_bIsAutomatic && !m_bIsFiring) || m_bIsAutomatic)
	{
		if (m_uiLastShot >= m_uiSpeed)
		{
			if (m_uiCurBullets > 0)
			{
				if (m_bIsAiming)
				{
					vDir.SetX(vCamDir.GetX() + (static_cast<float>(rand() % 2 - 1) / m_fAimPrecision));
					vDir.SetY(vCamDir.GetY() + (static_cast<float>(rand() % 2 - 1) / m_fAimPrecision));
					vDir.SetZ(vCamDir.GetZ() + (static_cast<float>(rand() % 2 - 1) / m_fAimPrecision));
				}
				else
				{
					vDir.SetX(vCamDir.GetX() + (static_cast<float>(rand() % 2 - 1) / m_fPrecision));
					vDir.SetY(vCamDir.GetY() + (static_cast<float>(rand() % 2 - 1) / m_fPrecision));
					vDir.SetZ(vCamDir.GetZ() + (static_cast<float>(rand() % 2 - 1) / m_fPrecision));
				}
				m_bIsFiring = true;
				m_uiLastShot = 0;
				m_uiCurBullets--;
				m_uiCurFrame = m_uiNormalAnimation;
				m_uiCurMode = 2;
				Mix_PlayChannel(-1, m_sFireSound, 0);
				return (true);
			}
			else
			{
				Mix_PlayChannel(-1, m_sEmptySound, 0);
				Reload();
				return (false);
			}
		}
	}
	return (false);
}

void CWeapon::Reload()
{
	if (!m_bIsReloading && m_uiMaxMagCapacity != m_uiCurBullets)
	{
		m_bIsReloading = true;
		if (m_uiAllBullets > m_uiMaxMagCapacity - m_uiCurBullets)
		{
			m_uiAllBullets -= m_uiMaxMagCapacity - m_uiCurBullets;
			m_uiCurBullets = m_uiMaxMagCapacity;
		}
		else
		{
			m_uiCurBullets = m_uiAllBullets + m_uiCurBullets;
			m_uiAllBullets = 0;
		}
		m_uiCurFrame = m_uiNormalAnimation + m_uiFireAnimation;
		m_uiCurMode = 3;
		Mix_PlayChannel(-1, m_sReloadSound, 0);
	}
}

void CWeapon::StopFire()
{
	m_bIsFiring = false;
}

void CWeapon::Aim()
{
	if (IsAiming())
	{
		m_bIsAiming = false;
	}
	else
	{
		m_bIsAiming = true;
	}

	if (m_bIsAiming)
	{
		m_v3CurPos = m_v3AimPos;
		m_v3CurRot = m_v3AimRot;
	}
	else
	{
		m_v3CurPos = m_v3Pos;
		m_v3CurRot = m_v3Rot;
	}
}

int32_t CWeapon::GetWeaponStrength() const
{
	return (m_iStrength);
}

void CWeapon::AddBullets(const uint32_t iNum)
{
	m_uiAllBullets += iNum;
}

void CWeapon::SetBullets(const uint32_t iNum)
{
	m_uiAllBullets = iNum;
}

uint32_t CWeapon::GetCurrentAmmo() const
{
	return (m_uiCurBullets);
}

uint32_t CWeapon::GetAllAmmo() const
{
	return (m_uiAllBullets);
}

uint32_t CWeapon::GetMagCap() const
{
	return (m_uiMaxMagCapacity);
}

std::string CWeapon::GetName() const
{
	return (m_stName);
}

std::vector<uint32_t>& CWeapon::GetAnimation()
{
	return (m_vFrames);
}

bool CWeapon::IsAiming() const
{
	return (m_bIsAiming);
}

uint32_t CWeapon::GetOuterView() const
{
	return (m_uiOuterView);
}

void CWeapon::SetStrength(const int32_t iNum)
{
	m_iStrength = iNum;
}
