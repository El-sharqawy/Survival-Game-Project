#include "SkyBox.h"

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include "Utils.h"

CSkyBox::CSkyBox() :
    skyboxes{999}
{
    for (auto &skybox : skyboxes)
    {
        skybox = 999;
    }
    fSkyBoxSize = 0.0f;
}

CSkyBox::CSkyBox(const char* left, const char* back, const char* right, const char* front, const char* top, const char* bottom) :
    skyboxes{999}
{
    skyboxes[SKY_TOP] = LoadTexture(top);
    skyboxes[SKY_BOTTOM] = LoadTexture(bottom);
    skyboxes[SKY_LEFT] = LoadTexture(left);
    skyboxes[SKY_RIGHT] = LoadTexture(right);
    skyboxes[SKY_FRONT] = LoadTexture(front);
    skyboxes[SKY_BACK] = LoadTexture(back);
    fSkyBoxSize = 0.0f;
}

void CSkyBox::Change(const char *left, const char *back, const char *right, const char *front, const char *top, const char *bottom)
{
    if (skyboxes[0] != 999)
    {
        glDeleteTextures(6, &skyboxes[0]);
    }
    skyboxes[SKY_TOP] = LoadTexture(top);
    skyboxes[SKY_BOTTOM] = LoadTexture(bottom);
    skyboxes[SKY_LEFT] = LoadTexture(left);
    skyboxes[SKY_RIGHT] = LoadTexture(right);
    skyboxes[SKY_FRONT] = LoadTexture(front);
    skyboxes[SKY_BACK] = LoadTexture(back);
}

CSkyBox::~CSkyBox()
{
    glDeleteTextures(6, &skyboxes[0]);
}

void CSkyBox::DrawSkyBox(const float fSize)
{
    const bool b1 = glIsEnabled(GL_TEXTURE_2D); /* new, we left the textures turned on, if it was turned on */
    const bool b2 = glIsEnabled(GL_LIGHTING);

    glDepthMask(GL_FALSE);
    glDepthRange(1, 1);

    glDisable(GL_LIGHTING); /* turn of lighting when making the skybox */
    glDisable(GL_DEPTH_TEST);   /* Turn off Depth texting */
    glEnable(GL_TEXTURE_2D);    /* Turn On Texturing */
    glColor3f(1.0f, 1.0f, 1.0f);

    /* back face */
    glBindTexture(GL_TEXTURE_2D, skyboxes[SKY_BACK]); /* use the Texture we want */
    glBegin(GL_QUADS); /* and draw a face */
        glTexCoord2f(0, 0); /* use the correct texture vector 3D */
        glVertex3f(fSize / 2, fSize / 2, fSize / 2); /* and vertex */
        glTexCoord2f(1, 0); /* and repeat it */
        glVertex3f(-fSize / 2, fSize / 2, fSize / 2);
        glTexCoord2f(1, 1);
        glVertex3f(-fSize / 2, -fSize / 2, fSize / 2);
        glTexCoord2f(0, 1);
        glVertex3f(fSize / 2, -fSize / 2, fSize / 2);
    glEnd();

    /* Left face */
    glBindTexture(GL_TEXTURE_2D, skyboxes[SKY_LEFT]); /* use the Texture we want */
    glBegin(GL_QUADS); /* and draw a face */
        glTexCoord2f(0, 0); /* use the correct texture vector 3D */
        glVertex3f(-fSize / 2, fSize / 2, fSize / 2); /* and vertex */
        glTexCoord2f(1, 0); /* and repeat it */
        glVertex3f(-fSize / 2, fSize / 2, -fSize / 2);
        glTexCoord2f(1, 1);
        glVertex3f(-fSize / 2, -fSize / 2, -fSize / 2);
        glTexCoord2f(0, 1);
        glVertex3f(-fSize / 2, -fSize / 2, fSize / 2);
    glEnd();

    /* Front face */
    glBindTexture(GL_TEXTURE_2D, skyboxes[SKY_FRONT]); /* use the Texture we want */
    glBegin(GL_QUADS); /* and draw a face */
        glTexCoord2f(1, 0); /* use the correct texture vector 3D */
        glVertex3f(fSize / 2, fSize / 2, -fSize / 2); /* and vertex */
        glTexCoord2f(0, 0); /* and repeat it */
        glVertex3f(-fSize / 2, fSize / 2, -fSize / 2);
        glTexCoord2f(0, 1);
        glVertex3f(-fSize / 2, -fSize / 2, -fSize / 2);
        glTexCoord2f(1, 1);
        glVertex3f(fSize / 2, -fSize / 2, -fSize / 2);
    glEnd();

    /* Right face */
    glBindTexture(GL_TEXTURE_2D, skyboxes[SKY_RIGHT]); /* use the Texture we want */
    glBegin(GL_QUADS); /* and draw a face */
        glTexCoord2f(0, 0); /* use the correct texture vector 3D */
        glVertex3f(fSize / 2, fSize / 2, -fSize / 2); /* and vertex */
        glTexCoord2f(1, 0); /* and repeat it */
        glVertex3f(fSize / 2, fSize / 2, fSize / 2);
        glTexCoord2f(1, 1);
        glVertex3f(fSize / 2, -fSize / 2, fSize / 2);
        glTexCoord2f(0, 1);
        glVertex3f(fSize / 2, -fSize / 2, -fSize / 2);
    glEnd();

    /* Top face */
    glBindTexture(GL_TEXTURE_2D, skyboxes[SKY_TOP]); /* use the Texture we want */
    glBegin(GL_QUADS); /* and draw a face */
        glTexCoord2f(1, 0); /* use the correct texture vector 3D */
        glVertex3f(fSize / 2, fSize / 2, fSize / 2); /* and vertex */
        glTexCoord2f(0, 0); /* and repeat it */
        glVertex3f(-fSize / 2, fSize / 2, fSize / 2);
        glTexCoord2f(0, 1);
        glVertex3f(-fSize / 2, fSize / 2, -fSize / 2);
        glTexCoord2f(1, 1);
        glVertex3f(fSize / 2, fSize / 2, -fSize / 2);
    glEnd();

    /* Bottom face */
    glBindTexture(GL_TEXTURE_2D, skyboxes[SKY_BOTTOM]); /* use the Texture we want */
    glBegin(GL_QUADS); /* and draw a face */
        glTexCoord2f(1, 1); /* use the correct texture vector 3D */
        glVertex3f(fSize / 2, -fSize / 2, fSize / 2); /* and vertex */
        glTexCoord2f(0, 1); /* and repeat it */
        glVertex3f(-fSize / 2, -fSize / 2, fSize / 2);
        glTexCoord2f(0, 0);
        glVertex3f(-fSize / 2, -fSize / 2, -fSize / 2);
        glTexCoord2f(1, 0);
        glVertex3f(fSize / 2, -fSize / 2, -fSize / 2);
    glEnd();

    /* Turn everything back, which we turned on and turn everything off which we have turned on. */
    glEnable(GL_DEPTH_TEST);
    glDepthRange(0, 1);
    glDepthMask(GL_TRUE);

    if (b1 == false)
    {
        glDisable(GL_TEXTURE_2D);
    }
    if (b2 == true)
    {
        glEnable(GL_LIGHTING);
    }
    fSkyBoxSize = fSize;
}
