#pragma once


#include "UV/UIVerse/UVRectManager.h"
#include <list>
#include "coordinates.hpp"


class RectManager : public UV::RectManager
{
public:
  RectManager();
  ~RectManager();

  void Draw(const UV::Declaration2& a_decl);

  void DrawCircle(int a_x, int a_y, float a_r);

  void OnResize(unsigned short a_x, unsigned short a_y);
	
  void DrawPath(const std::list<point>& path01, int a_yOffset, unsigned long a_colour);

};
