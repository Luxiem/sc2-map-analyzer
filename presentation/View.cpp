#include "View.h"
#include "Controller.h"
#include "SC2Map.hpp"
#include "UV/UV.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "Common.h" // siege path stuff
#include "pngwriter.h"


#define DWORD_ARGB(a,r,g,b) \
((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))


static inline DWORD ColorToDword(Color* a_color)
{
  if (a_color == 0) return 0xffaaaaaa;

  return DWORD_ARGB(0xff, (int)(255 * a_color->r), (int)(255 * a_color->g), (int)(255 * a_color->b));
}


View::View()
{
  m_renderedTexture = 0;
  m_FramebufferName = 0;
  m_sc2map = 0;
  m_controller = 0;

  bufferWidth = 256;
  bufferHeight = 256;
  windowWidth = 256;
  windowHeight = 256;

  m_pathN = 0;
  m_pathT = 0;
}


View::~View()
{
  releaseBuffer();
}



void View::Init(Controller* a_controller)
{
  //
  m_controller = a_controller;

  // TODO GL setup
	// Enable Alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void View::Done()
{
  releaseBuffer();
}


void View::releaseBuffer()
{
	if (m_FramebufferName)
	{
		glDeleteFramebuffers(1, &m_FramebufferName);
		m_FramebufferName = 0;
	}
	
	if (m_renderedTexture)
	{
		glDeleteTextures(1, &m_renderedTexture);
		m_renderedTexture = 0;
	}
}


void View::createBuffer()
{
	// Check for FBO and destroy?
	if (m_FramebufferName)
	{
		glDeleteFramebuffers(1, &m_FramebufferName);
		m_FramebufferName = 0;
	}
	
	if (m_renderedTexture)
	{
		glDeleteTextures(1, &m_renderedTexture);
		m_renderedTexture = 0;
	}
	
	// Create Render Target (once)
	glEnable(GL_TEXTURE_2D);
	
	glGenFramebuffers(1, &m_FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferName);
	
	// The texture we're going to render to
	glGenTextures(1, &m_renderedTexture);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
	
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(
				 GL_TEXTURE_2D, 
				 0, 
				 GL_RGBA8, //GL_RGB, 
				 bufferWidth, 
				 bufferHeight, 
				 0, 
				 GL_RGBA, //GL_RGB, 
				 GL_UNSIGNED_BYTE, 
				 0);
	
	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderedTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void View::DrawScreen()
{
	// Draw screen
	glClearColor(0.92, 0.92, 0.92, 1);
	glClear(GL_COLOR_BUFFER_BIT);	
	
	// Render Frame Map
	if (m_FramebufferName == 0)
	{
		createBuffer();
	} 
	
  if (m_sc2map)
	{	
		// Set Render Target and Draw Map
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferName);
    	
	  // Always check that our framebuffer is ok
	  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
      glViewport(0, 0, bufferWidth, bufferHeight);
		  glClearColor( 0.f, 0.f, 0.f, 1.f );
		  glClear( GL_COLOR_BUFFER_BIT );
		
		  pushScreenCoordinateMatrix();

      if (m_controller)
        m_controller->m_core->OnClientAreaChanged(bufferWidth, bufferHeight);
		
		  DrawMap();
		
		  popScreenCoordinateMatrix();
    }
  }
	
	// Restore frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
  
	if (m_controller)
    m_controller->m_core->OnClientAreaChanged(windowWidth, windowHeight);

	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, windowWidth, 0, windowHeight);
	glPopAttrib();
	
  if (!m_sc2map)
  {
    g_fm.Draw(276 + 24, 32, 1, "Drag and drop SC2Map file here");
  }

	// Display Target as texture
	if (m_FramebufferName)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
		
		glBegin(GL_QUADS);
        glColor4f( 1.0f, 1.0f, 1.0f, 1.f );
		
        int x = (windowWidth - bufferWidth - 270) / 2 + 270;
        int y = (windowHeight - bufferHeight) / 2;
		
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x,  y,  1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + bufferWidth,  y,  1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + bufferWidth,  y + bufferHeight,  1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x,  y + bufferHeight,  1.0f);  // Top Left Of The Texture and Quad

		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Background UI
	UV::Declaration2 decl2;
	
	decl2.Rect.left = 0;
	decl2.Rect.right = 270;
	decl2.Rect.top = 0;
	decl2.Rect.bottom = windowHeight;
	decl2.Color0 = 0xff8a8a8a;
	decl2.Color1 = 0xff8a8a8a;
	decl2.Color2 = 0xff8a8a8a;
	decl2.Color3 = 0xff8a8a8a;
	g_rm.Draw(decl2);	
	
	decl2.Rect.left = 270;
	decl2.Rect.right = 276;
	decl2.Rect.top = 0;
	decl2.Rect.bottom = windowHeight;
	decl2.Color0 = 0xff555555;
	decl2.Color1 = 0xff555555;
	decl2.Color2 = 0xff555555;
	decl2.Color3 = 0xff555555;
	g_rm.Draw(decl2);
    
	decl2.Rect.left = 271;
	decl2.Rect.right = 275;
	decl2.Rect.top = 0;
	decl2.Rect.bottom = windowHeight;
	decl2.Color0 = 0xfffafafa;
	decl2.Color1 = 0xfffafafa;
	decl2.Color2 = 0xffaaaaaa;
	decl2.Color3 = 0xffaaaaaa;
	g_rm.Draw(decl2);
	
	// UV Core
  if (m_controller)
    m_controller->m_core->OnPaint();

	glFlush();	
}


