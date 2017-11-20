#pragma once

#include <d3d10.h>
#include <D3DX10math.h>
#include <fstream>

class CubeShader
{
public:
	CubeShader(void);
	~CubeShader(void);

	bool init(ID3D10Device *device, HWND hWnd);
	void Shutdown ();
	void Render (ID3D10Device* device, int indexCount,
		D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix,
		D3DXVECTOR3 lightDirection,
		D3DXVECTOR4 diffuseColor,
		D3DXVECTOR4 ambientColor);

private:
	bool initShader (ID3D10Device *device, HWND hWnd, LPSTR filename);
	void ShutdownShader ();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage,
		HWND hWnd,
		LPSTR shaderFilename
		);

	void SetShaderParameters (D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix,
		D3DXVECTOR3 lightDirection,
		D3DXVECTOR4 diffuseColor,
		D3DXVECTOR4 ambientColor
		);
	void RenderShader (ID3D10Device *device, int indexCount);

	/*Variables.*/
	ID3D10Effect* m_effect;
	ID3D10EffectTechnique* m_technique;
	ID3D10InputLayout* m_layout;

	ID3D10EffectMatrixVariable* m_worldMatrixPtr;
	ID3D10EffectMatrixVariable* m_viewMatrixPtr;
	ID3D10EffectMatrixVariable* m_projectionMatrixPtr;
	ID3D10EffectVectorVariable* m_diffuseColorPtr;
	ID3D10EffectVectorVariable* m_lightDirectionPtr;
	ID3D10EffectVectorVariable* m_ambientColorPtr;
};

