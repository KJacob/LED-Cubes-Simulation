#include "CubeModel.h"


CubeModel::CubeModel(void)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

bool CubeModel::init(ID3D10Device *device)
{
	if (!InitBuffers(device))
		return false;

	return true;
}

void CubeModel::Shutdown ()
{
	ShutdownBuffers ();
}

void CubeModel::Render(ID3D10Device *device)
{
	RenderBuffers(device);
}

bool CubeModel::InitBuffers (ID3D10Device *device)
{
	VertexType *vertices = NULL;
	UINT* indices;
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	LoadModel (vertices, m_vertexCount);
	m_indexCount = m_vertexCount;

	indices = new UINT[m_indexCount];
	if (!indices)
		return false;

	for (i = 0; i < m_indexCount; ++i)
		indices[i] = i;

	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	vertexData.pSysMem = vertices;
	result = device->CreateBuffer(&vertexBufferDesc,
		&vertexData,
		&m_vertexBuffer
		);
	if (FAILED(result))
		return false;

	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_indexCount;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.MiscFlags = 0;

	indexData.pSysMem = indices;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool CubeModel::LoadModel(VertexType* &vertices, int& vCount)
{
	ifstream infile;
	int size, i;
	double x, y, z, nx, ny, nz;

	vCount = 0;

	infile.open("sphere.txt", ios::in);

	if (!infile.good())
		return false;

	infile >> size;
	vertices = new VertexType[size];

	for (i = 0; i < size; ++i)
	{
		infile >> x >> y >> z;
		infile >> nx >> ny;
		infile >> nx >> ny >> nz;
		vertices[i].position = D3DXVECTOR3(float(x), float(y), float(z));
		vertices[i].normal = D3DXVECTOR3(float(nx), float(ny), float(nz));
	}

	vCount = size;

	infile.close();
	return true;
}

int CubeModel::GetIndexCount()
{
	return m_indexCount;
}

void CubeModel::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release ();
		m_vertexBuffer = 0;
	}
}

void CubeModel::RenderBuffers (ID3D10Device *device)
{
	UINT stride;
	UINT offset;

	stride = sizeof(VertexType);
	offset = 0;

	device->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	device->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
}

CubeModel::~CubeModel(void)
{
}
