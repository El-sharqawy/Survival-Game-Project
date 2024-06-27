#include "Engine.h"
#include "game.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL_opengl.h>
#include "Debug/Log.h"
#include "BaseLib/Camera.h"
#include <fstream>
#include <bits/unique_ptr.h>
#include <cstdlib> // For strtol
#include "Debug/Timer.h"
#include <SDL2/SDL_image.h>
#include "BaseLib/Utils.h"

SDL_Window* CEngine::window = nullptr;
SDL_Renderer* CEngine::renderer = nullptr;
SDL_Texture* CEngine::texture = nullptr;
SDL_GLContext CEngine::glContext = nullptr;
std::vector<CMap*> CEngine::myMaps;

CEngine::CEngine()
{
    width = WINDOW_WIDTH;
    height = WINDOW_HEIGHT;
    appTitle = APP_TITLE;
	Player = new CPlayer("Osama", CCollisionSphere(CVector3D(0, 10.0, 0), 3.0), 0.2, 1.5,  0.2);
}

bool CEngine::InitializeOpenGL()
{
    GLenum error = GL_NO_ERROR;
    //Initialize Projection Matrix
	glClearColor(0.5,0.5,0.5,1.0);
	glClearDepth(1.0);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	/* fovy = Zoom In, zFar = Max View Point */
	//gluPerspective(45, 640.0 / 480.0, 1.0, 500.0);
	gluPerspective(45, 640.0 / 480.0, 1.0, 1500.0);

	//Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    //Initialize clear color
    glViewport(0, 0, GetWidth(), GetHeight());
    glEnable(GL_TEXTURE_2D);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        TraceError( "Error initializing OpenGL! %s\n", gluErrorString(error));
        return (false);
    }

    char temp[10][300];
    int tempArr[10];

    std::ifstream in("spawn.dat");
    std::vector<CVector3D> spawnPoints;
    while (!in.eof())
    {
        in.getline(temp[0], 300);
    	char *endPtr;
    	tempArr[0] = static_cast<int>(strtol(temp[0], &endPtr, 10));
    	tempArr[1] = static_cast<int>(strtol(endPtr, &endPtr, 10));
    	tempArr[2] = static_cast<int>(strtol(endPtr, &endPtr, 10));
        spawnPoints.emplace_back(static_cast<float>(tempArr[0]),
            static_cast<float>(tempArr[1]),
            static_cast<float>(tempArr[2]));
    }
    in.close();
    in.open("game.dat");
    std::vector<std::string> skyB;
    std::vector<CCollisionPlane> colPlanes;

    in.getline(temp[9], 300);
    sscanf(temp[9], "%s %s %s %s %s %s %s %s", temp[0], temp[1] ,temp[2] ,temp[3] ,temp[4], temp[5], temp[6], temp[7]);
    for (int32_t i = 1; i < 7; i++)
    {
        TraceLog(temp[i]);
    }
    for (int32_t i = 1; i < 7; i++)
    {
        skyB.emplace_back(temp[i]);
    }

    uint32_t uMesh = objLoader.Load(temp[7], &colPlanes);
    myMaps.emplace_back(new CMap(uMesh, colPlanes, temp[0], skyB, spawnPoints));
    return (true);
}

bool CEngine::InitializeWindow(const std::string &title, const int32_t iWidth, const int32_t iHeight, bool bIsFullScreen)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        TraceError("Engine::InitializeWindow Failed to Initialize SDL Library ! Error : %s", SDL_GetError());
        return (false);
    }

	constexpr int32_t flags = IMG_INIT_JPG | IMG_INIT_PNG;
	const int32_t initted = IMG_Init(flags);
	if ((initted & flags) != flags)
		{
		TraceError("IMG_Init: Failed to init required jpg and png support!\n");
		TraceError("IMG_Init: %s\n", IMG_GetError());
		// handle error
	}

    // SETUP OPENGL SETTINGS
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    if (bIsFullScreen)
    {
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, iWidth, iHeight,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    }
    else
    {
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, iWidth, iHeight,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    }
    if (!window)
    {
        TraceError("Engine::InitializeWindow Failed to Initialize Window ! Error : %s", SDL_GetError());
        return (false);
    }

    glContext = SDL_GL_CreateContext(window);
    if(glContext == nullptr)
    {
        TraceError( "Engine::InitializeWindow OpenGL context could not be created! Error: %s", SDL_GetError());
        return (false);
    }

    //Use Vsync
    if(SDL_GL_SetSwapInterval(1) < 0)
    {
        TraceLog( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }

    appTitle = title;
    width = iWidth;
    height = iHeight;
	Player->SetWindow(GetWindow());
    return (true);
}

void CEngine::UpdateEngine() const
{
	Player->UpdatePlayer(myMaps[0]->GetCollisionPlanes());
}

