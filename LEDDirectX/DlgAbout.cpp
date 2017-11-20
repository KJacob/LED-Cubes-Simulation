#include "DlgAbout.h"


DlgAbout::DlgAbout(void)
{
}

DlgAbout* DlgAbout::self;

void DlgAbout::init(HINSTANCE hInstance, HWND hWndParent, const char* grName)
{
	self = this;
	m_grName = grName;

	DialogBox (hInstance, MAKEINTRESOURCE(IDD_ABOUT), hWndParent, (DLGPROC)DlgAbout::DlgProc);
}

HRESULT CALLBACK DlgAbout::DlgProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemText (hWnd, IDC_STATIC_GNAME, self->m_grName.c_str());
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_ABOUT_OK)
			EndDialog (hWnd, 0);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

DlgAbout::~DlgAbout(void)
{
}
