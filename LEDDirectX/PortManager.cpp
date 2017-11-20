#include "PortManager.h"

PortManager* PortManager::self;

PortManager::PortManager(void)
{
	bContinue = false;
	m_hComm = INVALID_HANDLE_VALUE;
	m_port = 0;
}

void PortManager::StopWriting()
{
	bContinue = false;
}

bool PortManager::init(ValueManager* valueMan)
{
	m_Value = valueMan;
	return true;
}

int PortManager::GetPort()
{
	return m_port;
}

bool PortManager::StartWriting (int portNum)
{
	char buffer[100];

	m_port = portNum;

	sprintf_s (buffer, 100, "\\\\.\\COM%d", portNum);

	m_hComm = CreateFile (
		buffer,
		GENERIC_READ | GENERIC_WRITE,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	if (INVALID_HANDLE_VALUE == m_hComm)
		return false;

	bContinue = true;

	m_hWorkerThread = CreateThread (
		NULL,
		NULL,
		RunLoop,
		NULL,
		NULL,
		&m_threadID
		);

	if (NULL == m_hWorkerThread)
	{
		CloseHandle(m_hComm);
		return false;
	}

	return true;
}

DWORD WINAPI PortManager::RunLoop (LPVOID lpParam)
{
	self->Write ();
	return 0;
}

void PortManager::Write ()
{
	DWORD dwWritten;

	while (bContinue)
	{
		WriteFile (m_hComm, m_Value->GetCurrentValue(), BUFFER_SIZE, &dwWritten, NULL);

		if (dwWritten != BUFFER_SIZE)
			break;
	}

	CloseHandle (m_hComm);
	bContinue = false;
	m_hComm = INVALID_HANDLE_VALUE;
	return;
}

bool PortManager::IsWriting()
{
	return bContinue;
}

void PortManager::die ()
{
	if (m_hWorkerThread)
	{
		WaitForSingleObject(m_hWorkerThread, INFINITE);
		CloseHandle(m_hWorkerThread);
	}

	m_hWorkerThread = NULL;
}

PortManager::~PortManager(void)
{
}
