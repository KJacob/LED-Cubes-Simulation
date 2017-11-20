#pragma once

class PositionManager
{
public:
	PositionManager(void);

	void SetFrameTime(float time);
	bool init();

	void TurnLeft(bool keydown);
	void TurnRight(bool keydown);
	void TurnUp (bool keydown);
	void TurnDown (bool keydown);

	void ZoomIn(bool keydown);
	void ZoomOut(bool keydown);

	void GetData (float& z, float& yaw, float& pitch);

	~PositionManager(void);

private:
	float m_FrameTime, m_leftTurnSpeed, m_rightTurnSpeed, m_upTurnSpeed, m_downTurnSpeed, m_zoomInSpeed, m_zoomOutSpeed;
	float m_z, m_yaw, m_pitch;
};

