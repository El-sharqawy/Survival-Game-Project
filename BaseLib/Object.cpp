//
// Created by osama on 03/07/24.
//

#include "Object.h"

COneObject::COneObject(const std::vector<uint32_t> &vFrames, const std::vector<CCollisionPlane> &vColPlanes, const std::string &stName, const int32_t iID)
{
	m_vFrames = vFrames;
	m_vColPlanes = vColPlanes;
	m_stName = stName;
	m_Id = iID;

}

std::string &COneObject::GetName()
{
	return (m_stName);
}

int32_t COneObject::GetID() const
{
	return (m_Id);
}

SObjectPosition::SObjectPosition(const int32_t id, const CVector3D pos)
{
	m_Id = id;
	m_v3Pos = pos;
}

void CObject::AddObject(const std::vector<uint32_t>& vFrames, const std::vector<CCollisionPlane>& colPlanes,
	const CVector3D & v3Pos, const std::string& sName, const int32_t iID)
{
	for (auto& obj : m_vOneObj)
	{
		if (obj.GetID() == iID)
		{
			m_vObjPos.emplace_back(iID, v3Pos);
		}
		else
		{
			m_vOneObj.emplace_back(vFrames, colPlanes, sName, iID);
			m_vObjPos.emplace_back(iID, v3Pos);
		}
	}
}

void CObject::AddObject(const int32_t iID, const CVector3D v3Pos)
{
	for (auto& obj : m_vOneObj)
	{
		if (obj.GetID() == iID)
		{
			m_vObjPos.emplace_back(iID, v3Pos);
		}
	}
}

void CObject::AddObject(const std::string& sName, const CVector3D v3Pos)
{
	for (auto& obj : m_vOneObj)
	{
		if (obj.GetName() == sName)
		{
			m_vObjPos.emplace_back(obj.GetID(), v3Pos);
		}
	}
}

bool CObject::DeleteObject(const int32_t iID)
{
	const auto initialSize = m_vOneObj.size();
	std::erase_if(m_vOneObj, [iID](const COneObject& obj) { return obj.GetID() == iID; });
	return m_vOneObj.size() != initialSize;
}

bool CObject::DeleteObject(const std::string &sName)
{
	const auto initialSize = m_vOneObj.size();
	std::erase_if(m_vOneObj, [sName](COneObject& obj) { return obj.GetName() == sName; });
	return m_vOneObj.size() != initialSize;
}
