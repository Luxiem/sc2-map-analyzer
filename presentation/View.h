#pragma once


#include "RectManager.h"
#include "FontManager.h"
#include <vector>
#include <list>


class SC2Map;
struct Base;
enum PathType;
class Controller;
class FBORenderTexture;


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

protected:

  SC2Map* m_sc2map;
	
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

  FBORenderTexture* m_fbo;

  int m_bufferWidth;
  int m_bufferHeight;	

  int m_windowWidth;
  int m_windowHeight;	

  // Pathing
	float pathTo(point& p0, point& p1, PathType t, std::list<point>& path);

  std::list<point> m_pathG;
  std::list<point> m_pathC;

  float m_dG;
  float m_dC;

  std::string m_spawnNameA;
  std::string m_spawnNameB;
  std::string m_baseNameA;
  std::string m_baseNameB;
};
