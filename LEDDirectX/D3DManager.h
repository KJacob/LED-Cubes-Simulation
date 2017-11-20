#pragma once

#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d10.h>
#include <D3DX10.h>

class D3DManager
{
public:
	D3DManager(void);

	bool init (
		int ScreenWidth,
		int screenHeight,
		bool vsync,
		HWND hWnd,
		bool fullscreen,
		float screenDepth,
		float screenNear
		);

	void Shutdown();
	void BeginScene (float red, float green, float blue, float alpha);
	void EndScene ();

	ID3D10Device* GetDevice();
	void GetProjectionMatrix (D3DXMATRIX& projectionMatrix);
	void GetWorldMatrix (D3DXMATRIX& worldMatirx);
	void GetOrthoMatrix (D3DXMATRIX& orthoMatrix);
	void GetVideoCardInfo (char *cardName, int& memory);

	~D3DManager(void);

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[120];
	IDXGISwapChain* m_SwapChain;
	ID3D10Device* m_device;
	ID3D10RenderTargetView* m_renderTargetView;
	ID3D10Texture2D* m_depthStencilBuffer;
	ID3D10DepthStencilState* m_depthStencilState;
	ID3D10DepthStencilView* m_depthStencilView;
	ID3D10RasterizerState* m_rasterState;
	D3DXMATRIX m_projectionMatrix, m_worldMatrix, m_orthoMatrix;
};