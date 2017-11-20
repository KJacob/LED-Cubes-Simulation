#pragma once

#include "D3DManager.h"
#include "CubeModel.h"
#include "CubeShader.h"
#include "CameraManager.h"
#include "ModelManager.h"
#include "ValueManager.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsManager
{
public:
	GraphicsManager(void);

	bool init (int screenWidth, int screenHeight, HWND hWnd, ValueManager* value);
	void Shutdown ();
	bool Frame(float z, float yaw, float pitch);
	D3DManager* getAdapter();

	~GraphicsManager(void);

private:
	bool Render ();
	D3DManager *m_d3d;
	CameraManager* m_Camera;
	CubeModel* m_Cube;
	CubeShader* m_CubeShader;
	ModelManager *m_ModelManager;
	ValueManager *m_Value;
};

