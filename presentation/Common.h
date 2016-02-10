#pragma once


#define QUOTEHELPER( x ) #x
#define QUOTEMACRO( x ) QUOTEHELPER( x )


// Map Analyser version
#define VALG 2.0


#include <string>


// Model - Sc2Map instance
// View - OpenGL context
// Controller - UV, MainWindow
class View;
class SettingsMap;
class SC2Map;


// Common static class 
class Common
{
public:

  Common();
  ~Common();
  
  // Init with path to config files
  void Init(std::string a_path);

  void Finalise();

   // Ready to exit the application?
  bool Quit();
    
  // Draw a frame of UI and Map View
  void DrawScreen();

public:
	static void Log(const char* a_msg);
	static void SetQuit();
	static void beginLoadMap(const char* a_fileName);
	static void SaveImage(const char* a_fileName);

protected:
 
  std::string common_log;
    
  void Log_impl(const char* a_msg);

  void SetQuit_impl() { m_quit = true;  }

  // Load and analyse map
  void beginLoadMap_impl(std::string a_fileName);

  // Save a PNG image of the current analysis
  void SaveImage_impl(std::string a_fileName);

private:

	// Legacy
	void SetPath(int a_pathSpawnA, int a_pathBaseA, int a_pathSpawnB, int a_pathBaseB);

	// Check if map co-ordinate is pathable
	bool IsPathable(int i, int j, int t);

	// Refresh config data from files
	void ReloadConfig(std::string a_path);

protected:
	
  // Core objects
  SC2Map* s_sc2map;
  SettingsMap* s_controller;
  View* s_view;

  // Siege map helpers
  // TODO - roll into analyser algorithms
  typedef void (*PlotFunc)(int, int);
  void DDA(int x1, int y1, int x2, int y2, PlotFunc Plot);
  int siege_processBlock;
  bool siege_processUncover;
  void PlotSiege(int a_x, int a_y);
  int siege_processBlock2;
  void PlotSiege2(int a_x, int a_y);
  float isSiegeable(int a_x, int a_y, int a_innerRadius, int a_radius);

  bool m_quit;

public:
  void GenerateSiegeMap(int a_type);
  float* siegeMap;
  int s_genPathType;

};
