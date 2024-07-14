//
// Created by osama on 14/07/24.
//

#include "Item.h"

#include <GL/gl.h>
#include "../BaseLib/CollisionSphere.h"

SItemData::SItemData(const int32_t &ID, const uint32_t &uiMesh, const CCollisionSphere &colSphere)
{
	m_ID = ID;
	m_uiMesh = uiMesh;
	m_v3Rot.Change(0.0f, 0.0f, 0.0f);
	m_colSphere = colSphere;
	m_v3Scale = CVector3D(1.0f, 1.0f, 1.0f);
}

CItem::CItem(int32_t ID, uint32_t uiMesh, CCollisionSphere &colSphere)
{
	m_vecItems.emplace_back(ID, uiMesh, colSphere);
}

void CItem::Add(int32_t ID, uint32_t uiMesh, CCollisionSphere colSphere)
{
	m_vecItems.emplace_back(ID, uiMesh, colSphere);
}

void CItem::Show()
{
	glEnable(GL_NORMALIZE);
	for (auto& item : m_vecItems)
	{
		glPushMatrix();
			glTranslatef(item.m_colSphere.GetCenter().GetX(), item.m_colSphere.GetCenter().GetY(), item.m_colSphere.GetCenter().GetZ());
			glRotatef(item.m_v3Rot.GetX(), 1.0f, 0.0f, 0.0f);
			glRotatef(item.m_v3Rot.GetY(), 0.0f, 1.0f, 0.0f);
			glRotatef(item.m_v3Rot.GetZ(), 0.0f, 0.0f, 1.0f);
			glScalef(item.m_v3Scale.GetX(), item.m_v3Scale.GetY(), item.m_v3Scale.GetZ());
			glCallList(item.m_uiMesh);
		glPopMatrix();
	}
	glDisable(GL_NORMALIZE);
}

int32_t CItem::Update(CCollisionSphere PlayerSphere)
{
	for (auto& item : m_vecItems)
	{
		item.m_v3Rot.SetY(item.m_v3Rot.GetY() + 1);
		if (item.m_v3Rot.GetY() > 360)
			item.m_v3Rot.SetY(item.m_v3Rot.GetY() - 360);
	}

	for (int32_t i = 0; i < m_vecItems.size(); i++)
	{
		if (SphereSphere(m_vecItems[i].m_colSphere.GetCenter(), m_vecItems[i].m_colSphere.GetRadius(), PlayerSphere.GetCenter(), PlayerSphere.GetRadius()))
		{
			const int32_t id = m_vecItems[i].m_ID;
			m_vecItems.erase(m_vecItems.begin() + i);
			return (id);
		}
	}
	return (-1);
}

void CItem::Delete(const int32_t ID)
{
	for (int32_t i = 0; i < m_vecItems.size(); i++)
	{
		if (m_vecItems[i].m_ID == ID)
		{
			m_vecItems.erase(m_vecItems.begin() + i);
			break;
		}
	}
}

