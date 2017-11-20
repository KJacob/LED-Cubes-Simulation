#include "InputManager.h"


InputManager::InputManager(void)
{
	m_directInput = NULL;
	m_keyboard = NULL;
}

bool InputManager::init (HINSTANCE hInstance, HWND hWnd)
{
	HRESULT result;

	result = DirectInput8Create (hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_directInput, NULL);
	if (FAILED(result))
	{
		MessageBox (hWnd, "Can not create direct input.", "Error", MB_ICONSTOP);		
		return false;
	}

	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		MessageBox (hWnd, "Can not create direct input.", "Error", MB_ICONSTOP);
		return false;
	}

	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	result = m_keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;

	result = m_keyboard->Acquire();

	if (FAILED(result))
	{
		MessageBox (hWnd, "Can not create direct input.", "Error", MB_ICONSTOP);
		return false;
	}

	ZeroMemory(m_keyboardState, 256);

	return true;
}

bool InputManager::Frame ()
{
	if(!ReadKeyboard())
		return false;

	return true;
}

bool InputManager::ReadKeyboard ()
{
	HRESULT result;

	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_keyboard->Acquire();
		}
		else return false;
	}
	
	return true;
}

bool InputManager::IsLeftArrowPressed()
{
	if (m_keyboardState[DIK_LEFTARROW] & 0x80)
		return true;

	return false;
}

bool InputManager::IsRightArrowPressed()
{
	if (m_keyboardState[DIK_RIGHTARROW] & 0x80)
		return true;
	return false;
}

bool InputManager::IsUpArrowPressed()
{
	if (m_keyboardState[DIK_UPARROW] & 0x80)
		return true;
	return false;
}

bool InputManager::IsDownArrowPressed()
{
	if (m_keyboardState[DIK_DOWNARROW] & 0x80)
		return true;

	return false;
}

bool InputManager::IsPlusPressed()
{
	if(m_keyboardState[DIK_I] & 0x80)
		return true;
	return false;
}

bool InputManager::IsMinusPressed()
{
	if (m_keyboardState[DIK_O] & 0x80)
		return true;
	return false;
}

void InputManager::Shutdown()
{
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}
}

InputManager::~InputManager(void)
{
}
