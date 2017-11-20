#pragma once

#include <Windows.h>
#include "PortManager.h"
#include "resource.h"

class PortDialog
{
public:
	PortDialog(void);

	bool init (HINSTANCE hInstance, HWND hWndParent, PortManager* portMan);
	static HRESULT CALLBACK DlgProc (
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
		);

	void InitDialog(HWND hWnd);
	void OnCommand(HWND hWnd, WPARAM wParam);

	~PortDialog(void);

private:
	static PortDialog* self;
	PortManager* m_PortMan;
};

