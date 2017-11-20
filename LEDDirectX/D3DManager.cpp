#include "D3DManager.h"


D3DManager::D3DManager(void)
{
	m_device			=	NULL;
	m_SwapChain			=	NULL;
	m_renderTargetView	=	NULL;
	m_depthStencilBuffer=	NULL;
	m_depthStencilState =	NULL;
	m_depthStencilView	=	NULL;
	m_rasterState		=	NULL;
}

bool D3DManager::init (int screenWidth,
					   int screenHeight,
					   bool vsync,
					   HWND hWnd,
					   bool fullscreen,
					   float screenDepth,
					   float screenNear
					   )
{
	HRESULT result;
	IDXGIFactory *factory;
	IDXGIAdapter *adapter;
	IDXGIOutput *adapterOutput;
	UINT numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC *displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D10Texture2D* backBufferPtr;
	D3D10_TEXTURE2D_DESC depthBufferDesc;
	D3D10_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D10_VIEWPORT viewport;
	float fieldOfView, screenAspect;
	D3D10_RASTERIZER_DESC rasterDesc;

	m_vsync_enabled = vsync;

	/* Factory. */
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

	if (FAILED(result))
		return false;

	/* Video card. */
	result = factory->EnumAdapters (0, &adapter);
	if (FAILED(result))
		return false;

	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
		return false;

	result = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, NULL);

	if (FAILED(result))
		return FALSE;

	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return false;

	/* Dispay Mode structures. */
	result = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList
		);

	if (FAILED(result))
		return false;

	/* Find out the thing that matches the screen width and height. */
	for (i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (UINT)screenWidth &&
			displayModeList[i].Height == (UINT) screenHeight)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	/* King's description. */
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
		return false;

	m_videoCardMemory = int (adapterDesc.DedicatedVideoMemory / (1024 * 1024));

	/* NVIDIA. They way it's meant to be played. */
	error = wcstombs_s (&stringLength, m_videoCardDescription,
		128,
		adapterDesc.Description, 128);

	if (error)
		return false;

	/* Now y'all are free. */
	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	/* Initialize the swap chain. */
	ZeroMemory (&swapChainDesc, sizeof(swapChainDesc));

	/* One back buffer. */
	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	/* Reular, 32 bit surface. */
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	/* Refresh rate. */
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = hWnd;

	/* Multisampling off. */
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.Windowed = !fullscreen;
	
	/* Scan line ordering and scaling. */
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	/* Discard after presenting. */
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	/* No advanced flags. */
	swapChainDesc.Flags = 0;

	/* Swap chain and D3D device. */
	result = D3D10CreateDeviceAndSwapChain (NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		D3D10_SDK_VERSION,
		&swapChainDesc,
		&m_SwapChain,
		&m_device
		);
	if (FAILED(result))
		return false;

	/* Get the back buffer. */
	result = m_SwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D),
		(LPVOID*) &backBufferPtr);
	if (FAILED(result))
		return false;

	result = m_device->CreateRenderTargetView(backBufferPtr,
		NULL,
		&m_renderTargetView
		);
	if (FAILED(result))
		return false;

	backBufferPtr->Release();
	backBufferPtr = NULL;

	ZeroMemory (&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width			=	screenWidth;
	depthBufferDesc.Height			=	screenHeight;
	depthBufferDesc.MipLevels		=	1;
	depthBufferDesc.ArraySize		=	1;
	depthBufferDesc.Format			=	DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count=	1;
	depthBufferDesc.SampleDesc.Quality=	0;
	depthBufferDesc.Usage			=	D3D10_USAGE_DEFAULT;
	depthBufferDesc.BindFlags		=	D3D10_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags	=	0;
	depthBufferDesc.MiscFlags		=	0;

	result = m_device->CreateTexture2D(&depthBufferDesc,
		NULL,
		&m_depthStencilBuffer);
	if (FAILED(result))
		return false;

	ZeroMemory (&depthStencilDesc, sizeof(depthStencilDesc));

	/* Stencil descprition.*/
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;
	
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	
	depthStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	/* Back facing. */
	depthStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	/* Depth stencil state construction. */
	result = m_device->CreateDepthStencilState(&depthStencilDesc,
		&m_depthStencilState);
	if(FAILED(result))
		return false;

	m_device->OMSetDepthStencilState(m_depthStencilState, 1);

	/* Depth stencil view. */
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	/* Construct depth stencil view. */
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer,
		&depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
		return false;

	/* Bind render target view. */
	m_device->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode	= D3D10_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D10_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
		return false;

	/* Rasteriser state. */
	m_device->RSSetState(m_rasterState);

	/* Viewport for rendering. */
	viewport.Width = screenWidth;
	viewport.Height = screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	m_device->RSSetViewports(1, &viewport);

	/* Projection matrix. */
	fieldOfView = float(D3DX_PI / 4.0f);
	screenAspect = float(screenWidth) / screenHeight;

	/* Create. */
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect,
		screenNear, screenDepth);

	D3DXMatrixIdentity (&m_worldMatrix);

	/* Orthographic projection. */
	D3DXMatrixOrthoLH(&m_orthoMatrix, float(screenWidth),
		float(screenHeight), screenNear, screenDepth);

	return true;
}

void D3DManager::Shutdown ()
{
	if (m_SwapChain)
		m_SwapChain->SetFullscreenState(false, NULL);

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = NULL;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = NULL;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = NULL;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = NULL;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = NULL;
	}

	if (m_SwapChain)
	{
		m_SwapChain->Release();
		m_SwapChain= NULL;
	}

	return;
}

void D3DManager::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = red;
	color[3] = alpha;

	/* Clear back buffer. */
	m_device->ClearRenderTargetView(m_renderTargetView, color);

	/* Depth buffer. */
	m_device->ClearDepthStencilView(m_depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DManager::EndScene()
{
	if (m_vsync_enabled)
	{
		m_SwapChain->Present(1, 0);
	}
	else {
		m_SwapChain->Present(0, 0);
	}
}

ID3D10Device* D3DManager::GetDevice ()
{
	return m_device;
}

void D3DManager::GetProjectionMatrix (D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}

void D3DManager::GetWorldMatrix (D3DXMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}

void D3DManager::GetOrthoMatrix (D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

void D3DManager::GetVideoCardInfo (char *cardName, int &memory)
{
	strcpy_s (cardName, 120, m_videoCardDescription);
	memory = m_videoCardMemory;
}

D3DManager::~D3DManager(void)
{
}
