// ImGui - standalone example application for Glfw + OpenGL 2, using fixed pipeline
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "Shellapi.h"
#include <commdlg.h>
#include <process.h>


#include "Common.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow) 
{

	Common g_application;
	
	// Setup app (create context)
	g_application.Init(".");
	//Common::Init(".");


	// Main loop
	while (g_application.Quit() == false)
	{
		g_application.DrawScreen();
	}

	return 0;
}
