#include "CubeShader.h"


CubeShader::CubeShader(void)
{
	m_effect = NULL;
	m_technique = NULL;
	m_layout = NULL;
	m_worldMatrixPtr = NULL;
	m_viewMatrixPtr = NULL;
	m_projectionMatrixPtr = NULL;
}

bool CubeShader::init(ID3D10Device* device, HWND hWnd)
{
	if (!initShader(device, hWnd, "effects.hlsl"))
		return false;

	return true;
}

void CubeShader::Shutdown()
{
	ShutdownShader();
}

void CubeShader::Render(
	ID3D10Device* device,
	int indexCount,
	D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix,
	D3DXVECTOR3 lightDirection,
	D3DXVECTOR4 diffuseColor,
	D3DXVECTOR4 ambientColor
	)
{
	SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix, lightDirection, diffuseColor, ambientColor);
	RenderShader(device, indexCount);
}

bool CubeShader::initShader (ID3D10Device* device,
							 HWND hWnd,
							 LPSTR filename
							 )
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	D3D10_INPUT_ELEMENT_DESC polygonLayout[2];
	UINT numElements;
	D3D10_PASS_DESC passDesc;

	errorMessage = 0;

	result = D3DX10CreateEffectFromFile(
		filename,
		NULL,
		NULL,
		"fx_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		device,
		NULL,
		NULL,
		&m_effect,
		&errorMessage,
		NULL
		);

	if (FAILED(result))
	{
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hWnd, filename);
		else MessageBox(hWnd, filename, "Missing shader file.",
			MB_ICONSTOP);
		return false;
	}

	m_technique = m_effect->GetTechniqueByName("LightTechnique");
	if (!m_technique)
		return false;

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	m_technique->GetPassByIndex(0)->GetDesc(&passDesc);

	result = device->CreateInputLayout(polygonLayout, numElements,
		passDesc.pIAInputSignature, passDesc.IAInputSignatureSize,
		&m_layout);

	if (FAILED(result))
		return false;
	m_worldMatrixPtr = m_effect->GetVariableByName("worldMatrix")->AsMatrix();
	m_viewMatrixPtr = m_effect->GetVariableByName("viewMatrix")->AsMatrix();
	m_projectionMatrixPtr = m_effect->GetVariableByName("projectionMatrix")->AsMatrix();
	m_diffuseColorPtr = m_effect->GetVariableByName("diffuseColor")->AsVector();
	m_lightDirectionPtr = m_effect->GetVariableByName("lightDirection")->AsVector();
	m_ambientColorPtr = m_effect->GetVariableByName("ambientColor")->AsVector();
	return true;
}

void CubeShader::ShutdownShader()
{
	m_worldMatrixPtr = 0;
	m_viewMatrixPtr = 0;
	m_projectionMatrixPtr = 0;

	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	m_technique = 0;

	if (m_effect)
	{
		m_effect->Release();
		m_effect = 0;
	}
}

void CubeShader::OutputShaderErrorMessage(ID3DBlob* errorMessage,
										  HWND hWnd,
										  LPSTR shaderFileName
										  )
{
	using namespace std;

	char *compileErrors;
	UINT bufferSize, i;
	ofstream fout;

	compileErrors = (char *)errorMessage->GetBufferPointer();
	bufferSize = errorMessage->GetBufferSize();

	fout.open("shader-error.txt");

	for (i = 0; i < bufferSize; ++i)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = 0;

	MessageBox (hWnd, "Error while compiling shader.", "Exception", MB_ICONEXCLAMATION);
}

void CubeShader::SetShaderParameters (
	D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix,
	D3DXVECTOR3 lightDirection,
	D3DXVECTOR4 diffuseColor,
	D3DXVECTOR4 ambientColor
	)
{
	m_worldMatrixPtr->SetMatrix((float*) &worldMatrix);
	m_viewMatrixPtr->SetMatrix((float*) &viewMatrix);
	m_projectionMatrixPtr->SetMatrix((float*) &projectionMatrix);
	m_diffuseColorPtr->SetFloatVector((float*) &diffuseColor);
	m_lightDirectionPtr->SetFloatVector((float*) &lightDirection);
	m_ambientColorPtr->SetFloatVector((float*) &ambientColor);
}

void CubeShader::RenderShader (ID3D10Device* device, int indexCount)
{
	D3D10_TECHNIQUE_DESC techniqueDesc;
	UINT i;

	device->IASetInputLayout(m_layout);
	m_technique->GetDesc(&techniqueDesc);

	for (i = 0; i < techniqueDesc.Passes; ++i)
	{
		m_technique->GetPassByIndex(i)->Apply(0);
		device->DrawIndexed(indexCount, 0, 0);
	}
}

CubeShader::~CubeShader(void)
{
}
