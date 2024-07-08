//
// Created by osama on 02/07/24.
//

#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <vector>
#include <cstdint>

#include "../BaseLib/Collision.h"
#include "../BaseLib/CollisionPlane.h"
#include "../BaseLib/CollisionSphere.h"
#include "../BaseLib/Vector3D.h"

class CZombie : public CCollision
{
public:
	CZombie(const std::vector<uint32_t>& vAnimFrames, const uint32_t &uiWalk, const uint32_t &uiAttack, const uint32_t &uiDie,
		int32_t iHealth, int32_t iStrength, float fSpeed, CCollisionSphere ZCollSphere, CVector3D v3Rotation, CVector3D v3PlayerLoc);

	CZombie(int32_t iHealth, float fSpeed, int32_t iStrength, CCollisionSphere colSphere, CVector3D Rot, CVector3D playerLoc);

	~CZombie() = default;

	bool Update(const std::vector<CCollisionPlane>& vColPlanes, CVector3D playerLocation);
	void Show();
	bool SetAttack(CCollisionSphere playerSphere);
	[[nodiscard]]CCollisionSphere *GetCollisionSphere();
	void SetPosition(const CVector3D &newLocation);
	void SetHealth(int32_t iHealth);
	void TakeDamage(int32_t iDam);
	[[nodiscard]]int32_t GetHealth() const;
	[[nodiscard]]int32_t GetStrength() const;
	[[nodiscard]]bool IsDead() const;
	[[nodiscard]]bool IsWalking() const;
	[[nodiscard]]bool IsAttacking() const;
	[[nodiscard]]float GetSpeed() const;

	void SetIsWalking(bool bIsWalking);
	void SetIsAttacking(bool bIsAttacking);
	void SetIsDead(bool bIsDead);

	[[nodiscard]]CVector3D GetPosition() const;
	[[nodiscard]]CVector3D GetRotation() const;
	[[nodiscard]]CVector3D GetStartPos() const;

private:
	static std::vector<uint32_t> m_vFrames;
	static uint32_t m_uiWalk, m_uiAttack, m_uiDie;
	uint32_t m_uiCurFrame;

	int32_t m_iHealth, m_iStrength;
	float m_fSpeed;
	CVector3D m_v3Position, m_v3Rotation, m_v3StartPos;
	bool m_bIsWalking;
	bool m_bIsAttacking;
	bool m_bIsDead;
	uint32_t m_iLastAttackTime;

	CCollisionSphere m_csZombieSphere;
};



#endif //ZOMBIE_H
