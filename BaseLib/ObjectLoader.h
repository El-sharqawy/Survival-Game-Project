#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H
#include <cstdint>
#include <string>
#include <vector>

#include "CollisionPlane.h"

typedef struct SCoordinate
{
	float x, y, z;
	SCoordinate(float a,float b,float c);
} TCoordinate;

typedef struct SFace
{
	int iFaceNum;
	bool bFour;
	int iFaces[4];
	int iTexCoord[4];
	int mat;
	SFace(int32_t iFNum, int32_t iFace1, int32_t iFace2, int32_t iFace3,
		int32_t iTexCoord1, int32_t iTexCoord2, int32_t iTexCoord3, int32_t iMat);
	SFace(int32_t iFNum, int32_t iFace1, int32_t iFace2, int32_t iFace3, int32_t iFace4,
		int32_t iTexCoord1, int32_t iTexCoord2, int32_t iTexCoord3, int32_t iTexCoord4, int32_t iMat);
} TFace;

typedef struct SMaterial
{
	std::string materialName;
	float alpha, numShining, numDensity;
	float diffColor[3], ambientColor[3], specularColor[3];
	int illuminationNum;
	int textureNum;
	SMaterial(const std::string &matName, float fAlpha, float fNumShining, float fNumDensity,
		const float *fDiffColor, const float *fAmbColor, const float* fSpecColor, int32_t illumNum, int32_t iTexNum);
} TMaterial;

typedef struct STextureCoord
{
	float u, v;
	STextureCoord(float a, float b);
} TTextureCoord;

class CObjectLoader
{
public:
	CObjectLoader();
	~CObjectLoader();

	int32_t Load(const std::string& fileName, std::vector<CCollisionPlane>* collisionPlaneVec);
	void Clean();
	void SmoothNormals();

	uint32_t LoadObjectTexturePNG(const char *c_szFileName);
	uint32_t LoadObjectTextureBMP(const char *c_szFileName);
	uint32_t LoadObjectTexture(const char *c_szFileName);

private:
	bool bIsMaterial, bIsNormals, bIsTexture, bIsVertexNormal;

	std::vector<std::string*> coordsVec;
	std::vector<CVector3D*> vertexVec;
	std::vector<SFace*> facesVec;
	std::vector<CVector3D*> normalsVec;
	std::vector<unsigned int> texturesVec;
	std::vector<unsigned int> listsVec;
	std::vector<SMaterial*> materialsVec;
	std::vector<STextureCoord*> textureCoordinateVec;
	std::vector<CVector3D*> vertexNormalVec;
	std::vector<std::string> loadedTexturesStr;
	std::vector<int> loadedTexturesNums;
};

#endif
