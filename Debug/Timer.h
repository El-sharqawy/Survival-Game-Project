//
// Created by osama on 24/05/24.
//

#ifndef TIMER_H
#define TIMER_H

#include <cstdint>
#include "Singleton.h"

class CTimer
{
public:
	CTimer();
	~CTimer() = default;

	void Advance();
	void Adjust(int32_t iTimeGap);
	void SetTimeBase();

	[[nodiscard]]float GetCurrentSecond() const;
	[[nodiscard]]uint32_t GetCurrnetMilliSecond() const;

	[[nodiscard]]float GetElapsedtSecond() const;
	[[nodiscard]]uint32_t GetElapsedMilliSecond() const;

	void UseCustomTime();

protected:
	bool m_bUseRealTime;
	uint32_t m_uiBaseTime;
	uint32_t m_uiCurrentTime;
	float m_fCurrentTime;
	uint32_t m_uiElapsedTime;
	int32_t m_iIndex;
};

bool ELTimer_Init();
uint32_t ELTimer_GetMSec();

void ELTimer_SetServerMSec(uint32_t m_uiServerTime);
uint32_t ELTimer_GetServerMSec();

void ELTimer_SetFrameMSec();
uint32_t ELTimer_GetFrameMSec();

#endif //TIMER_H
