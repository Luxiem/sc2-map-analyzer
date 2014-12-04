#include "RectManager.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "Common.h" // for SDL_Renderer
#include "View.h"
#include "colour.h"


RectManager::RectManager()
{
  m_width = 1;
  m_height = 1;
}


RectManager::~RectManager()
{

}


void RectManager::Draw(const UV::Declaration2& a_decl)
{
    SDL_Renderer* renderer = Common::GetView()->Renderer();
   
    SDL_Rect rect;
    rect.x = a_decl.Rect.left;
    rect.y = a_decl.Rect.top;
    rect.w = a_decl.Rect.right - a_decl.Rect.left;
    rect.h = a_decl.Rect.bottom - a_decl.Rect.top;
    
  if (a_decl.Fill)
  {
      Uint8 a, r, g, b;
      Colour::ToARGB(a_decl.Color0, a, r, g, b);

      SDL_SetRenderDrawColor(renderer, r, g, b, a);

      
      SDL_RenderFillRect(renderer, &rect);
  }
  else
  {
      Uint8 a, r, g, b;
      Colour::ToARGB(a_decl.Color0, a, r, g, b);
      SDL_SetRenderDrawColor(renderer, r, g, b, a);
      SDL_RenderDrawRect(renderer, &rect);
  }
}


#define PI 3.141592653589793
void RectManager::DrawCircle(int a_x, int a_y, float a_r)
{
    
    int n = (int)(0.5 * a_r);
    SDL_Point* points = new SDL_Point[n];
    for (int i = 0; i < n; ++i)
    {
        float a = (float)(i) / (float)(n - 1) * 2.0 * PI;
        //points[i] = (SDL_Point) { a_x + (int)(a_r * sin(a)), a_y + (int)(a_r * cos(a)) };
		points[i].x = a_x + (int)(a_r * sin(a));
		points[i].y = a_y + (int)(a_r * cos(a));
    }
    
    SDL_Renderer* renderer = Common::GetView()->Renderer();
    
    SDL_RenderDrawLines(renderer, points, n);
    
    delete[] points;
}


void RectManager::OnResize(unsigned short a_x, unsigned short a_y)
{
  m_width = a_x;
  m_height = a_y;
}


void RectManager::DrawPath(const std::list<point>& path01, int a_yOffset, unsigned long a_colour)
{
    int n = (int)path01.size();
    SDL_Point* points = new SDL_Point[n];
    int j = 0;
    SDL_Renderer* renderer = Common::GetView()->Renderer();

    Uint8 a, r, g, b;
    Colour::ToARGB(a_colour, a, r, g, b);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    
    for (std::list<point>::const_iterator i = path01.begin(); i != path01.end(); ++i)
    {
        //points[j] = (SDL_Point){ (*i).mx, (*i).my };
		points[j].x = (*i).mx;
		points[j].y = (*i).my;
        ++j;
    }
    
    glLineWidth(3.f);
    SDL_RenderDrawLines(renderer, points, n);
    glLineWidth(1.f);

    delete[] points;
    
	/*
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
    */
}


