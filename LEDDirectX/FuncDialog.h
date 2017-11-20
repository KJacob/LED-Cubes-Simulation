#pragma once

#include <Windows.h>
#include "ValueManager.h"
#include "resource.h"

class FuncDialog
{
public:
	FuncDialog(void);
	~FuncDialog(void);

	bool init (
		HINSTANCE appInstance,
		HWND hWndParent,
		ValueManager *valueMan
		);

	static HRESULT CALLBACK DlgProc (HWND Hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
		);

	void onInitDialog(HWND hWnd);

	void onCommand(HWND hWnd, WPARAM wParam);

private:
	ValueManager* m_Value;
	static FuncDialog* self;
};

