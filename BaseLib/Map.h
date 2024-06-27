//
// Created by osama on 10/05/24.
//

#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include "CollisionPlane.h"
#include "Map.h"
#include "SkyBox.h"
#include "Vector3D.h"

class CMap
{
public:
    CMap(uint32_t uiMesh, const std::vector<CCollisionPlane> &colPlanes, const std::string &sName,
         const std::vector<std::string> &skyImages, const std::vector<CVector3D> &vSP);
    ~CMap() = default;

    void Show() const;
    void Change(uint32_t uiMesh, const std::vector<CCollisionPlane> &colPlanes, const std::string &sName,
        std::string skyImages[]);

    std::vector<CCollisionPlane>& GetCollisionPlanes();
    std::string& GetName();
    std::vector<CVector3D>* GetSpawnPoints();
    CVector3D* GetRandomSpawnPoints();
    [[nodiscard]]CSkyBox* GetSkyBox() const;

private:
    uint32_t mesh;
    std::vector<CCollisionPlane> vColPlanes;
    std::vector<CVector3D> spawnPoints;
    std::string szName;
    CSkyBox *skyBox;
};



#endif //MAP_H
