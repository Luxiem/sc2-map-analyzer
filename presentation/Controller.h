#pragma once


#include "UV/UV.h"


class SC2Map;


// SC2MA Controller class
class Controller
{
public:
  Controller();
  ~Controller();

  void Init();
  void Done();

  //
  void OnWindowSizeChanged(int a_width, int a_height);
  	
  //
  void Log(const char* a_msg);

  // TODO
  // public access ptr
	UV::Core* m_core;  

  bool ShowOpennessLayer();
  bool ShowLoS();
  bool ShowHeightLayer();
  bool ShowResourceLayer();
  bool ShowPhotonOvercharge();
  bool ShowWatchTower();
  bool ShowBases();
  bool ShowPathingLayer();

  float GetAlpha1();
  float GetAlpha2();
  float GetAlpha3();
  
  int SiegeMapType();

  void OnMapLoaded(SC2Map* a_map);

protected:

  // UI
	UV::Slider* s0;
	UV::Slider* s1;
	UV::Slider* s2;

  UV::List* m_log;

  UV::Combo* m_basePathStart;
  UV::Combo* m_basePathEnd;

  void PathCallback1(UV::Widget*, UV::EventArgs&);
  void PathCallback2(UV::Widget*, UV::EventArgs&);
  void PathCallback3(UV::Widget*, UV::EventArgs&);
  void PathCallback4(UV::Widget*, UV::EventArgs&);
  
  int m_pathSpawnA;
  int m_pathSpawnB;
  int m_pathBaseA;
  int m_pathBaseB;
};
