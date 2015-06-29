#pragma once


#include "RectManager.h"
#include "FontManager.h"
#include <vector>
#include <list>
#include "sc2mapTypes.hpp" // for PathType
#include <SDL.h>

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#endif

class SC2Map;
struct Base;
class Controller;


// SC2MA View class
class View
{
public:
  View();
  ~View();

  void Init(Controller* a_controller);
  void Done();

  // Set the window size
  void OnClientAreaChanged(int a_width, int a_height);    
  
  // Draw a frame of UI and Map View
	void DrawScreen();

  // TODO - optimise
  void SaveImage(const char* a_fileName);

  void SetMap(SC2Map* a_map);
  		
  // Drawing objects
	FontManager g_fm;
	RectManager g_rm;

  // ?
  void SetPath(int a_pathSpawnA, int a_pathBaseA, int a_pathSpawnB, int a_pathBaseB);

    SDL_Renderer* Renderer() { return m_renderer; }
    
    // Draw the map again
    void NeedsUpdate() { m_updateMap = true; }

#ifdef _WINDOWS
	HWND getHwnd();
#endif
    
protected:

  SC2Map* m_sc2map;
  bool m_updateMap;
	
  // Controller for this view
  Controller* m_controller;

  // Image plotting helpers
	int imgX(int x);
	int imgY(int y);
	int playableToImgX(int x);
	int playableToImgY(int y);
	int mapToImgX(int x);
	int mapToImgY(int y);

  // Draw the map analyser image to a framebuffer
	void DrawMap();
  	
  // TODO - roll into analyser algorithms
  // Nexus positioning
  bool isClearNexus(const point& a_loc);
  	
  // TODO - roll into analyser algorithms
  // Test if a cell of the map is pathable
	bool IsPathable(int i, int j, int t);
  
  void createBuffer();
  void releaseBuffer();

    SDL_Surface* m_targetSurf;
    SDL_Texture* m_targetTex;

  int m_bufferWidth;
  int m_bufferHeight;	

  int m_windowWidth;
  int m_windowHeight;	

  // Pathing
	float pathTo(point& p0, point& p1, PathType t, std::list<point>& path);

  std::list<point> m_pathG;
  std::list<point> m_pathC;
  std::list<point> m_pathA;

  float m_dG;
  float m_dC;
  float m_dA;

  std::string m_spawnNameA;
  std::string m_spawnNameB;
  std::string m_baseNameA;
  std::string m_baseNameB;
    
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};