void View::DrawMap()
{
	if (m_sc2map == 0) return;
	
	// Title
	g_fm.Draw(24, 34, 1, m_sc2map->mapName.c_str());
	
	// Legend
	char sp[128];
	std::string a, b;
	int x1 = 24;
	int x2 = 124;
	int y1 = 60;
	
	// Playable area
	a = std::string("Playable area");
	_snprintf(sp, 128, "%d", m_sc2map->cxDimPlayable);
	b = std::string(sp);
	b += std::string(" x ");
	_snprintf(sp, 128, "%d", m_sc2map->cyDimPlayable);
	b += std::string(sp);
	
	g_fm.Draw(x1, y1, 2, a.c_str());
	g_fm.Draw(x2, y1, 2, b.c_str());
	y1 += 20;
	
	// # Bases
	a = std::string("Bases identified");
	_snprintf(sp, 128, "%d", (int)m_sc2map->bases.size());
	b = std::string(sp);
	
	g_fm.Draw(x1, y1, 2, a.c_str());
	g_fm.Draw(x2, y1, 2, b.c_str());
	y1 += 20;	
	
	// Total resources
	a = std::string("Total resources");
	_snprintf(sp, 128, "%d", (int)m_sc2map->totalMinerals);
	b = std::string(sp) +
	std::string(" M   ");
	_snprintf(sp, 128, "%d", (int)m_sc2map->totalVespeneGas);
	b += std::string(sp) +
	std::string(" G   ");
	
	g_fm.Draw(x1, y1, 2, a.c_str());
	g_fm.Draw(x2, y1, 2, b.c_str());
	y1 += 20;	
	
	// Avg. openness
	a = std::string("Avg openness");
	_snprintf(sp, 128, "%f", m_sc2map->opennessAvg[PATH_GROUND_NOROCKS]);
	b = std::string(sp);
	
	g_fm.Draw(x1, y1, 2, a.c_str());
	g_fm.Draw(x2, y1, 2, b.c_str());
	y1 += 20;
	
	// Footer
	a = std::string("SC Map Analyser for HotS alpha, algorithms v1.62");
	g_fm.Draw(x1, bufferHeight - 20, 2, a.c_str());
	
	// Map grids:
	
	// Openness
	unsigned long colour = 0;
	UV::Declaration2 drawRect;
	for (int i = 0; i < m_sc2map->cxDimPlayable; ++i)
	{
		for (int j = 0; j < m_sc2map->cyDimPlayable; ++j)
		{ 
			colour = ColorToDword(m_sc2map->getColor("default"));
			
			// Pathing
			if (IsPathable(i, j, PATH_GROUND_WITHROCKS_NORESOURCES))
			{
				// Draw Quad
				int hx = playableToImgX(i);
				int hy = playableToImgY(j);
				
				drawRect.Color0 = colour;
				drawRect.Color1 = colour;
				drawRect.Color2 = colour;
				drawRect.Color3 = colour;
				drawRect.Rect.left = hx;
				drawRect.Rect.right = hx + 4;
				drawRect.Rect.top = hy;
				drawRect.Rect.bottom = hy + 4;
				g_rm.Draw(drawRect);
			}
			
			// Openness
      if (m_controller->ShowOpennessLayer())
			{
				//float openness = sc2map.GetOpenness(i, j, PATH_GROUND_NOROCKS) / 14.0;
				float openness = m_sc2map->mapOpenness[NUM_PATH_TYPES*(j * m_sc2map->cxDimPlayable + i) + PATH_GROUND_NOROCKS];
				openness /= 14.f;
				if (openness > 1.f) openness = 1.f;
				
				if (openness > 0)
				{
					float colourWeight1 = openness < 0.5f ? 1.f - openness * 2.f : 0.f;
					float colourWeight2 = 1.f - 2.f * fabs(openness - 0.5f);
					float colourWeight3 = openness > 0.5f ? (openness - 0.5f) * 2.f : 0.f;
					
          Color* color1 = 0;
          Color* color2 = 0;
          Color* color3 = 0;

          char h = m_sc2map->mapHeight[j * m_sc2map->txDimPlayable + i];
          switch (h)
          {
            case 1:
              color1 = m_sc2map->getColor("oplo1");
              color2 = m_sc2map->getColor("oplo2");
              color3 = m_sc2map->getColor("oplo3");
              break;
            case 2:
              color1 = m_sc2map->getColor("opmid1");
              color2 = m_sc2map->getColor("opmid2");
              color3 = m_sc2map->getColor("opmid3");
              break;
            case 3:
              color1 = m_sc2map->getColor("ophi1");
              color2 = m_sc2map->getColor("ophi2");
              color3 = m_sc2map->getColor("ophi3");
              break;
          }

					u8 r1 = 255 * (color1 ? color1->r : 0);
					u8 g1 = 255 * (color1 ? color1->g : 0);
					u8 b1 = 255 * (color1 ? color1->b : 0);
					
					u8 r2 = 255 * (color2 ? color2->r : 0);
					u8 g2 = 255 * (color2 ? color2->g : 0);
					u8 b2 = 255 * (color2 ? color2->b : 0);
					
					u8 r3 = 255 * (color3 ? color3->r : 0);
					u8 g3 = 255 * (color3 ? color3->g : 0);
					u8 b3 = 255 * (color3 ? color3->b : 0);
					
					u8 r = (u8)(colourWeight1 * (float)r1 + colourWeight2 * (float)r2 + colourWeight3 * (float)r3);
					u8 g = (u8)(colourWeight1 * (float)g1 + colourWeight2 * (float)g2 + colourWeight3 * (float)g3);
					u8 b = (u8)(colourWeight1 * (float)b1 + colourWeight2 * (float)b2 + colourWeight3 * (float)b3);
					
					colour = DWORD_ARGB((int)(255.0 * m_controller->GetAlpha2()), r, g, b);
					
					// Draw Quad
					int hx = playableToImgX(i);
					int hy = playableToImgY(j);
					
					drawRect.Color0 = colour;
					drawRect.Color1 = colour;
					drawRect.Color2 = colour;
					drawRect.Color3 = colour;
					drawRect.Rect.left = hx;
					drawRect.Rect.right = hx + 4;
					drawRect.Rect.top = hy;
					drawRect.Rect.bottom = hy + 4;
					g_rm.Draw(drawRect);
				}
			}
		}
	}	
	
	// Show LoS Blockers
  if (m_controller->ShowLoS())
	{
		for (list<LoSB*>::iterator i = m_sc2map->losbs.begin(); i != m_sc2map->losbs.end(); ++i)
		{
			colour = ColorToDword(m_sc2map->getColor("LoSB"));
			
			// Draw Quad
			int hx = mapToImgX((*i)->loc.mtx - 2);
			int hy = mapToImgY((*i)->loc.mty);
			
			drawRect.Color0 = colour;
			drawRect.Color1 = colour;
			drawRect.Color2 = colour;
			drawRect.Color3 = colour;
			drawRect.Rect.left = hx;
			drawRect.Rect.right = hx + 4;
			drawRect.Rect.top = hy;
			drawRect.Rect.bottom = hy + 4;
			g_rm.Draw(drawRect);
		}
	}
	
	// Show Siegable 
  if (m_controller->SiegeMapType())
	{
		if (Common::s_genPathType != m_controller->SiegeMapType())
			Common::GenerateSiegeMap(m_controller->SiegeMapType());
		
		if (Common::siegeMap == 0)
			Common::GenerateSiegeMap(m_controller->SiegeMapType());
		
		for (int i = 0; i < m_sc2map->cxDimPlayable; ++i)
		{
			for (int j = 0; j < m_sc2map->cyDimPlayable; ++j)
			{      
				int r = 0, g = 0, b = 0;
				float s = Common::siegeMap[i + j * m_sc2map->cxDimPlayable];
				if (s)
				{					
					//float intensity = s;
					//if (intensity > 1.f) intensity = 1.f; 
					//if (intensity < 0.f) intensity = 0.f; 

          Color* siegeColor = m_sc2map->getColor("siege");
					
					r = 255 * (siegeColor ? siegeColor->r : 0);
					g = 255 * (siegeColor ? siegeColor->g : 0);
					b = 255 * (siegeColor ? siegeColor->b : 0);

					colour = DWORD_ARGB((int)(255.0 * m_controller->GetAlpha3()), r, g, b);
					
					// Draw Quad
					int hx = playableToImgX(i);
					int hy = playableToImgY(j);
					
					drawRect.Color0 = colour;
					drawRect.Color1 = colour;
					drawRect.Color2 = colour;
					drawRect.Color3 = colour;
					drawRect.Rect.left = hx;
					drawRect.Rect.right = hx + 4;
					drawRect.Rect.top = hy;
					drawRect.Rect.bottom = hy + 4;
					g_rm.Draw(drawRect);
				}        
			}
		}
	}
	
	// Height
  if (m_controller->ShowHeightLayer())
	{
		char h = 0;
		for (int i = 0; i < m_sc2map->cxDimPlayable; ++i)
		{
			for (int j = 0; j < m_sc2map->cyDimPlayable; ++j)
			{
				h = m_sc2map->mapHeight[j * m_sc2map->txDimPlayable + i];
				
				if (h == 1)
				{
					colour = ColorToDword(m_sc2map->getColor("terrainElev1"));
				}
				else if (h == 2)
				{
					colour = ColorToDword(m_sc2map->getColor("terrainElev2"));
				}
				else if (h == 3)
				{
					colour = ColorToDword(m_sc2map->getColor("terrainElev3"));
				}
				
				if (IsPathable(i, j, PATH_GROUND_NOROCKS))
				{
					int hx = playableToImgX(i);
					int hy = playableToImgY(j);
					
					drawRect.Color0 = colour;
					drawRect.Color1 = colour;
					drawRect.Color2 = colour;
					drawRect.Color3 = colour;
					
					// Cross 1
					drawRect.Rect.left = hx;
					drawRect.Rect.right = hx + 2;
					drawRect.Rect.top = hy;
					drawRect.Rect.bottom = hy + 1;
					g_rm.Draw(drawRect);
					
					// Cross 2
					drawRect.Rect.left = hx;
					drawRect.Rect.right = hx + 1;
					drawRect.Rect.top = hy;
					drawRect.Rect.bottom = hy + 2;
					g_rm.Draw(drawRect);
					
					// Cross 3
					drawRect.Rect.left = hx + 3;
					drawRect.Rect.right = hx + 4;
					drawRect.Rect.top = hy;
					drawRect.Rect.bottom = hy + 1;
					g_rm.Draw(drawRect);
					
					// Cross 4
					drawRect.Rect.left = hx;
					drawRect.Rect.right = hx + 1;
					drawRect.Rect.top = hy + 3;
					drawRect.Rect.bottom = hy + 4;
					g_rm.Draw(drawRect);
				}
			}
		}    
	}	
	
	// Destruct
  if (m_controller->ShowResourceLayer())
	{
		for (list<Destruct*>::iterator i = m_sc2map->destructs.begin(); i != m_sc2map->destructs.end(); ++i)
		{
			drawRect.Rect.left = mapToImgX((*i)->loc.mtx - 1);
			drawRect.Rect.right = drawRect.Rect.left + 4;
			drawRect.Rect.top = mapToImgY((*i)->loc.mty + 1);
			drawRect.Rect.bottom = drawRect.Rect.top + 4;
			drawRect.Color0 = 0xff4040ff;
			drawRect.Color1 = 0xff4040ff;
			drawRect.Color2 = 0xff4040ff;
			drawRect.Color3 = 0xff4040ff;
			g_rm.Draw(drawRect);
		}
	}
	
	// Pathing
  if (m_controller->ShowPathingLayer() && m_mainPairs.size() > m_pathN && m_natPairs.size() > m_pathN)
	{
		std::list<point> path;
		point p0, p1;
    DWORD c1 = ColorToDword(m_sc2map->getColor("shortestPathAir"));
    DWORD c2 = ColorToDword(m_sc2map->getColor("shortestPathGround"));
    DWORD c3 = ColorToDword(m_sc2map->getColor("shortestPathCWalk")); 
		
		if (m_pathT == 1)
		{
			p0 = m_natPairs[m_pathN].first->loc;
			p1 = m_natPairs[m_pathN].second->loc;
		}
		else
		{
			p0 = m_mainPairs[m_pathN].first->loc;
			p1 = m_mainPairs[m_pathN].second->loc;
		}
		
		float d0 = sqrt((p0.mx - p1.mx) * (p0.mx - p1.mx) + (p0.my - p1.my) * (p0.my - p1.my));
		{
			point pp0 = p0;
			point pp1 = p1;
			pp0.mx = mapToImgX(p0.mx - 1);
			pp0.my = mapToImgY(p0.my + 1);
			pp1.mx = mapToImgX(p1.mx - 1);
			pp1.my = mapToImgY(p1.my + 1);
			path.push_back(pp0);
			path.push_back(pp1);
		}
		g_rm.DrawPath(path, -2, c1);
		path.clear();

		{
			path.push_back(p1);
		}
    float d1 = pathTo(p0, p1, PATH_GROUND_WITHROCKS, path);
		for (std::list<point>::iterator k = path.begin(); k != path.end(); ++k)
		{
			(*k).mx = mapToImgX((*k).mx - 1);
			(*k).my = mapToImgY((*k).my + 1);
		}
		g_rm.DrawPath(path, 0, c2);
		path.clear();
		
		{
			path.push_back(p1);
		}
		float d2 = pathTo(p0, p1, PATH_CWALK_WITHROCKS, path);
		for (std::list<point>::iterator k = path.begin(); k != path.end(); ++k)
		{
			(*k).mx = mapToImgX((*k).mx - 1);
			(*k).my = mapToImgY((*k).my + 1);
		}
		g_rm.DrawPath(path, 2, c3);
		path.clear();		
		
		// Legend
		std::string title = "";
		
		if (m_pathT == 1)
		{
			std::string a = "";
			a += "nat (";
			//_snprintf(sp, 32, "%d", (int)m_natPairs[m_pathN].first->sl->idNum);
			//a += std::string(sp);
      a += m_mainPairs[m_pathN].first->sl->name;
			a += ") to nat (";
			//_snprintf(sp, 32, "%d", (int)m_natPairs[m_pathN].second->sl->idNum, &(sp[0]));
			//a += std::string(sp);
      a += m_mainPairs[m_pathN].second->sl->name;
			a += ")";
			title = a;
		}
		else
		{
			std::string a = "";
			a += "main (";
			//_snprintf(sp, 32, "%d", (int)m_mainPairs[m_pathN].first->sl->idNum, &(sp[0]));
			//a += std::string(sp);
      a += m_mainPairs[m_pathN].first->sl->name;
			a += ") to main (";
			//_snprintf(sp, 32, "%d", (int)m_mainPairs[m_pathN].second->sl->idNum, &(sp[0]));
			//a += std::string(sp);
      a += m_mainPairs[m_pathN].second->sl->name;
			a += ")";
			title = a;
		}
		
		g_fm.Draw(bufferWidth - 235, 34, 1, title.c_str());
		
		drawRect.Rect.left = bufferWidth - 233;
		drawRect.Rect.right = drawRect.Rect.left + 12;
		drawRect.Rect.top = 55;
		drawRect.Rect.bottom = drawRect.Rect.top + 12;
		drawRect.Color0 = c1;
		drawRect.Color1 = c1;
		drawRect.Color2 = c1;
		drawRect.Color3 = c1;
		g_rm.Draw(drawRect);
		
		drawRect.Rect.left = bufferWidth - 233;
		drawRect.Rect.right = drawRect.Rect.left + 12;
		drawRect.Rect.top = 79;
		drawRect.Rect.bottom = drawRect.Rect.top + 12;
		drawRect.Color0 = c2;
		drawRect.Color1 = c2;
		drawRect.Color2 = c2;
		drawRect.Color3 = c2;
		g_rm.Draw(drawRect);
		
		drawRect.Rect.left = bufferWidth - 233;
		drawRect.Rect.right = drawRect.Rect.left + 12;
		drawRect.Rect.top = 103;
		drawRect.Rect.bottom = drawRect.Rect.top + 12;
		drawRect.Color0 = c3;
		drawRect.Color1 = c3;
		drawRect.Color2 = c3;
		drawRect.Color3 = c3;
		g_rm.Draw(drawRect);
		
		_snprintf(sp, 128, "%.1f", d0);
		title = std::string("Air: ") + std::string(sp);
		_snprintf(sp, 128, "%.1f", d0 / OVERLORD_SPEED);
		title += std::string(" (Overlord: ") + std::string(sp) + std::string("s)");
		g_fm.Draw(bufferWidth - 215, 67, 2, title.c_str());
		
		_snprintf(sp, 128, "%.1f", d1);
		title = std::string("Ground: ") + std::string(sp);
		_snprintf(sp, 128, "%.1f", d1 / PROBE_SPEED);
		title += std::string(" (Probe: ") + std::string(sp) + std::string("s)");
		g_fm.Draw(bufferWidth - 215, 91, 2, title.c_str());
		
		_snprintf(sp, 128, "%.1f", d2);
		title = std::string("Cliff: ") + std::string(sp);
		_snprintf(sp, 128, "%.1f", d2 / REAPER_SPEED);
		title += std::string(" (Reaper: ") + std::string(sp) + std::string("s)");
		g_fm.Draw(bufferWidth - 215, 115, 2, title.c_str());
	}
	
	
	// Bases + Resources
  if (m_controller->ShowPhotonOvercharge())
	{
		for (list<Base*>::iterator i = m_sc2map->bases.begin(); i != m_sc2map->bases.end(); ++i)
		{
			if (isClearNexus((*i)->loc))
			{
				drawRect.Rect.left = mapToImgX((*i)->loc.mtx - 1) - 4 * 2;
				drawRect.Rect.right = drawRect.Rect.left + 4 * 5;
				drawRect.Rect.top = mapToImgY((*i)->loc.mty + 1) - 4 * 2;
				drawRect.Rect.bottom = drawRect.Rect.top + 4 * 5;
				drawRect.Color0 = 0xffa4a4a4;
				drawRect.Color1 = 0xffa4a4a4;
				drawRect.Color2 = 0xffa4a4a4;
				drawRect.Color3 = 0xffa4a4a4;
				drawRect.Fill = true;
				g_rm.Draw(drawRect);
				
				drawRect.Color0 = 0xffffffff;
				drawRect.Color1 = 0xffffffff;
				drawRect.Color2 = 0xffffffff;
				drawRect.Color3 = 0xffffffff;
				drawRect.Fill = false;
				g_rm.Draw(drawRect);
				
				drawRect.Fill = true;
				
				g_rm.DrawCircle(
								mapToImgX((*i)->loc.mtx) - 2,
								mapToImgY((*i)->loc.mty) - 2, 
								4.f * 13.f); // Photon Overcharge range
			}
			else
			{
				drawRect.Rect.left = mapToImgX((*i)->loc.mtx - 1) - 4 * 2;
				drawRect.Rect.right = drawRect.Rect.left + 4 * 5;
				drawRect.Rect.top = mapToImgY((*i)->loc.mty + 1) - 4 * 2;
				drawRect.Rect.bottom = drawRect.Rect.top + 4 * 5;
				drawRect.Color0 = 0xff4a40bb;
				drawRect.Color1 = 0xffa4a4bb;
				drawRect.Color2 = 0xff4a40bb;
				drawRect.Color3 = 0xffa4a4bb;
				g_rm.Draw(drawRect);
			}
		}
	}
	
	// Watchtower
  if (m_controller->ShowWatchTower())
	{
		for (list<Watchtower*>::iterator i = m_sc2map->watchtowers.begin(); i != m_sc2map->watchtowers.end(); ++i)
		{
			g_rm.DrawCircle(mapToImgX((*i)->loc.mtx - 1), mapToImgY((*i)->loc.mty), 4.f * (*i)->range); // sight range
			//g_rm.DrawCircle(mapToImgX((*i)->loc.mtx - 1), mapToImgY((*i)->loc.mty), 4.f * 2.5f); // activation range
		}
	}
	
	// Start Locations
  if (m_controller->ShowBases())
	{
		for (list<StartLoc*>::iterator i = m_sc2map->startLocs.begin(); i != m_sc2map->startLocs.end(); ++i)
		{
			std::string title = "";
			
			if ((*i)->mainBase)
			{	
				int basex = mapToImgX((*i)->mainBase->loc.mtx);
				int basey = mapToImgY((*i)->mainBase->loc.mty);
				
				title = std::string("Main @ ") + std::string((*i)->name);
				g_fm.Draw(basex - 26, basey + 12, 2, title.c_str());
				/*
				 snprintf(sp, 32, "%d", (*i)->spaceInMain);
				 title = "space: " + std::string(sp);
				 g_fm.Draw(basex - 26, basey + 12 + 16, 2, title.c_str());
				 
				 float numCCs = ((float)(*i)->spaceInMain) / 25.0f;
				 snprintf(sp, 32, "%d", (int)numCCs);
				 title = "(" + std::string(sp) + " CCs)";
				 g_fm.Draw(basex - 26, basey + 12 + 32, 2, title.c_str());
				 */
			}
			
			if ((*i)->natBase)
			{
				title = "Natural";
				int basex = mapToImgX((*i)->natBase->loc.mtx);
				int basey = mapToImgY((*i)->natBase->loc.mty);
				g_fm.Draw(basex - 26, basey + 12, 2, title.c_str());
			}
			
			if ((*i)->thirdBase)
			{
				title = "Third";
				int basex = mapToImgX((*i)->thirdBase->loc.mtx);
				int basey = mapToImgY((*i)->thirdBase->loc.mty);
				g_fm.Draw(basex - 26, basey + 12, 2, title.c_str());
			}
		}
	} 
}


