#pragma once

#include <Windows.h>

class TimeManager
{
public:
	TimeManager(void);


	bool init ();
	bool Frame ();
	INT64 FrameTime();
	INT64 CurrentTime();

	~TimeManager(void);

private:
	INT64 m_currTime, m_currFrameTime, m_lastFrameTime, m_appTime, m_TicksPerMs;
};

