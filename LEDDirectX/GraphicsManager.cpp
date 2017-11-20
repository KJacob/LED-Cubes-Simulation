#include "GraphicsManager.h"


GraphicsManager::GraphicsManager(void)
{
	m_d3d = NULL;
	m_Camera = NULL;
	m_Cube = NULL;
	m_CubeShader = NULL;
	m_ModelManager = NULL;
}

bool GraphicsManager::init(int screenWidth, int screenHeight, HWND hWnd, ValueManager* value)
{
	bool result;

	this->m_Value = value;

	m_d3d = new D3DManager ();
	if (!m_d3d)
		return false;

	result = m_d3d->init(screenWidth, screenHeight,
		VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR
		);

	if (!result)
	{
		MessageBox (hWnd, "Can not start DirectX.", "Fatal",
			MB_ICONSTOP);
		return false;
	}

	m_Camera = new CameraManager();
	if (!m_Camera)
		return false;

	m_Camera->SetPosition(0.0f, 0.0f, -3.0f);

	m_Cube = new CubeModel ();
	if (!m_Cube)
		return false;

	if (!m_Cube->init(m_d3d->GetDevice()))
	{
		MessageBox (hWnd, "Could not load the model.", "Error",
			MB_ICONSTOP);
		return false;
	}

	m_CubeShader = new CubeShader();
	if (!m_CubeShader)
		return false;

	if (!m_CubeShader->init(m_d3d->GetDevice(), hWnd))
	{
		MessageBox (hWnd, "Can not load the shader.", "Error", MB_ICONSTOP);
		return false;
	}

	m_ModelManager = new ModelManager();
	if (!m_ModelManager)
	{
		MessageBox (hWnd, "Can not load the Model Manager.", "Error", MB_ICONSTOP);
		return false;
	}

	m_ModelManager->init(D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.667f, 0.667f, 0.667f, 1.0f), m_Value);

	return true;
}

D3DManager* GraphicsManager::getAdapter()
{
	return m_d3d;
}

void GraphicsManager::Shutdown()
{
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = NULL;
	}

	if (m_CubeShader)
	{
		m_CubeShader->Shutdown();
		delete m_CubeShader;
		m_CubeShader = NULL;
	}

	if (m_Cube)
	{
		m_Cube->Shutdown();
		delete m_Cube;
		m_Cube = 0;
	}

	if (m_d3d)
	{
		m_d3d->Shutdown();
		delete m_d3d;
		m_d3d = NULL;
	}

	if (m_ModelManager)
	{
		delete m_ModelManager;
		m_ModelManager = NULL;
	}
	return;
}

bool GraphicsManager::Frame (float z, float yaw, float pitch)
{
	m_Camera->SetPosition(0.0f, 0.0f, z);
	m_Camera->SetRotation(pitch, yaw, 0.0f);
	return Render();
}

bool GraphicsManager::Render ()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	D3DXVECTOR4 diffuseColor, ambientColor;
	D3DXVECTOR3 lightDirection(0.577350f, -0.577350f, 0.577350f), position;
	D3DXMATRIX scaleMatrix, translationMatrix, effectMatrix;
	int count = 0, maxcount = m_ModelManager->GetModelCount();

	D3DXMatrixScaling(&scaleMatrix, 0.05f, 0.05f, 0.05f);

	m_d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render ();
	m_ModelManager->Render();

	m_Camera->GetViewMatrix(viewMatrix);
	m_d3d->GetProjectionMatrix(projectionMatrix);

	for (count = 0; count < maxcount; ++count)
	{
		m_d3d->GetWorldMatrix(worldMatrix);
		m_ModelManager->GetModel(count, position, diffuseColor, ambientColor);

		D3DXMatrixTranslation(&translationMatrix, position.x, position.y, position.z);

		D3DXMatrixMultiply (&effectMatrix, &scaleMatrix, &translationMatrix);
		D3DXMatrixMultiply (&worldMatrix, &effectMatrix, &worldMatrix);

		m_Cube->Render(m_d3d->GetDevice());
		m_CubeShader->Render (m_d3d->GetDevice(), m_Cube->GetIndexCount(), worldMatrix,
			viewMatrix, projectionMatrix, lightDirection, diffuseColor, ambientColor);
	}

	m_d3d->EndScene();
	return true;
}

GraphicsManager::~GraphicsManager(void)
{
}
