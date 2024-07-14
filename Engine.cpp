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
#include "BaseLib/Random.h"
#include "BaseLib/Utils.h"
SDL_Window* CEngine::m_hWindow = nullptr;
SDL_GLContext CEngine::m_glContext = nullptr;

CEngine::CEngine()
{
    m_iWidth = WINDOW_WIDTH;
    m_iHeight = WINDOW_HEIGHT;
    m_stAppTitle = APP_TITLE;
	m_pPlayer = nullptr;
	m_pText = nullptr;
	m_bIsFiring = false;
	m_bIsRunning= true;
	m_bIsOnMainMenu = true;
	m_Music = nullptr;
	m_pZombie = nullptr;
	m_pKnife = nullptr;
	m_pGameMenu = nullptr;
	m_pMenuText = nullptr;
	m_pItemObject = nullptr;
}

bool CEngine::InitializeOpenGL() const
{
    GLenum error = GL_NO_ERROR;
    //Initialize Projection Matrix
	glClearColor(0.0, 0.0, 0.0, 0.0);
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

	return (true);
}

bool CEngine::InitializeWindow(const std::string &title, const int32_t iWidth, const int32_t iHeight, const bool bIsFullScreen)
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
        m_hWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, iWidth, iHeight,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    }
    else
    {
        m_hWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, iWidth, iHeight,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    }
    if (!m_hWindow)
    {
        TraceError("Engine::InitializeWindow Failed to Initialize Window ! Error : %s", SDL_GetError());
        return (false);
    }

    m_glContext = SDL_GL_CreateContext(m_hWindow);
    if(m_glContext == nullptr)
    {
        TraceError( "Engine::InitializeWindow OpenGL context could not be created! Error: %s", SDL_GetError());
        return (false);
    }

    //Use Vsync
    if(SDL_GL_SetSwapInterval(1) < 0)
    {
        TraceLog( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }

    m_stAppTitle = title;
    m_iWidth = iWidth;
    m_iHeight = iHeight;
    return (true);
}

