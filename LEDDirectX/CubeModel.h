#pragma once

#include <d3d10.h>
#include <D3DX10math.h>

#include <fstream>

using namespace std;

class CubeModel
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
	};

public:
	CubeModel(void);
	~CubeModel(void);

	bool init (ID3D10Device *Device);
	void Shutdown ();
	void Render (ID3D10Device* device);

	int GetIndexCount ();

private:
	bool InitBuffers (ID3D10Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D10Device* device);
	bool LoadModel (VertexType* &vertices, int& vCount);

	ID3D10Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

