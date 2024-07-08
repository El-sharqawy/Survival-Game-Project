//
// Created by osama on 30/06/24.
//

#ifndef WEAPON_H
#define WEAPON_H

#include <vector>
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include "../BaseLib/Vector3D.h"
#include <SDL2/SDL_mixer.h>

class CWeapon
{

public:

	CWeapon(std::vector<uint32_t>& vAnimFrames,uint32_t uiNormAnim,uint32_t uiFireAnim, uint32_t uiReloadAnim,
	Mix_Chunk *fireSound,Mix_Chunk* reloadSound,Mix_Chunk* normalSound,Mix_Chunk* emptySound,CVector3D v3Pos,CVector3D v3Rot,CVector3D v3Apos,CVector3D v3Arot,
	float fPrec,float fAprec, uint32_t uiBulletsNum, float Length, uint32_t iSpeed,bool bIsAuto, int32_t iStr, const char* c_szName, uint32_t uiOutView);

	~CWeapon() = default;

	void Update();
	void Show();

	bool Fire(CVector3D& vDir, CVector3D vCamDir);
	void StopFire();

	void Reload();

	void Aim();

	void AddBullets(uint32_t iNum);
	void SetBullets(uint32_t iNum);
	void SetStrength(int32_t iNum);

	[[nodiscard]]int32_t GetWeaponStrength() const;
	[[nodiscard]]uint32_t GetCurrentAmmo() const;
	[[nodiscard]]uint32_t GetAllAmmo() const;
	[[nodiscard]]uint32_t GetMagCap() const;
	[[nodiscard]]std::string GetName() const;

	std::vector<uint32_t>& GetAnimation();

	[[nodiscard]]bool IsAiming() const;
	[[nodiscard]]uint32_t GetOuterView() const;

private:
	std::vector <uint32_t> m_vFrames;
	uint32_t m_uiOuterView;
	/**
	 * 0, 1 Normal
	 * 2, 3, 4 fireAnimation
	 * 5, 6, 7, 8 reloadAnimation
	 */
	uint32_t m_uiNormalAnimation;
	uint32_t m_uiFireAnimation;
	uint32_t m_uiReloadAnimation;
	CVector3D m_v3ExpectedPos;
	CVector3D m_v3ExpectedRot;

	CVector3D m_v3Pos;	/*** when we are not Aiming **/
	CVector3D m_v3Rot;

	CVector3D m_v3AimPos;	/*** when we are Aiming **/
	CVector3D m_v3AimRot;

	CVector3D m_v3CurPos;	/*** Current Position ***/
	CVector3D m_v3CurRot;

	float m_fPrecision;
	float m_fAimPrecision;
	int32_t m_iStrength;
	bool m_bIsAiming;
	bool m_bIsReloading;
	bool m_bIsAutomatic;
	bool m_bIsFiring;
	bool m_bIsTest;

	uint32_t m_uiMaxMagCapacity;	/*** Max Bullets in Mag ***/
	uint32_t m_uiCurBullets;	/*** Current Bullets in Mag ***/
	uint32_t m_uiAllBullets;	/*** All Weapon Bullets ***/

	uint32_t m_uiLastShot;	/*** Number of Frames ago ***/
	uint32_t m_uiSpeed;	/*** How many Frames we can shoot after ***/

	uint32_t m_uiCurFrame;
	uint32_t m_uiCurMode;	/*** 1 for normal, 2 for Fire, 3 for Reload ***/

	std::string m_stName;

	Mix_Chunk *m_sFireSound;
	Mix_Chunk *m_sReloadSound;
	Mix_Chunk *m_sNormalSound;
	Mix_Chunk *m_sEmptySound;
};



#endif //WEAPON_H