void CEngine::Show() const
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //Render Triangle
    glLoadIdentity();
    Player->GetPlayerCamera()->Control();
	for (const auto& map : myMaps)
	{
		map->GetSkyBox()->DrawSkyBox(1000.0f);
		map->Show();
	}
    Player->GetPlayerCamera()->UpdateCamera();
	glLoadIdentity();
	glClear(GL_DEPTH_BUFFER_BIT);
}

void CEngine::ChangeSkyBox(int32_t skyNum)
{
	if (skyNum == 0)
	{
		std::ifstream in("skybox1.dat");
		std::vector<std::string> skyB;
		std::vector<CCollisionPlane> colPlanes;

		char temp[10][300];
		in.getline(temp[7], 300);
		sscanf(temp[7], "%s %s %s %s %s %s", temp[0], temp[1] ,temp[2] ,temp[3] ,temp[4], temp[5]);
		for (int32_t i = 1; i < 7; i++)
		{
			TraceLog(temp[i]);
		}
		for (int32_t i = 1; i < 7; i++)
		{
			skyB.emplace_back(temp[i]);
		}
		myMaps[0]->GetSkyBox()->Change(temp[0], temp[1] ,temp[2] ,temp[3] ,temp[4], temp[5]);
		myMaps[0]->Show();
		in.close();
	}
	else if (skyNum == 1)
	{
		std::ifstream in("skybox2.dat");
		std::vector<std::string> skyB;
		std::vector<CCollisionPlane> colPlanes;

		char temp[10][300];
		in.getline(temp[7], 300);
		sscanf(temp[7], "%s %s %s %s %s %s", temp[0], temp[1] ,temp[2] ,temp[3] ,temp[4], temp[5]);
		for (int32_t i = 1; i < 7; i++)
		{
			TraceLog(temp[i]);
		}
		for (int32_t i = 1; i < 7; i++)
		{
			skyB.emplace_back(temp[i]);
		}
		myMaps[0]->GetSkyBox()->Change(temp[0], temp[1] ,temp[2] ,temp[3] ,temp[4], temp[5]);
		myMaps[0]->Show();
		in.close();
	}
}

bool CEngine::Start()
{
	SDL_Event event {};
	const bool bQuit = event.type == SDL_QUIT;
	while (!bQuit)
	{
	    const uint32_t start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
			    return false;
			}
			if (event.type == SDL_KEYDOWN)
			{
			    switch (event.key.keysym.sym)
			    {
			    	case SDLK_q:
			        {
			            Player->GetPlayerCamera()->SetMouseOut();
			            SDL_ShowCursor(true);
			        }
			    	break;
			    	case SDLK_t:
			    	{
			    		Player->GetPlayerCamera()->SetWindow(GetWindow());
			    		Player->GetPlayerCamera()->SetMouseIn();
			    		SDL_ShowCursor(false);
			    	}
			    	break;
			    	case SDLK_w:
			    	{
						Player->MoveForward(true);
			    	}
			    	break;
			    	case SDLK_s:
			    	{
			    		Player->MoveForward(false);
			    	}
			    	break;
			    	case SDLK_a:
			    	{
			    		Player->MoveRight(true);
			    	}
			    	break;
			    	case SDLK_d:
			    	{
			    		Player->MoveRight(false);
			    	}
			    	break;
			    	case SDLK_LSHIFT:
			    	{
			    		Player->Sprint(true);
			    	}
			    	break;
			    	case SDLK_SPACE:
			    	{
			    		if (Player->IsJumping() == false)
			    		{
			    			Player->Jump();
			    		}
			    	}
			    	break;
			    	case SDLK_F1:
			    	{
			    		ChangeSkyBox(0);
			    	}
			    	break;
			    	case SDLK_F2:
			    	{
			    		ChangeSkyBox(1);
			    	}
			    	break;
			    	default:
			    		continue;
			    }

			}
			else if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_LSHIFT:
					{
						Player->Sprint(false);
					}
					default:
						continue;
				}
			}
		}
		UpdateEngine();
        Show();

	    SDL_GL_SwapWindow(GetWindow());
	    if (1000/30 > (SDL_GetTicks() - start))
	    {
	        SDL_Delay(1000 / 30 - (SDL_GetTicks() - start));
	    }
	}
    return (true);
}

CEngine::~CEngine()
{
    Destroy();
}

void CEngine::Destroy()
{
	for (auto &map : myMaps)
	{
		delete map;
		map = nullptr;
	}
    SDL_DestroyTexture(texture);
    texture = nullptr;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_GL_DeleteContext(glContext);
    glContext = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
	IMG_Quit();
    SDL_Quit();
    TraceLog("Destroying Engine..");
}