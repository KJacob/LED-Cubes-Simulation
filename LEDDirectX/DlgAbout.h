#pragma once

#include <Windows.h>
#include <string>
#include "resource.h"

class DlgAbout
{
public:
	DlgAbout(void);

	void init  (HINSTANCE hInstance, HWND hWnd, const char* grName);
	static HRESULT CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	~DlgAbout(void);

private:
	static DlgAbout *self;
	std::string m_grName;
};

