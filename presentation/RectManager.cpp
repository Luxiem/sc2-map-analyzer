#include "RectManager.h"
#include <GL/glew.h>
#include <GL/gl.h>


static void ToRGB(unsigned long a_colour, float& a_r, float& a_g, float& a_b)
{
	a_r = float((a_colour >> 16) & 0xff) / 255.f;
	a_g = float((a_colour >> 8) & 0xff) / 255.f;
	a_b = float((a_colour & 0xff)) / 255.f;
}


static void ToARGB(unsigned long a_colour, float& a_a, float& a_r, float& a_g, float& a_b)
{
	a_a = float((a_colour >> 24) & 0xff) / 255.f;
	a_r = float((a_colour >> 16) & 0xff) / 255.f;
	a_g = float((a_colour >> 8) & 0xff) / 255.f;
	a_b = float((a_colour & 0xff)) / 255.f;
}



RectManager::RectManager()
{
}


RectManager::~RectManager()
{

}


void RectManager::Draw(const Declaration2& a_decl)
{
	float x0 = (float)a_decl.Rect.left;
	float x1 = (float)a_decl.Rect.right;

	float y0 = (float)m_height - (float)a_decl.Rect.top;
	float y1 = (float)m_height - (float)a_decl.Rect.bottom;

	if (a_decl.Fill)
	{
		glBegin(GL_TRIANGLES);

		float a = 0.f, r = 0.f, g = 0.f, b = 0.f;

		// 1
		ToARGB(a_decl.Color0, a, b, g, r);
		glColor4f(r, g, b, a);
		glVertex2f(x0, y0);

		ToARGB(a_decl.Color2, a, b, g, r);
		glColor4f(r, g, b, a);
		glVertex2f(x1, y0);

		ToARGB(a_decl.Color3, a, b, g, r);
		glColor4f(r, g, b, a);
		glVertex2f(x1, y1);

		// 2
		ToARGB(a_decl.Color0, a, b, g, r);
		glColor4f(r, g, b, a);
		glVertex2f(x0, y0);

		ToARGB(a_decl.Color1, a, b, g, r);
		glColor4f(r, g, b, a);
		glVertex2f(x0, y1);

		ToARGB(a_decl.Color3, a, b, g, r);
		glColor4f(r, g, b, a);
		glVertex2f(x1, y1);
	}
	else
	{
		glBegin(GL_LINE_LOOP);

		float r = 0.f, g = 0.f, b = 0.f;

		ToRGB(a_decl.Color0, r, g, b);
		glColor3f(r, g, b);
		glVertex2f(x0 - 0.5f, y0 - 0.5f);

		ToRGB(a_decl.Color1, r, g, b);
		glColor3f(r, g, b);
		glVertex2f(x0 - 0.5f, y1 - 0.5f);

		ToRGB(a_decl.Color3, r, g, b);
		glColor3f(r, g, b);
		glVertex2f(x1 - 0.5f, y1 - 0.5f);

		ToRGB(a_decl.Color2, r, g, b);
		glColor3f(r, g, b);
		glVertex2f(x1 - 0.5f, y0 - 0.5f);

	}

	glEnd();
}


void RectManager::DrawCircle(int a_x, int a_y, float a_r)
{
	const float DEG2RAD = 3.14159 / 180;
	const int nPoints = (int)a_r;

	glBegin(GL_LINE_LOOP);
	glColor3f(1.f, 1.f, 1.f);

	for (int i = 0; i < nPoints; i++)
	{
		float degInRad = i * DEG2RAD * (360.f / float(nPoints));
		glVertex2f(cos(degInRad) * a_r + a_x, sin(degInRad) * a_r + m_height - a_y);
	}

	glEnd();
}


void RectManager::OnResize(unsigned short a_x, unsigned short a_y)
{
	m_width = a_x;
	m_height = a_y;
}


void RectManager::DrawPath(const std::list<point>& path01, int a_yOffset, unsigned long a_colour)
{
	glLineWidth(3.f);

	glBegin(GL_LINE_STRIP);

	float r = 0.f, g = 0.f, b = 0.f;
	ToRGB(a_colour, r, g, b);
	glColor3f(r, g, b);

	int x, y;
	for (std::list<point>::const_iterator i = path01.begin(); i != path01.end(); ++i)
	{
		x = (*i).mx;
		y = m_height - (*i).my + a_yOffset;
		glVertex2f(x, y);
	}

	glEnd();

	glLineWidth(1.f);
}
