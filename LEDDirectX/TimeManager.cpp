#include "TimeManager.h"


TimeManager::TimeManager(void)
{
	m_currTime = 0;
	m_lastFrameTime = -1;
}

bool TimeManager::init ()
{
	INT64 frequency;

	QueryPerformanceFrequency((LARGE_INTEGER*) &frequency);

	if (!frequency)
		return false;

	m_TicksPerMs = frequency / 1000;

	QueryPerformanceCounter((LARGE_INTEGER*) &m_currTime);
	m_appTime = m_currTime;

	return true;
}

bool TimeManager::Frame ()
{
	INT64 timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*) &m_currTime);

	timeDifference = m_currTime - m_lastFrameTime;

	m_currFrameTime = timeDifference / m_TicksPerMs;
	m_lastFrameTime = m_currTime;

	return true;
}

INT64 TimeManager::FrameTime ()
{
	return m_currFrameTime;
}

INT64 TimeManager::CurrentTime()
{
	return (m_currTime - m_appTime) / (40 * m_TicksPerMs);
}

TimeManager::~TimeManager(void)
{
}