/// A fairly straight forward function that pushes
/// a projection matrix that will make object world 
/// coordinates identical to window coordinates.
void View::pushScreenCoordinateMatrix() 
{
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, bufferWidth, 0, bufferHeight);
	glPopAttrib();
}



/// Pops the projection matrix without changing the current
/// MatrixMode.
void View::popScreenCoordinateMatrix() 
{
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}


void View::calcBases()
{
	// find the best nexus location for each base
	for (list<Base*>::iterator b = m_sc2map->bases.begin(); b != m_sc2map->bases.end(); ++b)
	{
		// start from the average resource spot
		// check 8x8 region
		// if nexus can be placed, find avg resource distance
		Base* base = *b;
		point loc = base->loc;
		
		float minD = -1.f;
		
		for (int i = -11; i <= 11; ++i)
		{
			for (int j = -11; j <= 11; ++j)
			{
				loc.set(&(base->loc));
				
				loc.mx += i;  loc.my += j;
				loc.mtx += i; loc.mty += j;
				loc.mcx += i; loc.mcy += j;
				loc.ptx += i; loc.pty += j;
				loc.pcx += i; loc.pcy += j;
				loc.ix += i;  loc.iy += j;
				
				if (isClearNexus(loc) && base)
				{
					float D = 0.f;
					const list<Resource*>& baseResources = base->resources;
					for (list<Resource*>::const_iterator r = baseResources.begin(); r != baseResources.end(); ++r)
					{
						//if ((*r)->type == MINERALS)
						{
							int dx = (*r)->loc.mtx - loc.mtx;
							int dy = (*r)->loc.mty - loc.mty;
							
							float d = sqrt((float)(dx * dx + dy * dy));
							D += d;
						}
					}
					if (D > 0 && (minD < 0.f || D < minD))
					{
						minD = D;
						base->loc.set(&loc);
					} 
				}
			}
		}
	}
}


