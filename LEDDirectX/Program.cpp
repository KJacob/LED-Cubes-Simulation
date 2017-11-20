#include "Program.h"

Program::Program(void)
{
}

Program::Program (HINSTANCE hInstance,
				  LPSTR lpCmdLine,
				  int nShowCmd
				  )
{
	this->appCmdLine = lpCmdLine;
	this->appInstance = hInstance;
	this->nShowCmd = nShowCmd;
}

BOOL Program::init()
{
	WNDCLASSEX wcex;

	wcex.cbClsExtra			=	0;
	wcex.cbSize				=	sizeof(WNDCLASSEX);
	wcex.cbWndExtra			=	NULL;
	wcex.hbrBackground		=	(HBRUSH) NULL;
	wcex.hCursor			=	LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon				=	NULL;
	wcex.hIconSm			=	NULL;
	wcex.hInstance			=	appInstance;
	wcex.lpfnWndProc		=	MainWindow::WndProc;
	wcex.lpszClassName		=	"LEDCubes";
	wcex.lpszMenuName		=	(LPSTR) NULL;
	wcex.style				=	CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wcex))
		return FALSE;

	return TRUE;
}

int Program::run ()
{
	MSG m = {0};
	MainWindow frameWindow (appInstance, nShowCmd);
	bool done = false;

	if (!frameWindow.create())
		done = true;

	while (!done)
	{
		while (PeekMessage(&m, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&m);
			DispatchMessage(&m);
		}

		if (m.message == WM_QUIT)
			done = true;
		else {
			if (!frameWindow.Frame())
				done = true;
		}
	}

	frameWindow.Shutdown ();

	return m.wParam;
}

Program::~Program(void)
{
}
