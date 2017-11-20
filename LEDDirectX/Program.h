#pragma once

#include "MainWindow.h"

class Program
{
public:
	Program(void);
	Program (HINSTANCE hInstance, LPSTR lpCmdLine, int nShowCmd);
	
	BOOL init();
	int run();

	~Program(void);

private:
	HINSTANCE appInstance;
	LPSTR appCmdLine;
	int nShowCmd;
};

