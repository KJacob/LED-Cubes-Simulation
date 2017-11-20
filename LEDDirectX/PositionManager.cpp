#include "PositionManager.h"


PositionManager::PositionManager(void)
{
}

void PositionManager::SetFrameTime(float time)
{
	m_FrameTime = time;
}

bool PositionManager::init()
{
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_upTurnSpeed = 0.0f;
	m_downTurnSpeed = 0.0f;
	m_zoomInSpeed = 0.0f;
	m_zoomOutSpeed = 0.0f;

	m_z = -3.0f;
	m_yaw = 0.0f;
	m_pitch = 0.0f;

	return true;
}

void PositionManager::TurnLeft (bool keydown)
{
	if (keydown)
	{
		m_leftTurnSpeed += m_FrameTime * 0.001f;

		if (m_leftTurnSpeed > m_FrameTime * 0.015f)
			m_leftTurnSpeed = m_FrameTime * 0.015f;
	}
	else {
		m_leftTurnSpeed -= m_FrameTime * 0.0005f;

		if (m_leftTurnSpeed <= 0.0f)
			m_leftTurnSpeed = 0.0f;
	}

	m_yaw -= m_leftTurnSpeed * m_FrameTime;

	if (m_yaw < 0.0f)
		m_yaw += 360.0f;

	return;
}

void PositionManager::TurnRight (bool keydown)
{
	if (keydown)
	{
		m_rightTurnSpeed += m_FrameTime * 0.001f;

		if (m_rightTurnSpeed > m_FrameTime *  0.015f)
			m_rightTurnSpeed = m_FrameTime * 0.015f;
	}
	else {
		m_rightTurnSpeed -= m_FrameTime * 0.0005f;

		if (m_rightTurnSpeed <= 0.0f)
			m_rightTurnSpeed = 0.0f;
	}

	m_yaw += m_rightTurnSpeed * m_FrameTime;

	if (m_yaw > 360.0f)
		m_yaw -= 360.0f;

	return;
}

void PositionManager::TurnDown (bool keydown)
{
	if (keydown)
	{
		m_downTurnSpeed += m_FrameTime * 0.001f;

		if (m_downTurnSpeed > m_FrameTime * 0.015f)
			m_downTurnSpeed = m_FrameTime * 0.015f;
	}
	else {
		m_downTurnSpeed -= m_FrameTime * 0.0005f;

		if (m_downTurnSpeed <= 0.0f)
			m_downTurnSpeed = 0.0f;
	}

	m_pitch -= m_downTurnSpeed * m_FrameTime;

	if (m_pitch < 0.0f)
		m_pitch += 360.0f;

	return;
}


void PositionManager::TurnUp (bool keydown)
{
	if (keydown)
	{
		m_upTurnSpeed += m_FrameTime * 0.001f;

		if(m_upTurnSpeed > m_FrameTime * 0.015f)
			m_upTurnSpeed = m_FrameTime * 0.015f;
	}
	else {
		m_upTurnSpeed -= m_FrameTime * 0.0005f;

		if (m_upTurnSpeed <= 0.0f)
			m_upTurnSpeed = 0.0f;
	}

	m_pitch += m_upTurnSpeed * m_FrameTime;

	if (m_pitch > 360.0f)
		m_pitch -= 360.0f;

	return;
}

void PositionManager::ZoomIn (bool keydown)
{
	if (keydown)
	{
		m_zoomInSpeed += m_FrameTime * 0.0001f;

		if (m_zoomInSpeed > m_FrameTime * 0.0015f)
			m_zoomInSpeed = m_FrameTime * 0.0015f;
	}
	else {
		m_zoomInSpeed -= m_FrameTime * 0.00005f;

		if (m_zoomInSpeed <= 0.0f)
			m_zoomInSpeed = 0.0f;
	}

	m_z += m_zoomInSpeed * m_FrameTime;
	if (m_z > -3.0f)
		m_z = -3.0f;
}

void PositionManager::ZoomOut(bool keydown)
{
	if (keydown)
	{
		m_zoomOutSpeed += m_FrameTime * 0.0001f;

		if (m_zoomOutSpeed > m_FrameTime * 0.0015f)
			m_zoomOutSpeed = m_FrameTime * 0.0015f;
	}
	else {
		m_zoomOutSpeed -= m_FrameTime * 0.00005f;

		if (m_zoomOutSpeed <= 0.0f)
			m_zoomOutSpeed = 0.0f;
	}

	m_z -= m_zoomOutSpeed * m_FrameTime;
	if (m_z < -10.0f)
		m_z = -10.0f;
}

void PositionManager::GetData(float& z, float& yaw, float& pitch)
{
	z = m_z;
	yaw = m_yaw;
	pitch = m_pitch;
	return;
}

PositionManager::~PositionManager(void)
{
}