bool View::isClearNexus(const point& a_loc)
{
	for (int i = -2; i <= 2; ++i)
	{
		for (int j = -2; j <= 2; ++j)
		{
			if (!IsPathable(a_loc.ptx + i, a_loc.pty + j, PATH_BUILDABLE_MAIN))
				return false;
		}
	}
	
	return true;
}


bool View::IsPathable(int i, int j, int t)
{
  if (m_sc2map == 0 || m_sc2map->mapPathing == 0) return false;
  if (t >= NUM_PATH_TYPES) return false;
	if (i < 0) return false;
	if (i >= m_sc2map->cxDimPlayable) return false;
	if (j < 0) return false;
	if (j >= m_sc2map->cyDimPlayable) return false;
	return m_sc2map->mapPathing[NUM_PATH_TYPES * (j * m_sc2map->cxDimPlayable + i) + t];
}


int View::imgX(int x) { return 4 * x + 16; }
int View::imgY(int y) { return 4 * y + 160; }


int View::playableToImgX(int x)
{
	return imgX(x);
}


int View::playableToImgY(int y)
{
	y = m_sc2map->cyDimPlayable - y;
	return imgY(y);
}


int View::mapToImgX(int x)
{
	x = x - m_sc2map->tLeftBottom.mx + 1;
	return imgX(x);
}


