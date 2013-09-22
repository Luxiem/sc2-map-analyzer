#ifdef _WINDOWS

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "Resource.h"

#include "Shellapi.h"
#include <commdlg.h>
#include <process.h>
#include <gl/glew.h>
#include "Common.h"

#define MAX_LOADSTRING 100


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "ftgl.lib")
#pragma comment(lib, "libpng16.lib")
#pragma comment(lib, "Releasetinyxml/tinyxml.lib")


// Stuff



// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND g_hwnd;
HDC g_hDC;
HGLRC g_hRC;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SC2MA, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SC2MA));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

    // If the message is WM_QUIT, exit the while loop
    if(msg.message == WM_QUIT)
      break;

    // Run engine code
		Common::DrawScreen();
    SwapBuffers(g_hDC);
	}

  // Clean up
  DisableOpenGL(g_hwnd, g_hDC, g_hRC);
  DestroyWindow(g_hwnd);

	return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WindowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SC2MA));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  hInst = hInstance; // Store instance handle in our global variable

  g_hwnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

  if (!g_hwnd)
  {
    return FALSE;
  }

  // Initialise
  DragAcceptFiles(g_hwnd, true);

  // GL
  EnableOpenGL(g_hwnd, &g_hDC, &g_hRC);
      
  // GLEW
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    return FALSE;
  }

  // Init analyser with the tool paths
  Common::Init(".");
  
  // Final part of init, triggers a WM_SIZE message
  ShowWindow(g_hwnd, nCmdShow);
  UpdateWindow(g_hwnd);

  return TRUE;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_DESTROY:
    {
      PostQuitMessage(0);
      return 0;
    };
  case WM_SIZE:
    {
      // Resize the application window
      int x = (int)LOWORD(lParam); 
      int y = (int)HIWORD(lParam);
      Common::OnClientAreaChanged(x, y);
      return 0;
    }

    case WM_MOUSEMOVE:
      {
        // Mouse move
        int xPos = (int)LOWORD(lParam); 
        int yPos = (int)HIWORD(lParam);
        Common::OnMouseMove(xPos, yPos);
        return 0;
      }

    case WM_LBUTTONDOWN:
      {
        // Left mouse button down
        WORD xPos = LOWORD(lParam);
        WORD yPos = HIWORD(lParam);
        Common::OnLeftMouseDown(xPos, yPos);
        return 0;
      }

    case WM_LBUTTONUP:
      {
        // Left mouse button up
        WORD xPos = LOWORD(lParam); 
        WORD yPos = HIWORD(lParam);
        Common::OnLeftMouseUp(xPos, yPos);
        return 0;
      }

    case WM_MOUSEWHEEL:
      {
        return 0;
      }

    case WM_DROPFILES:
		  {
        // Drag-and-drop file
        TCHAR lpszFile[MAX_PATH] = {0};
		    UINT uFile = 0;
		    HDROP hDrop = (HDROP)wParam;

		    uFile = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, NULL);
		    if (uFile != 1)
        {
			    DragFinish(hDrop);
			    break;
			  }
		    lpszFile[0] = '\0';
		    if (DragQueryFile(hDrop, 0, lpszFile, MAX_PATH))
			  {
          Common::beginLoadMap(std::string(lpszFile));
			  }
			
		    DragFinish(hDrop);

			  return 0;
		  }

  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
}


void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;
	
	// get the device context (DC)
	*hDC = GetDC( hWnd );
	
	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( *hDC, &pfd );
	SetPixelFormat( *hDC, format, &pfd );
	
	// create and enable the render context (RC)
	*hRC = wglCreateContext( *hDC );
	wglMakeCurrent( *hDC, *hRC );

  // set params
  glShadeModel(GL_SMOOTH); 	
}


void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );
}


void SaveImageFile()
{
  OPENFILENAME data;
  char szFileName[MAX_PATH] = "";

  ZeroMemory(&data, sizeof(OPENFILENAME));
  data.lStructSize = sizeof(OPENFILENAME); 
  data.hwndOwner = g_hwnd; 
  data.lpstrFilter = "PNG Image (*.png)\0*.png\0"; 
  data.lpstrFile = szFileName; 
  data.nMaxFile = MAX_PATH;
  data.lpstrFileTitle = '\0';
  data.nMaxFileTitle = sizeof('\0'); 
  data.lpstrInitialDir = (LPSTR)NULL; 
  data.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT; 
  data.lpstrTitle = '\0'; 

  if (GetSaveFileName(&data))
  {
    Common::SaveImage(std::string(szFileName));
  }  
}


void LoadMapFile()
{
  OPENFILENAME data;
  char szFileName[MAX_PATH] = "";

  ZeroMemory(&data, sizeof(OPENFILENAME));
  data.lStructSize = sizeof(OPENFILENAME); 
  data.hwndOwner = g_hwnd; 
  data.lpstrFilter = "SC2Map (*.SC2Map)\0*.SC2Map\0SC2Map (*.s2ma)\0*.s2ma\0"; 
  data.lpstrFile = szFileName; 
  data.nMaxFile = MAX_PATH;
  data.lpstrFileTitle = '\0';
  data.nMaxFileTitle = sizeof('\0'); 
  data.lpstrInitialDir = (LPSTR)NULL; 
  data.Flags = OFN_EXPLORER; 
  data.lpstrTitle = '\0'; 

  if (GetOpenFileName(&data))
  {
    Common::beginLoadMap(std::string(szFileName));
  }  
}


#endif//_WINDOWS
