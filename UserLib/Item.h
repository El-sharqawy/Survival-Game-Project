//
// Created by osama on 14/07/24.
//

#ifndef ITEM_H
#define ITEM_H
#include "../BaseLib/Collision.h"
#include "../BaseLib/CollisionSphere.h"
#include "../BaseLib/Vector3D.h"

#include <cstdint>
#include <vector>

typedef struct SItemData
{
	int32_t m_ID;
	uint32_t m_uiMesh;
	CVector3D m_v3Rot;
	CCollisionSphere m_colSphere;
	CVector3D m_v3Scale;

	SItemData(const int32_t &ID, const uint32_t &uiMesh, const CCollisionSphere &colSphere);
} TItemData;

class CItem : public CCollision
{
public:
	CItem() = default;
	CItem(int32_t ID, uint32_t uiMesh, CCollisionSphere& colSphere);
	~CItem() = default;
	int32_t Update(CCollisionSphere PlayerSphere);
	void Show();
	void Add(int32_t ID, uint32_t uiMesh, CCollisionSphere colSphere);
	void Delete(int32_t ID);

private:
	std::vector<TItemData> m_vecItems;
};



#endif //ITEM_H