int View::mapToImgY(int y)
{
	y = m_sc2map->cyDimPlayable - y + (int)m_sc2map->tLeftBottom.my + 1;
	return imgY(y);
}


void View::OnClientAreaChanged(int a_width, int a_height)
{
	// set bounds
	windowWidth = a_width;
	windowHeight = a_height;
}


void View::SetMap(SC2Map* a_map)
{
  m_sc2map = a_map;
  if (m_sc2map)
  {
	  bufferWidth = m_sc2map->txDimPlayable * 4 + 32;
	  bufferHeight = m_sc2map->tyDimPlayable * 4 + 240;
  }

  releaseBuffer();

  CreatePathingLayout();
  calcBases();
}


void View::IncPath(int a_inc)
{
	m_pathN += a_inc;
	if (m_pathT == 1)
	{
		if (m_pathN == m_natPairs.size())
		{
			m_pathN = 0;
			m_pathT = 0;
		}
		else if (m_pathN < 0)
		{
			m_pathN = m_mainPairs.size() - 1;
			m_pathT = 0;
		}
	}
	else
	{
		if (m_pathN == m_mainPairs.size())
		{
			m_pathN = 0;
			m_pathT = 1;
		}
		else if (m_pathN < 0)
		{
			m_pathN = m_natPairs.size() - 1;
			m_pathT = 1;
		}
	}
}