void CEngine::InitializeWorld()
{
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

	std::ifstream in("weapons.dat");
	char tmp[10][300];
	int tmparr[10];
	float floatarr[12];
	while(!in.eof())
	{
		in.getline(tmp[0],300);
		if(tmp[0][0] == '\0')
			break;
		sscanf(tmp[0],"%s %s %d %d %d %f %f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %d %d %d %s %s %s %s %s",tmp[1],tmp[2],&tmparr[2],&tmparr[0],&tmparr[1],&floatarr[0],&floatarr[1],&floatarr[2],&floatarr[3],&floatarr[4],&floatarr[5],&floatarr[6],&floatarr[7],&floatarr[8],&floatarr[9],&floatarr[10],&floatarr[11],&tmparr[3],&tmparr[4],&tmparr[5],&tmparr[6],&tmparr[7],&tmparr[8],&tmparr[9],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7]);
		TraceLog("%f-%f-%f", floatarr[6],floatarr[7],floatarr[8]);
		std::vector<uint32_t> weaponAnims;
		LoadAnimations(weaponAnims,tmp[2],tmparr[0]+tmparr[1]+tmparr[2]);
		Mix_Chunk* fireSound = Mix_LoadWAV(tmp[3]);
		Mix_Chunk* reloadSound = Mix_LoadWAV(tmp[4]);
		Mix_Chunk* normalSound = Mix_LoadWAV(tmp[5]);
		Mix_Chunk* emptySound = Mix_LoadWAV(tmp[6]);
		m_vMyWeapons.emplace_back(weaponAnims,tmparr[2],tmparr[0],tmparr[1],fireSound,reloadSound,normalSound,emptySound,CVector3D(floatarr[0],floatarr[1],floatarr[2]),CVector3D(floatarr[3],floatarr[4],floatarr[5]),CVector3D(floatarr[6],floatarr[7],floatarr[8]),CVector3D(floatarr[9],floatarr[10],floatarr[11]),tmparr[3],tmparr[4],tmparr[5],tmparr[6],tmparr[7], tmparr[8], tmparr[9], tmp[1],m_ObjLoader.Load(tmp[7],nullptr));
	}
	in.close();

	//m_vMyWeapons[0].SetStrength(35);
	//m_vMyWeapons[1].SetStrength(50);
	m_pPlayer = new CPlayer("Osama", CCollisionSphere(CVector3D(0, 10.0, 0), 3.0), 0.2, 1.5,  0.2, &m_vMyWeapons[0]);
	m_pPlayer->SetWindow(GetWindow());
	m_pPlayer->SetPlayerHealth(100.0f);

	char temp[10][300];
	int tempArr[10];
	in.open("spawn.dat");
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

	uint32_t uMesh = m_ObjLoader.Load(temp[7], &colPlanes);
	m_vMyMaps.emplace_back(uMesh, colPlanes, temp[0], skyB, spawnPoints);
	in.close();
	in.open("zombie.dat");
	std::vector<uint32_t> frames;
	uint32_t a,b,c2;
	in.getline(tmp[9],300);
	sscanf(tmp[9],"%s %d %d %d",tmp[0],&a,&b,&c2);
	LoadAnimations(frames,std::string(tmp[0]),a+b+c2);
	m_pZombie = new CZombie(frames,a,b,c2,100,20,0.3,CCollisionSphere(*m_vMyMaps[0].GetRandomSpawnPoints(),1),CVector3D(0,0,0),m_pPlayer->GetPlayerCamera()->GetCameraLocation());

	std::vector<uint32_t> vCharacters;
	for(int32_t i = 0; i < 94; i++)
	{
		char c_szTemp[50];
		sprintf(c_szTemp,"font/%d.obj",i);
		uint32_t TempObj = m_ObjLoader.Load(c_szTemp,  nullptr);
		vCharacters.push_back(TempObj);
	}

	m_pText = new CText(vCharacters, 0.8f, 0.8f);
	m_pMenuText = new CText(vCharacters, 0.8f, 0.8f);

	std::vector<uint32_t> vFrames;
	LoadAnimations(vFrames, "data/knife/knife", 5);
	Mix_Chunk* HitSound = LoadSound("data/hit.wav");
	Mix_Chunk* NormSound = LoadSound("data/nhit.wav");

	m_pKnife = new CKnife(vFrames, 1000, 30, 30, CVector3D(-0.5f, -0.6f, -1.3f), HitSound, NormSound);

	int32_t mod = m_ObjLoader.Load("data/health.obj", nullptr);
	m_vModels.emplace_back(mod);
	mod = m_ObjLoader.Load("data/allammo.obj", nullptr);
	m_vModels.emplace_back(mod);

	uint32_t tex = LoadTexture("data/background.png");
	std::vector<CCollisionPlane> clPlanes;
	clPlanes.emplace_back(0,0,1,-0.13,0.16,-1,-0.13,0.16,-1,-0.13,0.16,-1,-0.13,0.16,-1);
	clPlanes.emplace_back(0,0,1,-0.13,0.08,-1,-0.13,0.16,-1,-0.08,0.16,-1,-0.13,0.16,-1);
	std::vector<std::string> strings;
	strings.emplace_back("Play");
	strings.emplace_back("Exit");
	CCollisionPlane xyz(0,0,1,-1.33,1,-2.4,-1.33,-1,-2.4,1.33,-1,-2.4,1.33,1,-2.4);

	m_pGameMenu = new CMenu(tex, clPlanes, strings, xyz, m_pMenuText, GetWindow());
	m_Music = Mix_LoadMUS("data/hive.wav");
	Mix_PlayMusic(m_Music,-1);

	m_pItemObject = new CItem();
}

void CEngine::ShowMenu()
{
	if (m_pGameMenu == nullptr)
	{
		return;
	}

	const int32_t iMenu = m_pGameMenu->Show();
	m_bIsOnMainMenu = true;
	switch(iMenu)
	{
		case 0:
			m_bIsOnMainMenu = false;
		break;
		case 1:
		{
			m_bIsRunning = false;
		}
		break;
		default:
			break;
	}
}

