#include "MainWindow.h"


MainWindow::MainWindow(void)
{
}

MainWindow* MainWindow::self = NULL;

MainWindow::MainWindow (
	HINSTANCE hInstance,
	int nCmdShow
	)
{
	this->appInstance = hInstance;
	this->nCmdShow = nCmdShow;
	this->screenHeight = 600;
	this->screenWidth = 800;
	m_Value = NULL;
}

void MainWindow::OnTimer ()
{
	char title[100];

	sprintf_s (title, 100, "LED Cubes Simulation %.0f FPS", float(1000) / m_Time->FrameTime());
	SetWindowText (hWnd, title);
}

LRESULT CALLBACK MainWindow::WndProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		self->OnTimer ();
		break;

	case WM_COMMAND:
		self->OnCommand (wParam);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return TRUE;
}

void MainWindow::OnFunctionClicked ()
{
	FuncDialog *dialog = new FuncDialog();

	if (!dialog)
		return;

	dialog->init(appInstance, hWnd, m_Value);
	delete dialog;
}

void MainWindow::OnAboutClicked()
{
	DlgAbout *dialog = new DlgAbout();
	char buffer[120];
	int memory;

	if (!dialog)
		return;

	m_graphics->getAdapter()->GetVideoCardInfo(buffer, memory);

	dialog->init(appInstance, hWnd, buffer);

	delete dialog;
}

void MainWindow::OnPortSettingsClicked()
{
	PortDialog *portDlg = new PortDialog();

	portDlg->init(appInstance, hWnd, m_PortMan);
	delete portDlg;
}

void MainWindow::OnCommand (WPARAM wParam)
{
	switch(LOWORD(wParam))
	{
	case ID_OPTIONS_FUNCTION:
		OnFunctionClicked ();
		break;

	case ID_HELP_ABOUT:
		OnAboutClicked();
		break;

	case ID_OPTIONS_EXIT:
		DestroyWindow (hWnd);
		break;

	case ID_OPTIONS_PORTSETTINGS:
		OnPortSettingsClicked();
		break;
	}
}

BOOL MainWindow::create()
{
	self = this;

	hWnd = CreateWindow ("LEDCubes", "", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		screenWidth, screenHeight, NULL, (HMENU) NULL, appInstance, NULL);

	if (!hWnd)
		return FALSE;

	m_Time = new TimeManager ();
	if (!m_Time)
		return false;

	if (!m_Time->init())
		return false;

	m_Value = new ValueManager();
	if (!m_Value)
		return false;

	if(!m_Value->init())
		return false;

	m_hMainMenu = LoadMenu (appInstance, MAKEINTRESOURCE(IDR_MENU1));
	if (!m_hMainMenu)
		return FALSE;

	SetMenu(hWnd, m_hMainMenu);
	ShowWindow (hWnd, nCmdShow);

	m_Input = new InputManager();
	if (!m_Input)
		return false;

	if (!m_Input->init(appInstance, hWnd))
		return false;

	m_Position = new PositionManager();
	if (!m_Position)
		return false;

	m_Position->init();

	m_PortMan = new PortManager ();
	if (!m_PortMan)
		return false;

	m_PortMan->init(m_Value);

	m_graphics = new GraphicsManager ();

	if (!m_graphics->init (screenWidth, screenHeight, hWnd, m_Value))
		return FALSE;

	if (!SetTimer (hWnd, 0, 1000, 0))
		return FALSE;

	return TRUE;
}

void MainWindow::Shutdown ()
{
	KillTimer (hWnd, 0);
	if (m_graphics)
	{
		m_graphics->Shutdown();
		delete m_graphics;
		m_graphics = NULL;
	}

	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = NULL;
	}

	if (m_Position)
	{
		delete m_Position;
		m_Position = NULL;
	}

	if (m_PortMan)
	{
		delete m_PortMan;
		m_PortMan = NULL;
	}

	return;
}

bool MainWindow::Frame ()
{
	float z, yaw, pitch;

	m_Time->Frame();
	m_Value->Frame(m_Time->CurrentTime());

	m_Input->Frame();
	m_Position->SetFrameTime(float(m_Time->FrameTime()));

	m_Position->TurnDown(m_Input->IsDownArrowPressed());
	m_Position->TurnLeft(m_Input->IsLeftArrowPressed());
	m_Position->TurnRight(m_Input->IsRightArrowPressed());
	m_Position->TurnUp(m_Input->IsUpArrowPressed());
	m_Position->ZoomIn(m_Input->IsPlusPressed());
	m_Position->ZoomOut(m_Input->IsMinusPressed());
	m_Position->GetData(z, yaw, pitch);

	return m_graphics->Frame(z, yaw, pitch);
}

MainWindow::~MainWindow(void)
{
}
