//
// Created by osama on 10/05/24.
//

#include "ObjectLoader.h"
#include <cstring>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "../Debug/Log.h"
#include "Utils.h"
#include "../Utils/stb_image.h"

SCoordinate::SCoordinate(const float a, const float b, const float c)
{
	x = a;
	y = b;
	z = c;
}

SFace::SFace(const int32_t iFNum, const int32_t iFace1, const int32_t iFace2, const int32_t iFace3,
    const int32_t iTexCoord1, const int32_t iTexCoord2, const int32_t iTexCoord3, const int32_t iMat) : iFaces {999}, iTexCoord{999}
{
    iFaceNum = iFNum;
    bFour = false;
    iFaces[0] = iFace1;
    iFaces[1] = iFace2;
    iFaces[2] = iFace3;
    iTexCoord[0] = iTexCoord1;
    iTexCoord[1] = iTexCoord2;
    iTexCoord[2] = iTexCoord3;
    mat = iMat;
}

SFace::SFace(const int32_t iFNum, const int32_t iFace1, const int32_t iFace2, const int32_t iFace3, const int32_t iFace4,
    const int32_t iTexCoord1, const int32_t iTexCoord2, const int32_t iTexCoord3, const int32_t iTexCoord4, const int32_t iMat)
	: iFaces {999}, iTexCoord{999}
{
    iFaceNum = iFNum;
    bFour = true;
    iFaces[0] = iFace1;
    iFaces[1] = iFace2;
    iFaces[2] = iFace3;
    iFaces[3] = iFace4;
    iTexCoord[0] = iTexCoord1;
    iTexCoord[1] = iTexCoord2;
    iTexCoord[2] = iTexCoord3;
    iTexCoord[3] = iTexCoord4;
    mat = iMat;
}

SMaterial::SMaterial(const std::string &matName, const float fAlpha, const float fNumShining, const float fNumDensity,
    const float *fDiffColor, const float *fAmbColor, const float* fSpecColor, const int32_t illumNum, const int32_t iTexNum)
{
    materialName = matName;
    alpha = fAlpha;
    numShining = fNumShining;
    numDensity = fNumDensity;
    diffColor[0] = fDiffColor[0];
    diffColor[1] = fDiffColor[1];
    diffColor[2] = fDiffColor[2];
    ambientColor[0] = fAmbColor[0];
    ambientColor[1] = fAmbColor[1];
    ambientColor[2] = fAmbColor[2];
    specularColor[0] = fSpecColor[0];
    specularColor[1] = fSpecColor[1];
    specularColor[2] = fSpecColor[2];
    illuminationNum = illumNum;
    textureNum = iTexNum;
}

STextureCoord::STextureCoord(const float a, const float b)
{
    u = a;
    v = b;
}

CObjectLoader::CObjectLoader()
{
    bIsMaterial = false;
    bIsTexture = false;
    bIsNormals = false;
    bIsVertexNormal = false;
}

CObjectLoader::~CObjectLoader()
{
    for (const auto it : texturesVec)
    {
        glDeleteTextures(1, &(it));
    }
    for (const auto it : listsVec)
    {
        glDeleteLists((it), 1);
    }
}