void CEngine::UpdateEngine()
{
	m_pPlayer->UpdatePlayer(m_vMyMaps[0].GetCollisionPlanes());

	if (m_pZombie->Update(m_vMyMaps[0].GetCollisionPlanes(), GetPlayer()->GetPlayerCamera()->GetCameraLocation()))
	{
		const float drop = frandom(1,4);
		if (drop == 1)
		{
			m_pItemObject->Add(0, m_vModels[0], CCollisionSphere(m_pZombie->GetCollisionSphere()->GetCenter(), 2.0f));
		}
		else if (drop == 2)
		{
			m_pItemObject->Add(1, m_vModels[1], CCollisionSphere(m_pZombie->GetCollisionSphere()->GetCenter(), 2.0f));
		}
		else if (drop >= 3)
		{
			m_pItemObject->Add(static_cast<int32_t>(GetPlayer()->GetWeaponsNumber() + 10), m_vMyWeapons[GetPlayer()->GetWeaponsNumber()].GetOuterView(),
				CCollisionSphere(m_pZombie->GetCollisionSphere()->GetCenter(), 2.0f));
		}

		delete m_pZombie;
		m_pZombie = new CZombie(100, 0.3, 10, CCollisionSphere(*m_vMyMaps[0].GetRandomSpawnPoints(), 1.0f), CVector3D(0.0f, 0.0f, 0.0f), GetPlayer()->GetPlayerCamera()->GetCameraLocation());
		GetPlayer()->AddPoints(10);
	}
	if(m_pZombie->SetAttack(m_pPlayer->GetCollisionSphere()))
	{
		m_pPlayer->SetPlayerHealth(m_pPlayer->GetPlayerHealth() - static_cast<float>(m_pZombie->GetStrength()));
	}

	if (m_bIsFiring)
	{
		const auto CamDirection = m_pPlayer->GetPlayerCamera()->GetVector();
		CVector3D Direction;
		const bool bIsShooting = m_pPlayer->GetCurrentWeapon()->Fire(Direction, CamDirection);
		Direction.Normalize();
		if (bIsShooting)
		{
			if (CCollision::RaySphere(m_pZombie->GetCollisionSphere()->GetCenter().GetX(), m_pZombie->GetCollisionSphere()->GetCenter().GetY(), m_pZombie->GetCollisionSphere()->GetCenter().GetZ(),
			Direction.GetX(), Direction.GetY(), Direction.GetZ(), m_pPlayer->GetPlayerCamera()->GetCameraLocation().GetX(), m_pPlayer->GetPlayerCamera()->GetCameraLocation().GetY(),
			m_pPlayer->GetPlayerCamera()->GetCameraLocation().GetZ(), 5.0))
			{
				m_pZombie->TakeDamage(GetWeapon()->GetWeaponStrength());
			}
		}
	}
	else
	{
		m_pPlayer->GetCurrentWeapon()->StopFire();
	}

	if (GetPlayer()->GetPlayerHealth() <= 0.0f)
	{
		m_bIsRunning = false;
	}

	if (GetPlayer()->GetPlayerHealth() <= 20)
	{
		glColorMask(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glColorMask(1.0f, 0.0f,
			0.0f, 1.0f);
	}

	m_pKnife->Update(GetPlayer());

	int32_t itemDrop = m_pItemObject->Update(GetPlayer()->GetCollisionSphere());
	switch (itemDrop)
	{
		case -1:
		{
			break;
		}
		case 0:
		{
			GetWeapon()->AddBullets(300);
		}
		break;
		case 1:
		{
			GetPlayer()->SetPlayerHealth(100);
		}
		break;
		case 10:
		{
			GetPlayer()->AddWeapon(&m_vMyWeapons[0]);
		}
		break;
		case 11:
		{
			GetPlayer()->AddWeapon(&m_vMyWeapons[1]);
		}
		break;
		case 12:
		{
			GetPlayer()->AddWeapon(&m_vMyWeapons[2]);
		}
		break;
		default:
			break;
	}

}

void CEngine::Show() const
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //Render Triangle
    glLoadIdentity();
	m_pPlayer->ShowPlayer();
    m_pPlayer->GetPlayerCamera()->Control();
	for (const auto& map : m_vMyMaps)
	{
		map.GetSkyBox()->DrawSkyBox(1000.0f);
		map.Show();
	}
	m_pZombie->Show();
    m_pPlayer->GetPlayerCamera()->UpdateCamera();
	m_pItemObject->Show();
	glLoadIdentity();
	m_pPlayer->ShowPlayer();
	glClear(GL_DEPTH_BUFFER_BIT);
	DrawPlayerStatus(GetPlayer()->GetPlayerHealth(), GetWeapon()->GetCurrentAmmo(), GetWeapon()->GetAllAmmo(), GetPlayer()->GetPoints(), GetWeapon()->GetName());
	m_pKnife->Show();
}

