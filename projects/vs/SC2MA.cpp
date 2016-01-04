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
//#include <gl/glew.h>
#include "Common.h"

#define MAX_LOADSTRING 100


#pragma comment(lib, "opengl32.lib")


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	Common::Init(".");

    while (!Common::Quit())
    {
        Common::DrawScreen();
    }

	return 0;
}


void SaveImageFile()
{
  OPENFILENAME data;
  char szFileName[MAX_PATH] = "";

  ZeroMemory(&data, sizeof(OPENFILENAME));
  data.lStructSize = sizeof(OPENFILENAME); 
  data.hwndOwner = Common::getHwnd(); 
  data.lpstrFilter = "PNG Image (*.png)\0*.png\0"; 
  data.lpstrFile = szFileName; 
  data.nMaxFile = MAX_PATH;
  data.lpstrFileTitle = '\0';
  data.nMaxFileTitle = sizeof('\0'); 
  data.lpstrInitialDir = (LPCSTR)NULL; 
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
  data.hwndOwner = Common::getHwnd(); 
  data.lpstrFilter = "SC2Map (*.SC2Map)\0*.SC2Map\0SC2Map (*.s2ma)\0*.s2ma\0"; 
  data.lpstrFile = szFileName; 
  data.nMaxFile = MAX_PATH;
  data.lpstrFileTitle = '\0';
  data.nMaxFileTitle = sizeof('\0'); 
  data.lpstrInitialDir = (LPCSTR)NULL; 
  data.Flags = OFN_EXPLORER; 
  data.lpstrTitle = '\0'; 

  if (GetOpenFileName(&data))
  {
    Common::beginLoadMap(std::string(szFileName));
  }  
}


#endif//_WINDOWS
