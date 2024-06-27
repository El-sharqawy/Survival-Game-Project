//
// Created by osama on 10/05/24.
//

#include "Map.h"

#include <GL/gl.h>

CMap::CMap(const uint32_t uiMesh, const std::vector<CCollisionPlane> &colPlanes, const std::string &sName,
    const std::vector<std::string> &skyImages, const std::vector<CVector3D> &vSP)
{
    mesh = uiMesh;
    vColPlanes = colPlanes;
    szName = sName;
    spawnPoints = vSP;
    skyBox = new CSkyBox(skyImages[0].c_str(), skyImages[1].c_str(), skyImages[2].c_str(),
        skyImages[3].c_str(), skyImages[4].c_str(), skyImages[5].c_str());
}

void CMap::Show() const
{
    glCallList(mesh);
}

void CMap::Change(const uint32_t uiMesh, const std::vector<CCollisionPlane>& colPlanes,
    const std::string &sName, std::string skyImages[])
{
    mesh = uiMesh;
    vColPlanes = colPlanes;
    szName = sName;
    skyBox->Change(skyImages[0].c_str(), skyImages[1].c_str(), skyImages[2].c_str(),
        skyImages[3].c_str(), skyImages[4].c_str(), skyImages[5].c_str());
}

std::vector<CCollisionPlane>& CMap::GetCollisionPlanes()
{
    return (vColPlanes);
}

std::string& CMap::GetName()
{
    return (szName);
}

std::vector<CVector3D>* CMap::GetSpawnPoints()
{
    return (&spawnPoints);
}

CVector3D* CMap::GetRandomSpawnPoints()
{
    return &(spawnPoints[random() % spawnPoints.size()]);
}

CSkyBox *CMap::GetSkyBox() const
{
    return (skyBox);
}
