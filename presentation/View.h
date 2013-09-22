#pragma once


#include "RectManager.h"
#include "FontManager.h"
#include <vector>
#include <list>


class SC2Map;
struct Base;
enum PathType;
class Controller;
typedef unsigned int GLuint;


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
  void IncPath(int a_inc);

  //
  void GetPixel(int a_i, int a_j, double& r, double& g, double& b);

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
  void calcBases();
  	
  // TODO - roll into analyser algorithms
  // Test if a cell of the map is pathable
	bool IsPathable(int i, int j, int t);
  
  void createBuffer();
  void releaseBuffer();

  void pushScreenCoordinateMatrix();
  void popScreenCoordinateMatrix();

  // GL View objects
	GLuint m_renderedTexture;
	GLuint m_FramebufferName;

  int bufferWidth;
  int bufferHeight;	

  int windowWidth;
  int windowHeight;	

  // Pathing
	std::vector< std::pair< Base*, Base* > > m_mainPairs;
	std::vector< std::pair< Base*, Base* > > m_natPairs;
	int m_pathN;
	int m_pathT;

  // Pathing generation
	void CreatePathingLayout();
	float pathTo(point& p0, point& p1, PathType t, std::list<point>& path);

};
