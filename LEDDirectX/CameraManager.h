#pragma once

#include <D3DX10math.h>

class CameraManager
{
public:
	CameraManager(void);
	~CameraManager(void);

	void SetPosition (float x, float y, float z);
	void SetRotation (float x, float y, float z);

	D3DXVECTOR3 GetPosition ();
	D3DXVECTOR3 GetRotation ();

	void Render();
	void GetViewMatrix (D3DXMATRIX& viewMatrix);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};

