//
// Created by osama on 14/07/24.
//

#ifndef MENU_H
#define MENU_H

#include "../BaseLib/Collision.h"
#include <cstdint>
#include <vector>
#include <string>

#include "../BaseLib/Text.h"
#include <SDL2/SDL.h>

class CMenu
{
public:
	CMenu(uint32_t uiBack, const std::vector<CCollisionPlane> &colPlanes, const std::vector<std::string> &stText,
	const CCollisionPlane& backPos, CText *text, SDL_Window *hWindow);
	~CMenu();

	int32_t Show();
	void Destroy();

private:
	uint32_t m_uiBackGround;
	std::vector<CCollisionPlane> m_vColPlanes;
	std::vector<std::string> m_sStrings;
	CText* m_tText;
	CCollisionPlane* m_cBackPos;
	int32_t m_iCurSelection;
	SDL_Window *m_hWindow;
};



#endif //MENU_H
