#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls'\
version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "Program.h"

INT APIENTRY WinMain (
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine,
	int nShowCmd
	)
{
	Program app(hInstance, lpszCmdLine, nShowCmd);

	if (!app.init())
		return FALSE;

	return app.run();
}