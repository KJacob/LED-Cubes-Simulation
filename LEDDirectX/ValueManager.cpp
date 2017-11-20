#include "ValueManager.h"


ValueManager::ValueManager(void)
	: m_strFuncion("pow(x-3.5,2)+pow(y-3.5,2)+pow(z-3.5,2)<=pow(t%8,2)")
{
}

bool ValueManager::init ()
{
	m_numTicks = 0;
	m_funcStart = 0;
	m_TimeStart = 0;
	m_TimeEnd = 1000;
	m_TimeInterval = 20;

	m_Func = new Function();
	m_Func->setFunction(m_strFuncion);
	m_Func->processFunction();

	ZeroMemory (m_currvalues, 64);

	return true;
}

const string& ValueManager::getCurrentFunction()
{
	return this->m_strFuncion;
}

void* ValueManager::GetCurrentValue()
{
	return m_currvalues;
}

DWORD ValueManager::getTBegin()
{
	return this->m_TimeStart;
}

DWORD ValueManager::getTEnd()
{
	return this->m_TimeEnd;
}

void ValueManager::SetValue(int x, int y, int z, bool set)
{
	int loc = z * 8 + y;

	if (set)
		m_currvalues[loc] = m_currvalues[loc] | (1 << x);
	else m_currvalues[loc] = m_currvalues[loc] & ~(1 << x);
}

DWORD ValueManager::getTUpdate()
{
	return this->m_TimeInterval;
}

void ValueManager::SetFunction(
	const string& strfn,
	DWORD dwTimeBegin,
	DWORD dwTimeEnd,
	DWORD dwTimeUpdate
	)
{
	try
	{
		m_Func->setFunction(strfn);
		m_Func->processFunction();
	}
	catch (string& msg)
	{
		m_Func->setFunction(m_strFuncion);
		m_Func->processFunction();
		throw;
	}

	this->m_strFuncion = strfn;
	this->m_TimeStart = dwTimeBegin;
	this->m_TimeEnd = dwTimeEnd;
	this->m_TimeInterval = dwTimeUpdate;

	return;
}

bool ValueManager::Frame (INT64 currTime)
{
	m_numTicks = currTime;
	return true;
}

double ValueManager::getTParam ()
{
	double t =  double(((m_numTicks - m_funcStart) / m_TimeInterval) + m_TimeStart);

	if (t > m_TimeEnd)
	{
		t = 0.0;
		m_funcStart = m_numTicks;
	}

	return t;
}

double ValueManager::getValue (double x, double y, double z, double t)
{
	FUNCPARAM param;

	param.t = t;
	param.x = x;
	param.y = y;
	param.z = z;

	return m_Func->evaluateFunction(param);
}

ValueManager::~ValueManager(void)
{
}
