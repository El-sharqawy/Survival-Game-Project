//
// Created by osama on 24/05/24.
//

#include "Timer.h"

#include <ctime>
#include <cstdint>
#include <SDL.h>

static uint32_t gs_uiCurTime = 0;
static uint32_t gs_uiServerTime = 0;
static uint32_t gs_uiClientTime = 0;
static uint32_t gs_uiFrameTime = 0;

bool ELTimer_Init()
{
    const uint32_t currentTime = SDL_GetTicks();
	gs_uiCurTime = currentTime;
	return (true);
}

uint32_t ELTimer_GetMSec()
{
    const uint32_t currentTime = SDL_GetTicks();
	return (currentTime - gs_uiCurTime);
}

void ELTimer_SetServerMSec(const uint32_t m_uiServerTime)
{
	if (m_uiServerTime != 0)
	{
		gs_uiServerTime = m_uiServerTime;
		//gs_uiClientTime = CTimer::Instance().GetCurrnetMilliSecond();
	}
}

uint32_t ELTimer_GetServerMSec()
{
	//return (CTimer::Instance().GetCurrnetMilliSecond() - gs_uiServerTime + gs_uiClientTime);
	return (0);
}

void ELTimer_SetFrameMSec()
{
	gs_uiFrameTime = ELTimer_GetMSec();
}

uint32_t ELTimer_GetFrameMSec()
{
	return (gs_uiFrameTime);
}

CTimer::CTimer()
{
	ELTimer_Init();

	if (this)
	{
		m_uiCurrentTime = 0;
		m_bUseRealTime = true;
		m_uiBaseTime= 0;
		m_iIndex = 0;
		m_uiElapsedTime = 0;
		m_fCurrentTime = 0.0f;
	}
}

void CTimer::Advance()
{
	if (!m_bUseRealTime)
	{
		m_iIndex++;

		if (m_iIndex == 1)
			m_iIndex = -1;

		m_uiCurrentTime += 16 + (m_iIndex & 1);
		m_fCurrentTime = static_cast<float>(m_uiCurrentTime) / 1000.f;
	}
	else
	{
		const uint32_t uiCurTime = ELTimer_GetMSec();

		if (m_uiCurrentTime == 0)
		{
			m_uiCurrentTime = uiCurTime;
		}

		m_uiElapsedTime = uiCurTime - m_uiCurrentTime;
		m_uiCurrentTime = uiCurTime;
	}
}

void CTimer::Adjust(const int32_t iTimeGap)
{
	m_uiCurrentTime += iTimeGap;
}

void CTimer::SetTimeBase()
{
	m_uiCurrentTime = 0;
}

float CTimer::GetCurrentSecond() const
{
	if (m_bUseRealTime)
	{
		return (static_cast<float>(ELTimer_GetMSec()) / 1000.f);
	}

	return m_fCurrentTime;
}

uint32_t CTimer::GetCurrnetMilliSecond() const
{
	if (m_bUseRealTime)
	{
		return (ELTimer_GetMSec());
	}

	return (m_uiCurrentTime);
}

float CTimer::GetElapsedtSecond() const
{
	return (static_cast<float>(GetElapsedMilliSecond()) / 1000.f);
}

uint32_t CTimer::GetElapsedMilliSecond() const
{
	if (m_bUseRealTime)
	{
		return (16 + (m_iIndex & 1));
	}

	return (m_uiElapsedTime);
}

void CTimer::UseCustomTime()
{
	m_bUseRealTime = false;
}