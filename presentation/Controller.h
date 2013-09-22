#pragma once


#include "UV/UV.h"


// SC2MA Controller class
class Controller
{
public:
  Controller();
  ~Controller();

  void Init();
  void Done();
  	
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

protected:

  // UI
	UV::Slider* s0;
	UV::Slider* s1;
	UV::Slider* s2;

};
