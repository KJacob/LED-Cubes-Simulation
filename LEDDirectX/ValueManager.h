#pragma once

#include <Windows.h>

#include "Function.h"

class ValueManager
{
public:
	ValueManager(void);

	bool init ();
	bool Frame(INT64 currTime);

	double getTParam ();
	double getValue(double x, double y, double z, double t);
	const string& getCurrentFunction();
	DWORD getTBegin();
	DWORD getTEnd();
	DWORD getTUpdate();

	void SetFunction (
		const string& strfn,
		DWORD dwTimeBegin,
		DWORD dwTimeEnd,
		DWORD dwTimeUpdate
		);

	void SetValue (int x, int y, int z, bool set);
	void *GetCurrentValue();

	~ValueManager(void);

private:
	DWORD m_numTicks, m_funcStart, m_TimeStart, m_TimeEnd, m_TimeInterval;
	Function* m_Func;
	string m_strFuncion;
	char m_currvalues[64];
};

