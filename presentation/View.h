#pragma once


#include <vector>
#include <list>
#include "sc2mapTypes.hpp" // for PathType

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <RectManager.h>
#include <FontManager.h>

class SC2Map;
struct Base;
class SettingsMap;
typedef void(*CallbackVoid)(void);
typedef void(*CallbackString)(const char*);


// SC2MA View class
class View
{
public:
  View();
  ~View();

  void Init(SettingsMap* a_controller, std::string* a_log, CallbackString a_loadFn, CallbackString a_logFn, CallbackVoid a_quitFn);
  void Done();

  // Set the window size
  void OnClientAreaChanged(int a_width, int a_height);    
  
  // Draw a frame of UI and Map View
  void DrawScreen();

  // TODO - optimise
  void SaveImage(const char* a_fileName);

  void SetMap(SC2Map* a_map);

  // 
  void SetPath(int a_pathSpawnA, int a_pathBaseA, int a_pathSpawnB, int a_pathBaseB);

  // Draw the map again
  void NeedsUpdate() { m_updateMap = true; }
   
protected:

  SC2Map* m_sc2map;
  bool m_updateMap;

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

  // Pathing
  float pathTo(point& p0, point& p1, PathType t, std::list<point>& path);

  // For renter target switching
  void pushScreenCoordinateMatrix();
  void popScreenCoordinateMatrix();
  
  void createBuffer();
  void releaseBuffer();

  int m_bufferWidth;
  int m_bufferHeight;	

  std::list<point> m_pathG;
  std::list<point> m_pathC;
  std::list<point> m_pathA;

  float m_dG;
  float m_dC;
  float m_dA;

  int m_pathSelectA;
  int m_pathSelectB;
  int m_pathSelectC;
  int m_pathSelectD;

  std::string m_spawnNameA;
  std::string m_spawnNameB;
  std::string m_baseNameA;
  std::string m_baseNameB;
    
  // IMGUI
  GLFWwindow* m_glwindow;
  std::string* m_commonLog;

  // Callback functions
  CallbackString m_beginLoadMap;
  CallbackString m_logMessage;
  CallbackVoid m_setQuit;

  // Draw
  RectManager m_rm;
  FontManager m_fm;

  // Render Target
  GLuint m_frameBuffer;
  GLuint m_renderTexture;

  // Controller for this view
  SettingsMap* settings;
};