int32_t CObjectLoader::Load(const std::string& fileName, std::vector<CCollisionPlane>* collisionPlaneVec)
{
	bIsMaterial=false;
	bIsNormals=false;
	bIsTexture=false;
	bIsVertexNormal=false;
	std::ifstream in(fileName.c_str());
	if(!in.is_open())
	{
		return -1;
	}
	std::string path=fileName.substr(0,((fileName.find_last_of('/')+1!=std::string::npos) ? (fileName.find_last_of('/')+1):0));
	char buf[256];
	int curmat=0;
	bool coll=false;
	while(!in.eof())
	{
		in.getline(buf,256);
		coordsVec.emplace_back(new std::string(buf));
	}
	for (int32_t i = 0 ;i<coordsVec.size(); i++)
	{
		if((*coordsVec[i])[0]=='#')
			continue;
		else if((*coordsVec[i])[0]=='v' && (*coordsVec[i])[1]==' ')
		{
			float tmpx,tmpy,tmpz;
			sscanf(coordsVec[i]->c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz);
			vertexVec.emplace_back(new CVector3D(tmpx,tmpy,tmpz));
		}else if((*coordsVec[i])[0]=='v' && (*coordsVec[i])[1]=='n')
		{
			float tmpx,tmpy,tmpz;
			sscanf(coordsVec[i]->c_str(),"vn %f %f %f",&tmpx,&tmpy,&tmpz);
			normalsVec.emplace_back(new CVector3D(tmpx,tmpy,tmpz));
		}else if((*coordsVec[i])[0]=='f')
		{
			int a,b,c,d,e;
			if(coll && collisionPlaneVec != nullptr)
			{
				sscanf(coordsVec[i]->c_str(),"f %d//%d %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b,&e,&b);
				collisionPlaneVec->emplace_back(normalsVec[b-1]->GetX(),normalsVec[b-1]->GetY(),normalsVec[b-1]->GetZ(),
					vertexVec[a-1]->GetX(),vertexVec[a-1]->GetY(),vertexVec[a-1]->GetZ(),vertexVec[c-1]->GetX(),
					vertexVec[c-1]->GetY(),vertexVec[c-1]->GetZ(),vertexVec[d-1]->GetX(),vertexVec[d-1]->GetY(),
					vertexVec[d-1]->GetZ(),vertexVec[e-1]->GetX(),vertexVec[e-1]->GetY(),vertexVec[e-1]->GetZ());
			}else
			{
				if(count(coordsVec[i]->begin(),coordsVec[i]->end(),' ')==4)
				{
					if(coordsVec[i]->find("//")!=std::string::npos)
					{
						sscanf(coordsVec[i]->c_str(),"f %d//%d %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b,&e,&b);
						facesVec.emplace_back(new SFace(b,a,c,d,e,0,0,0,0,curmat));
					}else if(coordsVec[i]->find("/")!=std::string::npos)
					{
						int t[4];
						sscanf(coordsVec[i]->c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",&a,&t[0],&b,&c,&t[1],&b,&d,&t[2],&b,&e,&t[3],&b);
						facesVec.emplace_back(new SFace(b,a,c,d,e,t[0],t[1],t[2],t[3],curmat));
					}else{
						sscanf(coordsVec[i]->c_str(),"f %d %d %d %d",&a,&b,&c,&d);
						facesVec.emplace_back(new SFace(-1,a,b,c,d,0,0,0,0,curmat));
					}
				}else{
						if(coordsVec[i]->find("//")!=std::string::npos)
						{
							sscanf(coordsVec[i]->c_str(),"f %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b);
							facesVec.emplace_back(new SFace(b,a,c,d,0,0,0,curmat));
						}else if(coordsVec[i]->find("/")!=std::string::npos)
						{
							int t[3];
							sscanf(coordsVec[i]->c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d",&a,&t[0],&b,&c,&t[1],&b,&d,&t[2],&b);
							facesVec.emplace_back(new SFace(b,a,c,d,t[0],t[1],t[2],curmat));
						}else{
							sscanf(coordsVec[i]->c_str(),"f %d %d %d",&a,&b,&c);
							facesVec.emplace_back(new SFace(-1,a,b,c,0,0,0,curmat));
						}
				}
			}
	}else if((*coordsVec[i])[0]=='u' && (*coordsVec[i])[1]=='s' && (*coordsVec[i])[2]=='e')
	{
		char tmp[200];
		sscanf(coordsVec[i]->c_str(),"usemtl %s",tmp);
		if(strcmp(tmp,"collision")==0)
		{
			coll=true;
		}else{
			coll=false;
			for(int j = 0 ; j < materialsVec.size(); j++)
			{
				if(strcmp(materialsVec[j]->materialName.c_str(),tmp)==0)
				{
					curmat=j;
					break;
				}
			}
		}
	}else if((*coordsVec[i])[0]=='m' && (*coordsVec[i])[1]=='t' && (*coordsVec[i])[2]=='l' && (*coordsVec[i])[3]=='l')
	{
		char filen[200];
		sscanf(coordsVec[i]->c_str(),"mtllib %s",filen);
		std::string filen2=path+filen;
		std::ifstream mtlin(filen2.c_str());
		if(!mtlin.is_open())
		{
			Clean();
			return -1;
		}
		bIsMaterial=true;
		std::vector<std::string> tmp;
		char c[200];
		while(!mtlin.eof())
		{
			mtlin.getline(c,200);
			tmp.emplace_back(c);
		}
		char name[200];
		char filename[200];
		float amb[3],dif[3],spec[3],alpha,ns,ni;
		int illum;
		unsigned int texture;
		bool ismat=false;
		strcpy(filename,"\0");

		for(int i=0;i<tmp.size();i++)
		{
			if(tmp[i][0]=='#')
				continue;
			if(tmp[i][0]=='n' && tmp[i][1]=='e' && tmp[i][2]=='w')
			{
				if(ismat)
				{
					if(strcmp(filename,"\0")!=0 && strcmp(filename,"collision")!=0)
					{
						materialsVec.emplace_back(new SMaterial(name,alpha,ns,ni,dif,amb,spec,illum,texture));
						strcpy(filename,"\0");
					}else{
							materialsVec.emplace_back(new SMaterial(name,alpha,ns,ni,dif,amb,spec,illum,-1));
					}
				}
				ismat=false;
				sscanf(tmp[i].c_str(),"newmtl %s",name);
			}else if(tmp[i][0]=='N' && tmp[i][1]=='s')
			{
				sscanf(tmp[i].c_str(),"Ns %f",&ns);
				ismat=true;
			}else if(tmp[i][0]=='K' && tmp[i][1]=='a')
			{
				sscanf(tmp[i].c_str(),"Ka %f %f %f",&amb[0],&amb[1],&amb[2]);
				ismat=true;
			}else if(tmp[i][0]=='K' && tmp[i][1]=='d')
			{
				sscanf(tmp[i].c_str(),"Kd %f %f %f",&dif[0],&dif[1],&dif[2]);
				ismat=true;
			}else if(tmp[i][0]=='K' && tmp[i][1]=='s')
			{
				sscanf(tmp[i].c_str(),"Ks %f %f %f",&spec[0],&spec[1],&spec[2]);
				ismat=true;
			}else if(tmp[i][0]=='N' && tmp[i][1]=='i')
			{
				sscanf(tmp[i].c_str(),"Ni %f",&ni);
				ismat=true;
			}else if(tmp[i][0]=='d' && tmp[i][1]==' ')
			{
				sscanf(tmp[i].c_str(),"d %f",&alpha);
				ismat=true;
			}else if(tmp[i][0]=='i' && tmp[i][1]=='l')
			{
				sscanf(tmp[i].c_str(),"illum %d",&illum);
				ismat=true;
			}else if(tmp[i][0]=='m' && tmp[i][1]=='a')
			{
				sscanf(tmp[i].c_str(),"map_Kd %s",filename);
				bool l= false;
				std::string filename2=path+filename;
				for(int i=0;i<loadedTexturesStr.size();i++)
					if(loadedTexturesStr[i]==filename2)
					{
						texture=loadedTexturesNums[i];
						l = true;
						break;
					}
				if(l == false)
					texture=LoadObjectTexture(filename2.c_str());
				ismat=true;
			}
		}
				if(ismat)
				{
					if(strcmp(filename,"\0")!=0)
					{
						materialsVec.emplace_back(new SMaterial(name,alpha,ns,ni,dif,amb,spec,illum,texture));
					}else{
							materialsVec.emplace_back(new SMaterial(name,alpha,ns,ni,dif,amb,spec,illum,-1));
					}
				}
	}else if((*coordsVec[i])[0]=='v' && (*coordsVec[i])[1]=='t')
	{
		float u,v;
		sscanf(coordsVec[i]->c_str(),"vt %f %f",&u,&v);
		textureCoordinateVec.emplace_back(new STextureCoord(u,1-v));
		bIsTexture=true;
	}
}
	if(materialsVec.size()==0)
		bIsMaterial=false;
	else
		bIsMaterial=true;

	//draw
	if(bIsVertexNormal)
		SmoothNormals();
	int num;
	num=glGenLists(1);
	glNewList(num,GL_COMPILE);
	int last=-1;
	for(int i=0;i<facesVec.size();i++)
	{
		if(last!=facesVec[i]->mat && bIsMaterial)
		{
			float diffuse[]={materialsVec[facesVec[i]->mat]->diffColor[0],materialsVec[facesVec[i]->mat]->diffColor[1],materialsVec[facesVec[i]->mat]->diffColor[2],1.0};
			float ambient[]={materialsVec[facesVec[i]->mat]->ambientColor[0],materialsVec[facesVec[i]->mat]->ambientColor[1],materialsVec[facesVec[i]->mat]->ambientColor[2],1.0};
			float specular[]={materialsVec[facesVec[i]->mat]->specularColor[0],materialsVec[facesVec[i]->mat]->specularColor[1],materialsVec[facesVec[i]->mat]->specularColor[2],1.0};
			glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
			glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
			glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
			glMaterialf(GL_FRONT,GL_SHININESS,materialsVec[facesVec[i]->mat]->numShining);
			glColor3f(diffuse[0],diffuse[1],diffuse[2]);
			last=facesVec[i]->mat;
			if(materialsVec[facesVec[i]->mat]->textureNum==-1)
				glDisable(GL_TEXTURE_2D);
			else{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,materialsVec[facesVec[i]->mat]->textureNum);
			}
		}
		if(facesVec[i]->iTexCoord[0]==0)
			bIsTexture=false;
		else
			bIsTexture=true;

		bIsNormals=true;
		if(facesVec[i]->bFour)
		{
			glBegin(GL_QUADS);

				if(bIsNormals)
					glNormal3f(normalsVec[facesVec[i]->iFaceNum-1]->GetX(),normalsVec[facesVec[i]->iFaceNum-1]->GetY(),
						normalsVec[facesVec[i]->iFaceNum-1]->GetZ());

				if(bIsTexture && materialsVec[facesVec[i]->mat]->textureNum!=-1)
					glTexCoord2f(textureCoordinateVec[facesVec[i]->iTexCoord[0]-1]->u,textureCoordinateVec[facesVec[i]->iTexCoord[0]-1]->v);

				if(bIsVertexNormal)
					glNormal3f(vertexNormalVec[facesVec[i]->iFaces[0]-1]->GetX(),vertexNormalVec[facesVec[i]->iFaces[0]-1]->GetY(),
						vertexNormalVec[facesVec[i]->iFaces[0]-1]->GetZ());



				glVertex3f(vertexVec[facesVec[i]->iFaces[0]-1]->GetX(),vertexVec[facesVec[i]->iFaces[0]-1]->GetY(),vertexVec[facesVec[i]->iFaces[0]-1]->GetZ());

				if(bIsTexture && materialsVec[facesVec[i]->mat]->textureNum!=-1)
					glTexCoord2f(textureCoordinateVec[facesVec[i]->iTexCoord[1]-1]->u,textureCoordinateVec[facesVec[i]->iTexCoord[1]-1]->v);


				if(bIsVertexNormal)
					glNormal3f(vertexNormalVec[facesVec[i]->iFaces[1]-1]->GetX(),vertexNormalVec[facesVec[i]->iFaces[1]-1]->GetY(),
						vertexNormalVec[facesVec[i]->iFaces[1]-1]->GetZ());

				glVertex3f(vertexVec[facesVec[i]->iFaces[1]-1]->GetX(),vertexVec[facesVec[i]->iFaces[1]-1]->GetY(),
					vertexVec[facesVec[i]->iFaces[1]-1]->GetZ());

				if(bIsTexture && materialsVec[facesVec[i]->mat]->textureNum!=-1)
					glTexCoord2f(textureCoordinateVec[facesVec[i]->iTexCoord[2]-1]->u,textureCoordinateVec[facesVec[i]->iTexCoord[2]-1]->v);

				if(bIsVertexNormal)
					glNormal3f(vertexNormalVec[facesVec[i]->iFaces[2]-1]->GetX(),vertexNormalVec[facesVec[i]->iFaces[2]-1]->GetY(),
						vertexNormalVec[facesVec[i]->iFaces[2]-1]->GetZ());

				glVertex3f(vertexVec[facesVec[i]->iFaces[2]-1]->GetX(),vertexVec[facesVec[i]->iFaces[2]-1]->GetY(),vertexVec[facesVec[i]->iFaces[2]-1]->GetZ());

				if(bIsTexture && materialsVec[facesVec[i]->mat]->textureNum!=-1)
					glTexCoord2f(textureCoordinateVec[facesVec[i]->iTexCoord[3]-1]->u,textureCoordinateVec[facesVec[i]->iTexCoord[3]-1]->v);

				if(bIsVertexNormal)
					glNormal3f(vertexNormalVec[facesVec[i]->iFaces[3]-1]->GetX(),vertexNormalVec[facesVec[i]->iFaces[3]-1]->GetY(),
						vertexNormalVec[facesVec[i]->iFaces[3]-1]->GetZ());

				glVertex3f(vertexVec[facesVec[i]->iFaces[3]-1]->GetX(),vertexVec[facesVec[i]->iFaces[3]-1]->GetY(),vertexVec[facesVec[i]->iFaces[3]-1]->GetZ());
			glEnd();
		}else{
			glBegin(GL_TRIANGLES);
				if(bIsNormals)
					glNormal3f(normalsVec[facesVec[i]->iFaceNum-1]->GetX(),normalsVec[facesVec[i]->iFaceNum-1]->GetY(),
						normalsVec[facesVec[i]->iFaceNum-1]->GetZ());

				if(bIsTexture && materialsVec[facesVec[i]->mat]->textureNum!=-1)
					glTexCoord2f(textureCoordinateVec[facesVec[i]->iTexCoord[0]-1]->u,textureCoordinateVec[facesVec[i]->iTexCoord[0]-1]->v);

				if(bIsVertexNormal)
					glNormal3f(vertexNormalVec[facesVec[i]->iFaces[0]-1]->GetX(),vertexNormalVec[facesVec[i]->iFaces[0]-1]->GetY(),
						vertexNormalVec[facesVec[i]->iFaces[0]-1]->GetZ());


				glVertex3f(vertexVec[facesVec[i]->iFaces[0]-1]->GetX(),vertexVec[facesVec[i]->iFaces[0]-1]->GetY(),
					vertexVec[facesVec[i]->iFaces[0]-1]->GetZ());

				if(bIsTexture && materialsVec[facesVec[i]->mat]->textureNum!=-1)
					glTexCoord2f(textureCoordinateVec[facesVec[i]->iTexCoord[1]-1]->u,textureCoordinateVec[facesVec[i]->iTexCoord[1]-1]->v);


				if(bIsVertexNormal)
					glNormal3f(vertexNormalVec[facesVec[i]->iFaces[1]-1]->GetX(),vertexNormalVec[facesVec[i]->iFaces[1]-1]->GetY(),
						vertexNormalVec[facesVec[i]->iFaces[1]-1]->GetZ());

				glVertex3f(vertexVec[facesVec[i]->iFaces[1]-1]->GetX(),vertexVec[facesVec[i]->iFaces[1]-1]->GetY(),
					vertexVec[facesVec[i]->iFaces[1]-1]->GetZ());


				if(bIsTexture && materialsVec[facesVec[i]->mat]->textureNum!=-1)
					glTexCoord2f(textureCoordinateVec[facesVec[i]->iTexCoord[2]-1]->u,textureCoordinateVec[facesVec[i]->iTexCoord[2]-1]->v);


				if(bIsVertexNormal)
					glNormal3f(vertexNormalVec[facesVec[i]->iFaces[2]-1]->GetX(),vertexNormalVec[facesVec[i]->iFaces[2]-1]->GetY(),
						vertexNormalVec[facesVec[i]->iFaces[2]-1]->GetZ());

				glVertex3f(vertexVec[facesVec[i]->iFaces[2]-1]->GetX(),vertexVec[facesVec[i]->iFaces[2]-1]->GetY(),
					vertexVec[facesVec[i]->iFaces[2]-1]->GetZ());
			glEnd();
		}
	}
	glEndList();
	Clean();
	listsVec.emplace_back(num);
	return num;
}

void CObjectLoader::Clean()
{
    for (const auto &coord : coordsVec)
    {
        delete coord;
    }

    for (const auto &vertex : vertexVec)
    {
        delete vertex;
    }

    for (const auto &face : facesVec)
    {
        delete face;
    }

    for (const auto &normal : normalsVec)
    {
        delete normal;
    }

    for (const auto &material : materialsVec)
    {
        delete material;
    }

    for (const auto &texCoord : textureCoordinateVec)
    {
        delete texCoord;
    }

    for (const auto &vertexNorm : vertexNormalVec)
    {
        delete vertexNorm;
    }

    coordsVec.clear();
    vertexVec.clear();
    facesVec.clear();
    normalsVec.clear();
    materialsVec.clear();
    textureCoordinateVec.clear();
    vertexNormalVec.clear();
}

void CObjectLoader::SmoothNormals()
{
    for (uint32_t i = 1; i < vertexVec.size() + 1; i++)
    {
        float vecX = 0.0f, vecY = 0.0f, vecZ = 0.0f;
        float num = 0;

        for (const auto& face : facesVec)
        {
            if (face->iFaces[0] == (i) || face->iFaces[1] == (i) || face->iFaces[2] == (i) || face->iFaces[3] == (i))
            {
                vecX += normalsVec[face->iFaceNum - 1]->GetX();
                vecY += normalsVec[face->iFaceNum - 1]->GetY();
                vecZ += normalsVec[face->iFaceNum - 1]->GetZ();
                num++;
            }
        }

        if (num > 0.0f)
        {
            vecX /= num;
            vecY /= num;
            vecZ /= num;
        }

        static float fDistance = std::sqrt(vecX*vecX + vecY*vecY + vecZ*vecZ);
        if (fDistance > 0.0f)
        {
            vecX /= fDistance;
            vecY /= fDistance;
            vecZ /= fDistance;
        }
        vertexNormalVec.emplace_back(new CVector3D(vecX, vecY, vecZ));
    }
}

uint32_t CObjectLoader::LoadObjectTexturePNG(const char *c_szFileName)
{
	GLuint uiTextureID;
	glGenTextures(1, &uiTextureID);

	int32_t iWidth, iHeight, iNumComponents;

	unsigned char* c_TexData = stbi_load(c_szFileName, &iWidth, &iHeight, &iNumComponents, 0);
	if (c_TexData)
	{
		GLenum ePictureFormat {};

		if (iNumComponents == 1)
		{
			ePictureFormat = GL_RED;
		}
		else if (iNumComponents == 3)
		{
			ePictureFormat = GL_RGB;
		}
		else if (iNumComponents == 4)
		{
			ePictureFormat = GL_RGBA;
		}
		glBindTexture(GL_TEXTURE_2D, uiTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(ePictureFormat), iWidth, iHeight, 0, ePictureFormat, GL_UNSIGNED_BYTE, c_TexData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(c_TexData);
	}
	else
	{
		fprintf(stderr, "Failed to Load Texture : %s\n", c_szFileName);
		stbi_image_free(c_TexData);
	}

	return (uiTextureID);
}

uint32_t CObjectLoader::LoadObjectTextureBMP(const char *c_szFileName)
{
	uint32_t num;
	glGenTextures(1,&num);
	SDL_Surface* img=SDL_LoadBMP(c_szFileName);
	glBindTexture(GL_TEXTURE_2D,num);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->w,img->h,0,GL_RGB,GL_UNSIGNED_SHORT_5_6_5,img->pixels);
	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	SDL_FreeSurface(img);
	return (num);
}

uint32_t CObjectLoader::LoadObjectTexture(const char *c_szFileName)
{
	uint32_t num;
	if (GetExtension(std::string(c_szFileName)) == "png")
	{
		num = LoadPNGTexture(c_szFileName);
	}
	else if (GetExtension(std::string(c_szFileName)) == "bmp")
	{
		num = LoadObjectTextureBMP(c_szFileName);
	}
	texturesVec.push_back(num);
	loadedTexturesStr.emplace_back(c_szFileName);
	loadedTexturesNums.emplace_back(num);
	return num;
}
