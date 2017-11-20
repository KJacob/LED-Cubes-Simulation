#include "PortDialog.h"


PortDialog::PortDialog(void)
{
}

PortDialog* PortDialog::self;

bool PortDialog::init(HINSTANCE hInstance, HWND hWndParent, PortManager* portMan)
{
	m_PortMan = portMan;
	self = this;
	DialogBox (hInstance, MAKEINTRESOURCE(IDD_PORT), hWndParent, (DLGPROC)DlgProc);

	return true;
}

void PortDialog::InitDialog(HWND hWnd)
{
	char buffer[15];

	sprintf_s (buffer, 15, "%d", m_PortMan->GetPort());
	SetDlgItemText (hWnd, IDC_PORT_NUMBER, buffer);

	if (m_PortMan->IsWriting())
	{
		EnableWindow (GetDlgItem(hWnd, IDC_PORT_START), FALSE);
		EnableWindow (GetDlgItem(hWnd, IDC_PORT_STOP), TRUE);
	}
	else{
		EnableWindow (GetDlgItem(hWnd, IDC_PORT_STOP), FALSE);
		EnableWindow (GetDlgItem(hWnd, IDC_PORT_START), TRUE);
	}

	return;
}

void PortDialog::OnCommand(HWND hWnd, WPARAM wParam)
{
	char buffer[15];

	switch(LOWORD(wParam))
	{
	case IDC_PORT_START:
		GetDlgItemText(hWnd, IDC_PORT_NUMBER, buffer, 15);
		if (!strlen(buffer))
		{
			MessageBox (hWnd, "Please fill the port number before continuing.",
			"Message", MB_ICONEXCLAMATION);
			return;
		}

		if (!m_PortMan->StartWriting(atoi(buffer)))
		{
			MessageBox (hWnd, "Error while opening port. Please review the port number and settings.",
				"Error", MB_ICONSTOP);
			return;
		}

		InitDialog (hWnd);
		break;

	case IDC_PORT_STOP:
		m_PortMan->StopWriting();
		m_PortMan->die();

		InitDialog(hWnd);
		break;

	case IDC_PORT_CLOSE:
		EndDialog(hWnd, 0);
		break;
	}

	return;
}

HRESULT CALLBACK PortDialog::DlgProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		self->InitDialog(hWnd);
		return TRUE;

	case WM_COMMAND:
		self->OnCommand(hWnd, wParam);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

PortDialog::~PortDialog(void)
{
}