void CEngine::ChangeSkyBox(int32_t skyNum) const
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
		m_vMyMaps[0].GetSkyBox()->Change(temp[0], temp[1] ,temp[2] ,temp[3] ,temp[4], temp[5]);
		m_vMyMaps[0].Show();
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
		m_vMyMaps[0].GetSkyBox()->Change(temp[0], temp[1] ,temp[2] ,temp[3] ,temp[4], temp[5]);
		m_vMyMaps[0].Show();
		in.close();
	}
}

bool CEngine::Start()
{
	SDL_Event event {};
	while (m_bIsRunning != false)
	{
	    const uint32_t start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					m_bIsRunning = false;
					return false;
				}
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
						{
							const int32_t iMenu = m_pGameMenu->Show();
							m_bIsOnMainMenu = true;
							switch(iMenu)
							{
								case 0:
									m_bIsOnMainMenu = false;
									break;
								case 1:
								{
									m_bIsRunning = false;
								}
								break;
								default:
									break;
							}
						}
						break;
						case SDLK_q:
						{
							m_pPlayer->GetPlayerCamera()->SetMouseOut();
							SDL_ShowCursor(true);
						}
						break;
						case SDLK_t:
						{
							m_pPlayer->GetPlayerCamera()->SetWindow(GetWindow());
							m_pPlayer->GetPlayerCamera()->SetMouseIn();
							SDL_ShowCursor(false);
						}
						break;
						case SDLK_w:
						{
							m_pPlayer->MoveForward(true);
						}
						break;
						case SDLK_s:
						{
							m_pPlayer->MoveForward(false);
						}
						break;
						case SDLK_a:
						{
							m_pPlayer->MoveRight(true);
						}
						break;
						case SDLK_d:
						{
							m_pPlayer->MoveRight(false);
						}
						break;
						case SDLK_LSHIFT:
						{
							m_pPlayer->Sprint(true);
						}
						break;
						case SDLK_SPACE:
						{
							if (m_pPlayer->IsJumping() == false)
							{
								m_pPlayer->Jump();
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
						case SDLK_1:
						{
							m_pPlayer->ChangeWeapon(0);
						}
						break;
						case SDLK_2:
						{
							m_pPlayer->ChangeWeapon(1);
						}
						break;
						case SDLK_r:
						{
							m_pPlayer->GetCurrentWeapon()->Reload();
						}
						break;
						case SDLK_e:
						{
							if (m_pKnife->SetKnife(GetPlayer(), m_pZombie) != -1)
							{
								m_pZombie->TakeDamage(m_pKnife->GetStrength());
							}
						}
						break;
						default:
							continue;
					}
				}
				break;
				case SDL_KEYUP:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_LSHIFT:
						{
							m_pPlayer->Sprint(false);
						}
						default:
							continue;
					}
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					if(!m_pPlayer->GetPlayerCamera()->IsMouseIn())
					{
						m_pPlayer->GetPlayerCamera()->SetMouseIn();
					}
					else
					{
						switch(event.button.button)
						{
							case SDL_BUTTON_LEFT:
							{
								if(!m_pPlayer->GetPlayerCamera()->IsMouseIn())
								{
									m_pPlayer->GetPlayerCamera()->SetMouseIn();
								}
								else
								{
									m_pPlayer->Sprint(false);
									m_bIsFiring = true;
								}
							}
							break;
							case SDL_BUTTON_RIGHT:
							{
								m_pPlayer->GetCurrentWeapon()->Aim();
							}
							break;
							default:
								continue;
						}
						break;
					}
				}
				break;
				case SDL_MOUSEBUTTONUP:
				{
					switch(event.button.button)
					{
						case SDL_BUTTON_LEFT:
						{
							m_bIsFiring = false;
						}
						break;
						default:
							continue;
					}
				}
				break;
				default:
					continue;
			}
		}

		if (!m_bIsOnMainMenu)
		{
			UpdateEngine();
		}

        Show();

	    SDL_GL_SwapWindow(GetWindow());
	    if (1000/30 > (SDL_GetTicks() - start))
	    {
	        SDL_Delay(1000 / 30 - (SDL_GetTicks() - start));
	    }
	}
    return (true);
}

