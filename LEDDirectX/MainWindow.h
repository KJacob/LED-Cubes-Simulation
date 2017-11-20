#pragma once

#include "GraphicsManager.h"
#include "TimeManager.h"
#include "ValueManager.h"
#include "InputManager.h"
#include "PositionManager.h"
#include "PortManager.h"
#include "PortDialog.h"
#include "FuncDialog.h"
#include "DlgAbout.h"
#include "resource.h"

class MainWindow
{
private:
	static MainWindow *self;

public:
	MainWindow(void);

	MainWindow (HINSTANCE hInstance, int nShowCmd);

	BOOL create();
	void Shutdown ();
	bool Frame();
	void OnTimer ();
	void OnCommand (WPARAM wParam);
	void OnFunctionClicked ();
	void OnAboutClicked ();
	void OnPortSettingsClicked();

	static LRESULT CALLBACK WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	~MainWindow(void);

private:
	HWND hWnd;
	HINSTANCE appInstance;
	int nCmdShow, screenHeight, screenWidth;
	GraphicsManager *m_graphics;
	TimeManager *m_Time;
	ValueManager *m_Value;
	InputManager *m_Input;
	PositionManager *m_Position;
	PortManager *m_PortMan;
	HMENU m_hMainMenu;
};

