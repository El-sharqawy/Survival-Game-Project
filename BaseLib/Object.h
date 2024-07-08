//
// Created by osama on 03/07/24.
//

#ifndef OBJECT_H
#define OBJECT_H

#include "Vector3D.h"
#include <cstdint>
#include <vector>
#include <string>
#include "CollisionPlane.h"

class COneObject
{
public:
	COneObject(const std::vector<uint32_t>& vFrames, const std::vector<CCollisionPlane>& vColPlanes,
		const std::string &stName, int32_t iID);
	std::string &GetName();
	[[nodiscard]]int32_t GetID() const;

private:
	std::vector<uint32_t> m_vFrames;
	std::vector<CCollisionPlane> m_vColPlanes;
	std::string m_stName;
	int32_t m_Id;
};

typedef struct SObjectPosition
{
	int32_t m_Id;
	CVector3D m_v3Pos;
	SObjectPosition(int32_t id, CVector3D pos);
} TObjectPosition;

class CObject
{
public:
	CObject() = default;
	~CObject() = default;

	void AddObject(const std::vector<uint32_t>& vFrames, const std::vector<CCollisionPlane>& colPlanes,
		const CVector3D & v3Pos, const std::string& sName, int32_t iID);
	void AddObject(int32_t iID, CVector3D v3Pos);
	void AddObject(const std::string& sName, CVector3D v3Pos);
	bool DeleteObject(int32_t iID);
	bool DeleteObject(const std::string &sName);

private:
	std::vector<COneObject> m_vOneObj;
	std::vector<TObjectPosition> m_vObjPos;
};

#endif //OBJECT_H