void View::CreatePathingLayout()
{
	// Enumerate bases
	std::vector<Base*> mains;
	std::vector<Base*> nats;
	//for (list<Base*>::iterator i = m_sc2map->bases.begin(); i != m_sc2map->bases.end(); ++i)
	//{
 //   Base* base = *i;
	//	if (base->numExpo == MAIN_EXPO) 
 //     mains.push_back(base);
	//	else if (base->numExpo == NATURAL_EXPO) 
 //     nats.push_back(base);
	//}
	for (list<StartLoc*>::iterator i = m_sc2map->startLocs.begin(); i != m_sc2map->startLocs.end(); ++i)
	{
    if ((*i)->mainBase)
    {
      mains.push_back((*i)->mainBase);
    }

    if ((*i)->natBase)
    {
      nats.push_back((*i)->natBase);
    }
  }
	
	// Enumerate path pairs
	m_mainPairs.clear();
	for (vector<Base*>::iterator i = mains.begin(); i != mains.end(); ++i)
	{
		for (vector<Base*>::iterator j = i + 1; j != mains.end(); ++j)
		{
			std::pair< Base*, Base* > b = std::pair< Base*, Base* >((Base*)(*i), (Base*)*j);
			m_mainPairs.push_back( b );
		}
	}
    
	m_natPairs.clear();
	for (vector<Base*>::iterator i = nats.begin(); i != nats.end(); ++i)
	{
		for (vector<Base*>::iterator j = i + 1; j != nats.end(); ++j)
		{
			std::pair< Base*, Base* > b = std::pair< Base*, Base* >((Base*)(*i), (Base*)*j);
			m_natPairs.push_back( b );
		}
	} 
}


