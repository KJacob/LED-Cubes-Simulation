#pragma once

#include <Windows.h>
#include "ValueManager.h"
#include <string>

#define BUFFER_SIZE 64

using namespace std;

class PortManager
{
public:
	PortManager(void);

	void StopWriting ();
	bool init (ValueManager* valueMan);

	bool StartWriting (int portNum);
	void die ();
	bool IsWriting();

	static DWORD WINAPI RunLoop(LPVOID lpParam);
	void Write ();
	int GetPort ();

	~PortManager(void);

private:
	HANDLE m_hComm;
	ValueManager *m_Value;
	bool bContinue;
	HANDLE m_hWorkerThread;
	int m_port;
	DWORD m_threadID;
	static PortManager* self;
};

