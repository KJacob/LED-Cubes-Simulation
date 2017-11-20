#pragma once

#include <D3DX10math.h>
#include <vector>
#include "ValueManager.h"

using namespace std;

class ModelManager
{
private:
	typedef struct _MODELINFO
	{
		int x, y, z;
		D3DXVECTOR3 position;
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
	} MODELINFO;

	D3DXVECTOR4 m_onColor, m_offColor;
	D3DXVECTOR3 m_lightDirection;
	int m_numModels;
	vector<MODELINFO> m_models;
	ValueManager *m_Value;
	double m_lastT;
public:
	ModelManager(void);

	bool init(D3DXVECTOR4 onColor, D3DXVECTOR4 offColor, ValueManager *value);

	bool Render();

	int GetModelCount ();
	void GetModel (int index, D3DXVECTOR3& postion, D3DXVECTOR4& diffuseColor, D3DXVECTOR4& ambientColor);

	~ModelManager(void);
};

