#include "FuncDialog.h"


FuncDialog::FuncDialog(void)
{
}

FuncDialog* FuncDialog::self;

bool FuncDialog::init(HINSTANCE appInstance, HWND hWndParent, ValueManager* valueMan)
{
	this->m_Value = valueMan;
	self = this;
	DialogBox (appInstance, MAKEINTRESOURCE(IDD_FUCTION), hWndParent, (DLGPROC) FuncDialog::DlgProc);

	return true;
}

void FuncDialog::onInitDialog(HWND hWnd)
{
	char buffer[15];

	SetDlgItemText(hWnd, IDC_EDIT_FUNC, m_Value->getCurrentFunction().c_str());
	sprintf_s (buffer, 15, "%d", m_Value->getTBegin());
	SetDlgItemText (hWnd, IDC_FUNC_TPB, buffer);

	sprintf_s (buffer, 15, "%d", m_Value->getTEnd());
	SetDlgItemText (hWnd, IDC_FUNC_TPE, buffer);

	sprintf_s (buffer, 15, "%d", m_Value->getTUpdate());
	SetDlgItemText (hWnd, IDC_FUNC_TPU, buffer);

	return;
}

void FuncDialog::onCommand(HWND hWnd, WPARAM wParam)
{
	DWORD startTime, endTime, updateInterval;
	char buffer[200];
	string str;

	if (LOWORD(wParam) == IDC_FUNC_OK)
	{
		GetDlgItemText (hWnd, IDC_EDIT_FUNC, buffer, 200);
		string str = buffer;

		GetDlgItemText (hWnd, IDC_FUNC_TPB, buffer, 15);
		startTime = atoi(buffer);

		GetDlgItemText (hWnd, IDC_FUNC_TPE, buffer, 15);
		endTime = atoi(buffer);

		GetDlgItemText (hWnd, IDC_FUNC_TPU, buffer, 15);
		updateInterval = atoi(buffer);

		if (updateInterval < 1 ||
			endTime < startTime)
		{
			MessageBox (hWnd, "Invalid values specified for the t-parameter.",
				"Error", MB_ICONSTOP);
			return;
		}

		try{
			m_Value->SetFunction(str, startTime, endTime, updateInterval);
		}
		catch (string& msg)
		{
			MessageBox (hWnd, msg.c_str(), "Error", MB_ICONEXCLAMATION);
			return;
		}

		EndDialog (hWnd, 0);
	}
	else if(IDC_FUNC_CANCEL == LOWORD(wParam)) 
		EndDialog (hWnd, 0);
}

HRESULT CALLBACK FuncDialog::DlgProc (
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		self->onInitDialog(hWnd);
		EnableWindow(hWnd, TRUE);
		return TRUE;

	case WM_COMMAND:
		self->onCommand (hWnd, wParam);
		break;

	default:
		return FALSE;
	}


	return TRUE;
}

FuncDialog::~FuncDialog(void)
{
}