float View::pathTo(point& p0, point& p1, PathType t, std::list<point>& path)
{
	// paths...
	Node* src = m_sc2map->getPathNode( &p0, t );
	Node* u   = m_sc2map->getPathNode( &p1, t );
	
	if (src == NULL || u == NULL) return 0;
	
	Node* v   = m_sc2map->getShortestPathPredecessor( src, u, t );
	
	float total_d = m_sc2map->getShortestPathDistance( src, u, t );
	
	while( v != NULL )
	{
		path.push_back(v->loc);
		
		u = v;
		v = m_sc2map->getShortestPathPredecessor( src, u, t );
	}
	
	return total_d;
}


void View::SaveImage(const char* a_fileName)
{	
	glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferName); // Set FBO
	
	float* pixelbuffer = new float[bufferWidth * bufferHeight * 4];
	GLvoid* pixels =  (GLvoid*)pixelbuffer;
	glReadPixels(0, 0, bufferWidth, bufferHeight, GL_RGBA, GL_FLOAT, pixels);
  //glReadPixels(0, 0, bufferWidth, bufferHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// set up image params
	int size[2];
	size[0] = bufferWidth;
	size[1] = bufferHeight;
	
	pngwriter w(bufferWidth, bufferHeight, 0, a_fileName);
	
	double r = 0;
	double g = 0;
	double b = 0;
	
	for (int i = 0; i < bufferWidth; ++i)
	{
		for (int j = 0; j < bufferHeight; ++j)
		{
			r = (double)(pixelbuffer[4 * (j * bufferWidth + i) + 0]);
			g = (double)(pixelbuffer[4 * (j * bufferWidth + i) + 1]);
			b = (double)(pixelbuffer[4 * (j * bufferWidth + i) + 2]);
			
			w.plot(i, j, r, g, b);
		}
	}			
	
	w.close();
}
