#pragma once


#include <list>
#include "coordinates.hpp"
#include <windows.h>


// Rectangle Declaration
class Declaration2
{
public:

	Declaration2()
	{
		Color0 = 0xfffafafa;
		Color1 = 0xffd2d2d2;
		Color2 = 0xfffafafa;
		Color3 = 0xffd2d2d2;

		Fill = true;
	};

	RECT Rect;
	unsigned long Color0;
	unsigned long Color1;
	unsigned long Color2;
	unsigned long Color3;

	bool Fill;
};


class RectManager
{
public:
	RectManager();
	~RectManager();

	void Draw(const Declaration2& a_decl);

	void DrawCircle(int a_x, int a_y, float a_r);

	void OnResize(unsigned short a_x, unsigned short a_y);

	void DrawPath(const std::list<point>& path01, int a_yOffset, unsigned long a_colour);

private:

	int m_height;
	int m_width;

};