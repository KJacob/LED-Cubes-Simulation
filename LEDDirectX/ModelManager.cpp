#include "ModelManager.h"


ModelManager::ModelManager(void) : m_models(512)
{
	m_numModels = 512;
}

bool ModelManager::init (D3DXVECTOR4 onColor, D3DXVECTOR4 offColor, ValueManager *value)
{
	int x, y, z, i;

	this->m_onColor = onColor;
	this->m_offColor = offColor;

	i = 0;

	for (x = 0; x < 8; ++x)
	{
		for (y = 0; y < 8; ++y)
		{
			for (z = 0; z < 8; ++z)
			{
				m_models[i].x = x;
				m_models[i].y = y;
				m_models[i].z = z;

				m_models[i].position = D3DXVECTOR3((x - 3.5f) * 0.2f, (y - 3.5f) * 0.2f, (z - 3.5f) * 0.2f);
				m_models[i].diffuseColor = onColor;
				m_models[i].ambientColor = 0.3f * onColor;

				i++;
			}
		}
	}

	this->m_Value = value;
	m_lastT = -1.0;

	return true;
}

bool ModelManager::Render ()
{
	double tparam = m_Value->getTParam();
	double result;

	if (tparam == m_lastT)
		return true; /* Nothing to do. */

	int i;
	m_lastT = tparam;

	for (i = 0; i < m_numModels; ++i)
	{
		result = m_Value->getValue (double(m_models[i].x),
			double (m_models[i].y),
			double (m_models[i].z),
			tparam);

		if (result > 0.5 || result < -0.5)
		{
			/* On. */
			m_models[i].ambientColor = 0.3f * m_onColor;
			m_models[i].diffuseColor = m_onColor;
			m_Value->SetValue(m_models[i].x, m_models[i].y, m_models[i].z, true);
		}
		else {
			m_models[i].ambientColor = 0.3f * m_offColor;
			m_models[i].diffuseColor = m_offColor;
			m_Value->SetValue(m_models[i].x, m_models[i].y, m_models[i].z, false);
		}
	}

	return true;
}

void ModelManager::GetModel (int index, D3DXVECTOR3& position, D3DXVECTOR4& diffuseColor, D3DXVECTOR4& ambientColor)
{
	if (index < 0 || index > m_numModels)
		return;

	position = m_models[index].position;
	diffuseColor = m_models[index].diffuseColor;
	ambientColor = m_models[index].ambientColor;

	return;
}

int ModelManager::GetModelCount ()
{
	return m_numModels;
}

ModelManager::~ModelManager(void)
{
}
