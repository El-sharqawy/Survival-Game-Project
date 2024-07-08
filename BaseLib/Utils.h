//
// Created by osama on 14/06/24.
//

#ifndef UTILS_H
#define UTILS_H
#include "../Debug/Log.h"
#include <cstdint>
#include "upng.h"
#include <GL/gl.h>
#include <cstdio>
#include <SDL_image.h>

static uint32_t LoadPNGTexture(const char *c_szFileName);
static uint32_t LoadBMPTexture(const char *c_szFileName);
static uint32_t LoadTexture(const char* c_szFileName);

static std::string GetExtension(const std::string& c_szFileName);

inline uint32_t LoadPNGTexture(const char *c_szFileName)
{
	uint32_t textureID;
	glGenTextures(1, &textureID);

	upng_t * pngIMG = upng_new_from_file(c_szFileName);
	if (pngIMG != nullptr)
	{
		upng_decode(pngIMG);
		if (upng_get_error(pngIMG) == UPNG_EOK)
		{
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			switch (upng_get_components(pngIMG)) {
				case 1:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, upng_get_width(pngIMG), upng_get_height(pngIMG), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, upng_get_buffer(pngIMG));
				break;
				case 2:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, upng_get_width(pngIMG), upng_get_height(pngIMG), 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, upng_get_buffer(pngIMG));
				break;
				case 3:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, upng_get_width(pngIMG), upng_get_height(pngIMG), 0, GL_RGB, GL_UNSIGNED_BYTE, upng_get_buffer(pngIMG));
				break;
				case 4:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, upng_get_width(pngIMG), upng_get_height(pngIMG), 0, GL_RGBA, GL_UNSIGNED_BYTE, upng_get_buffer(pngIMG));
				break;
				default:
					return 1;
			}
		}
	}
	fprintf(stdout, "LoadPNGTexture Loaded Texture %s, Id : %d\n", c_szFileName, textureID);
	upng_free(pngIMG);
	return (textureID);
}

inline uint32_t LoadBMPTexture(const char *c_szFileName)
{
	uint32_t textureID;	//the id for the texture
	glGenTextures(1,&textureID);	//we generate a unique one
	SDL_Surface* img = SDL_LoadBMP(c_szFileName);	//load the bmp image
	glBindTexture(GL_TEXTURE_2D,textureID);	//and use the texture, we have just generated
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//if the texture is smaller, than the image, we get the avarege of the pixels next to it
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //same if the image bigger
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);	//we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);	//we do it for vertically and horizontally (previous line)
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->w,img->h,0,GL_RGB,GL_UNSIGNED_SHORT_5_6_5,img->pixels);	//we make the actual texture
	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	SDL_FreeSurface(img);	//we delete the image, we don't need it anymore
	fprintf(stdout, "LoadBMPTexture Loaded Texture %s, Id : %d\n", c_szFileName, textureID);
	return textureID;	//and we return the id
}

inline uint32_t LoadTexture(const char* c_szFileName)
{
	if (fopen(c_szFileName, "r"))
	{
		TraceLog("CObjectLoader::LoadTexture Loading Texture : %s", c_szFileName);
	}
	else
	{
		TraceError("CObjectLoader::LoadTexture Failed to Load Texture : %s", c_szFileName);
		return (0);
	}
	uint32_t texId = 0;
	if (GetExtension(std::string(c_szFileName)) == "png")
	{
		texId = LoadPNGTexture(c_szFileName);
	}
	else if (GetExtension(std::string(c_szFileName)) == "bmp")
	{
		texId = LoadBMPTexture(c_szFileName);
	}
	return (texId);
}

inline std::string GetExtension(const std::string& c_szFileName)			// if filename is "/idv/code/file.cpp", it returns "cpp"
{
	std::string strExtension;

	size_t nLength = c_szFileName.length();

	if (nLength > 0)
	{
		for (size_t i = nLength - 1; i > 0 && c_szFileName[i] != '/'; i--)
			if (c_szFileName[i] == '.')
			{
				strExtension = std::string(c_szFileName.c_str( ) + i + 1);
				break;
			}
	}

	return strExtension;
}

#endif //UTILS_H