void CEngine::LoadAnimations(std::vector<uint32_t> &framesID, const std::string &fileName, const uint32_t uiNum)
{
    char frame[300] = {0};  // Buffer to store frame numbers as strings, initialized to 0
    char tmp[300] = {0};    // Temporary buffer, initialized to 0

	for(int32_t i = 1; i <= uiNum; i++)
	{
		std::string s(fileName + '_');
		sprintf(frame,"%d",i);
		const size_t len = strlen(frame);

		for(size_t j=0;j<len;j++)
		{
			tmp[j] = frame[j];
		}
		for(size_t j = 0; j < 6 - len; j++)
		{
			frame[j]='0';
		}

		for(size_t j = 6 - len; j < 6; j++)
		{
			frame[j]=tmp[j-6+len];
		}

		frame[6] = 0;
		s += frame;
		s += ".obj";
		framesID.emplace_back(m_ObjLoader.Load(s,nullptr));
	}
}

void CEngine::DrawPlayerStatus(const float fHealth, const uint32_t iAmmo, const uint32_t iAllAmmo, const int32_t iPoints, const std::string& stWeaponName) const
{
	char c_szTemp[200];
	sprintf(c_szTemp, "Health: %.1f", fHealth);
	if (fHealth > 70)
	{
		m_pText->RenderText(CVector3D(-0.5f, 0.35f, -1.0f), CVector3D(0.0f, 0.0f, 0.0f), CVector3D(0.020f, 0.020f, 0.020f), c_szTemp, CVector3D(0.0f, 1.0f, 0.0f));
	}
	else if (fHealth <= 70 && fHealth >= 50)
	{
		m_pText->RenderText(CVector3D(-0.5f, 0.35f, -1.0f), CVector3D(0.0f, 0.0f, 0.0f), CVector3D(0.020f, 0.020f, 0.020f), c_szTemp);
	}
	else
	{
		m_pText->RenderText(CVector3D(-0.5f, 0.35f, -1.0f), CVector3D(0.0f, 0.0f, 0.0f), CVector3D(0.020f, 0.020f, 0.020f), c_szTemp, CVector3D(1.0f, 0.0f, 0.0f));
	}
	sprintf(c_szTemp, "%s	-	(%d / %d)", stWeaponName.c_str(), iAmmo, iAllAmmo);
	m_pText->RenderText(CVector3D(-0.54, -0.39, -1), CVector3D(0.0f, 0.0f, 0.0f), CVector3D(0.020f, 0.020f, 0.020f), c_szTemp);
	sprintf(c_szTemp, "Points: %d", iPoints);
	m_pText->RenderText(CVector3D(0.22f, 0.35f, -1.0f), CVector3D(0.0f, 0.0f, 0.0f), CVector3D(0.020f, 0.020f, 0.020f), c_szTemp);
	sprintf(c_szTemp, "+");
	m_pText->RenderText(CVector3D(0.0f, 0.0f, -1.0f), CVector3D(0.0f, 0.0f, 0.0f), CVector3D(0.035f, 0.035f, 0.035f), c_szTemp);
	sprintf(c_szTemp, "Zombie Health: %d", m_pZombie->GetHealth());
	m_pText->RenderText(CVector3D(0.22f, -0.39f, -1.0f), CVector3D(0.0f, 0.0f, 0.0f), CVector3D(0.020f, 0.020f, 0.020f), c_szTemp);
}

Mix_Chunk *CEngine::LoadSound(const char *c_szFileName)
{
	Mix_Chunk *sound = Mix_LoadWAV(c_szFileName);
	if (!sound)
	{
		TraceError("CEngine::LoadSound Failed to Load Sound %s", c_szFileName);
		return (nullptr);
	}
	m_vSounds.emplace_back(sound);
	return (sound);
}

CEngine::~CEngine()
{
	TraceLog("Destroying Engine..");
    Destroy();
}

void CEngine::Destroy()
{
    SDL_GL_DeleteContext(m_glContext);
    m_glContext = nullptr;
    SDL_DestroyWindow(m_hWindow);
    m_hWindow = nullptr;
	delete m_pText;
	m_pText = nullptr;
	delete m_pMenuText;
	m_pMenuText = nullptr;
	delete m_pPlayer;
	m_pPlayer = nullptr;
	delete m_pZombie;
	m_pZombie = nullptr;
	Mix_FreeMusic(m_Music);
	for (const auto& chunk : m_vSounds)
	{
		Mix_FreeChunk(chunk);
	}
	Mix_CloseAudio();
	IMG_Quit();
    SDL_Quit();
}