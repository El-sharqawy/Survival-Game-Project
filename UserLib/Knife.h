//
// Created by osama on 14/07/24.
//

#ifndef KNIFE_H
#define KNIFE_H

#include <SDL_mixer.h>

#include "Player.h"
#include "Zombie.h"

class CKnife
{
public:
	CKnife(const std::vector<uint32_t>& Frames, int32_t iStr, int32_t iDel, int32_t iWepDelay, CVector3D v3Pos, Mix_Chunk* Hit, Mix_Chunk* Norm);
	~CKnife() = default;

	int32_t SetKnife(CPlayer *pPlayer, CZombie* pZombie);
	void Update(CPlayer *pPlayer);
	void Show() const;
	[[nodiscard]]bool IsKnifing() const;
	[[nodiscard]]int32_t GetStrength() const;

private:
	std::vector<uint32_t> m_vFrames;
	bool m_bIsKnifing;
	int32_t m_iStrength;
	int32_t m_iDelay;
	int32_t m_iWeaponDelay;
	int32_t m_iLastTimeHit;
	int32_t m_iCurFrame;
	CVector3D m_v3Position;
	Mix_Chunk *m_HitSound;
	Mix_Chunk *m_NormalSound;
};



#endif //KNIFE_H
