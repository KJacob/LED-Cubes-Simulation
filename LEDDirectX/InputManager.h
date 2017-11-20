#pragma once

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class InputManager
{
public:
	InputManager(void);
	~InputManager(void);

public:
	bool init (HINSTANCE hInstance, HWND hWnd);
	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();
	bool IsUpArrowPressed();
	bool IsDownArrowPressed();

	bool IsPlusPressed ();
	bool IsMinusPressed ();

	bool ReadKeyboard();
	bool Frame();

	void Shutdown ();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	unsigned char m_keyboardState[256];
};

