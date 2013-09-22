#pragma once


// Map Analyser version
#define VALG 1.62


#include <string>


// Model - Sc2Map instance
// View - OpenGL context
// Controller - UV, MainWindow
class View;
class Controller;
class SC2Map;


// Common static class 
class Common
{
public:
	
  // Init with path to data files
	static void Init(std::string a_path);

  // Load and analyse map
  // TODO Add callback function
  // TODO threaded
	static void beginLoadMap(std::string a_fileName);
	
  // TODO Not used ???
  // Save a PNG image of the current analysis
	static void SaveImage(std::string a_fileName);

  // Draw a frame of UI and Map View
	static void DrawScreen();
	
  // TODO
  // UI Controller
	static void IncPath(int a_inc);

  // User input
	static void OnLeftMouseDown(int a_x, int a_y);
	static void OnLeftMouseUp(int a_x, int a_y);
	static void OnMouseMove(int a_x, int a_y);
	static void OnScrollWheel(int a_delta);

  // Window size
  static void OnClientAreaChanged(int a_x, int a_y);

  // Check if map co-ordinate is pathable
  static bool IsPathable(int i, int j, int t);

  //
  static void ReloadConfig();
		
protected:
	
	// Core objects
	static SC2Map* s_sc2map;
  static Controller* s_controller;
  static View* s_view;

  // Siege map helpers
  // TODO - roll into analyser algorithms
  typedef void (*PlotFunc)(int, int);
	static void DDA(int x1, int y1, int x2, int y2, PlotFunc Plot);
	static int siege_processBlock;
	static bool siege_processUncover;
	static void PlotSiege(int a_x, int a_y);
	static int siege_processBlock2;
	static void PlotSiege2(int a_x, int a_y);
	static float isSiegeable(int a_x, int a_y, int a_innerRadius, int a_radius);

public:
	static void GenerateSiegeMap(int a_type);
  static float* siegeMap;
  static int s_genPathType;

};
