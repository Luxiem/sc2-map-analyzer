#include "View.h"
#include "Controller.h"
#include "SC2Map.hpp"
#include "UV/UV.h"
#include <SDL.h>
//#include "FBORenderTexture.h"
#include "Common.h" // siege path stuff
#include <sstream>
#include "SDL_image.h"
#include "colour.h"
#include <SDL_syswm.h> // HWND


// Naughty
#ifdef _WINDOWS
#define snprintf _snprintf
#endif


View::View()
{
  m_sc2map = 0;
  m_controller = 0;

  m_bufferWidth = 256;
  m_bufferHeight = 256;
  m_windowWidth = 256;
  m_windowHeight = 256;

  m_dG = 0.f;
  m_dC = 0.f;
  m_dA = 0.f;

    m_targetSurf = 0;
    m_targetTex = 0;
    
    m_window = 0;
    m_renderer = 0;
}


View::~View()
{
  releaseBuffer();
    
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}



void View::Init(Controller* a_controller)
{    
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

  //
  m_controller = a_controller;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        exit(0);
    }
    
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

    m_window = SDL_CreateWindow("SC2MA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
    
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    g_fm.Init(m_renderer, Common::getPath());
    
    Common::OnClientAreaChanged(640, 480);
    
}


void View::Done()
{
    releaseBuffer();
}


void View::releaseBuffer()
{
	if (m_targetTex)
    {
        SDL_DestroyTexture(m_targetTex);
        m_targetTex = 0;
    }
}


void View::createBuffer()
{
	releaseBuffer();
   
    m_targetTex = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, m_bufferWidth, m_bufferHeight);
}


void View::DrawScreen()
{
	// Draw screen
    SDL_SetRenderDrawColor(m_renderer, 0xa0, 0xa0, 0xa0, 0xff);
    SDL_RenderClear(m_renderer);
    
	// Render Frame Map
    if (m_sc2map)
	{	
        if (m_targetTex == 0)
        {
            createBuffer();
        }

		// Set Render Target and Draw Map
        SDL_SetRenderTarget(m_renderer, m_targetTex);
        SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(m_renderer);

        if (m_controller)
            m_controller->m_core->OnClientAreaChanged(m_bufferWidth, m_bufferHeight);
    	
		
        DrawMap();
        

        SDL_SetRenderTarget(m_renderer, NULL);
    }
	
	if (m_controller)
        m_controller->m_core->OnClientAreaChanged(m_windowWidth, m_windowHeight);
    
	SDL_RenderSetLogicalSize(m_renderer, m_windowWidth, m_windowHeight);
    
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.h = m_windowHeight;
    rect.w = m_windowWidth;
    SDL_RenderSetViewport(m_renderer, &rect);
    
    if (!m_sc2map)
    {
        g_fm.Draw(276 + 24, 32, 1, "Drag and drop SC2Map file here");
    }

	// Display Target as texture
	if (m_targetTex)
	{
        int x = (m_windowWidth - m_bufferWidth - 270) / 2 + 270;
        int y = (m_windowHeight - m_bufferHeight) / 2;

        SDL_Rect dest = {(int)x, (int)y, m_bufferWidth, m_bufferHeight};
        SDL_RenderCopy(m_renderer, m_targetTex, 0, &dest);
	}

	// Background UI
	UV::Declaration2 decl2;
	
	decl2.Rect.left = 0;
	decl2.Rect.right = 270;
	decl2.Rect.top = 0;
	decl2.Rect.bottom = m_windowHeight;
	decl2.Color0 = 0xff8a8a8a;
	decl2.Color1 = 0xff8a8a8a;
	decl2.Color2 = 0xff8a8a8a;
	decl2.Color3 = 0xff8a8a8a;
	g_rm.Draw(decl2);	
	
	decl2.Rect.left = 270;
	decl2.Rect.right = 276;
	decl2.Rect.top = 0;
	decl2.Rect.bottom = m_windowHeight;
	decl2.Color0 = 0xff555555;
	decl2.Color1 = 0xff555555;
	decl2.Color2 = 0xff555555;
	decl2.Color3 = 0xff555555;
	g_rm.Draw(decl2);
    
	decl2.Rect.left = 271;
	decl2.Rect.right = 275;
	decl2.Rect.top = 0;
	decl2.Rect.bottom = m_windowHeight;
	decl2.Color0 = 0xfffafafa;
	decl2.Color1 = 0xfffafafa;
	decl2.Color2 = 0xffaaaaaa;
	decl2.Color3 = 0xffaaaaaa;
	g_rm.Draw(decl2);
	
	// UV Core
    if (m_controller)
        m_controller->m_core->OnPaint();

    SDL_RenderPresent(m_renderer);
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
	int x2 = 144;
	int y1 = 60;
	
	// Playable area
	a = std::string("Playable area");
	snprintf(sp, 128, "%d", m_sc2map->cxDimPlayable);
	b = std::string(sp);
	b += std::string(" x ");
	snprintf(sp, 128, "%d", m_sc2map->cyDimPlayable);
	b += std::string(sp);
	
	g_fm.Draw(x1, y1, 2, a.c_str());
	g_fm.Draw(x2, y1, 2, b.c_str());
	y1 += 20;
	
	// # Bases
	a = std::string("Bases identified");
	snprintf(sp, 128, "%d", (int)m_sc2map->bases.size());
	b = std::string(sp);
	
	g_fm.Draw(x1, y1, 2, a.c_str());
	g_fm.Draw(x2, y1, 2, b.c_str());
	y1 += 20;	
	
	// Total resources
	a = std::string("Total resources");
	snprintf(sp, 128, "%d", (int)m_sc2map->totalMinerals);
	b = std::string(sp) +
	std::string(" M   ");
	snprintf(sp, 128, "%d", (int)m_sc2map->totalVespeneGas);
	b += std::string(sp) +
	std::string(" G   ");
	
	g_fm.Draw(x1, y1, 2, a.c_str());
	g_fm.Draw(x2, y1, 2, b.c_str());
	y1 += 20;	
	
	// Footer
	a = std::string("SC Map Analyser for HotS alpha, algorithms ");
  a += std::string(QUOTEMACRO(VALG));
	g_fm.Draw(x1, m_bufferHeight - 20, 2, a.c_str());
	
	// Map grids:
	
	// Openness
	unsigned long colour = 0;
	UV::Declaration2 drawRect;
	for (int i = 0; i < m_sc2map->cxDimPlayable; ++i)
	{
		for (int j = 0; j < m_sc2map->cyDimPlayable; ++j)
		{ 
			colour = Colour::ColorToDword(m_sc2map->getColor("default"));
			
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
                    Color* color1 = 0;
                    Color* color2 = 0;
                    Color* color3 = 0;
                    DWORD blendedColour = 0;

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
                    
                    if (openness < 0.5f)
                    {
                        blendedColour = Colour::HSVBlend(Colour::ColorToDword(color1), Colour::ColorToDword(color2), 1.f - openness * 2.f);
                    }
                    else
                    {
                        blendedColour = Colour::HSVBlend(Colour::ColorToDword(color2), Colour::ColorToDword(color3), 1.f - (openness - 0.5f) * 2.f);
                    }

					// Draw Quad
					int hx = playableToImgX(i);
					int hy = playableToImgY(j);
					
					drawRect.Color0 = blendedColour;
					drawRect.Color1 = blendedColour;
					drawRect.Color2 = blendedColour;
					drawRect.Color3 = blendedColour;
					drawRect.Rect.left = hx;
					drawRect.Rect.right = hx + 4;
					drawRect.Rect.top = hy;
					drawRect.Rect.bottom = hy + 4;
					g_rm.Draw(drawRect);
				}
			}
            
            // Influence
            if(m_controller->ShowInfluenceLayer())
            {
                if (IsPathable(i, j, PATH_GROUND_WITHROCKS_NORESOURCES))
                {
                    
                
                    int s = m_controller->GetInfluenceSpawn();
                    
                    StartLoc* sl1 = 0;
                    for (std::list<StartLoc*>::iterator i = m_sc2map->startLocs.begin(); s >= 0; ++i)
                    {
                        sl1 = *i;
                        s--;
                    }
                    point p;
                    p.pcSet(i, j);
                    
                    float influence = m_sc2map->weightedInfluenceDistance(sl1, NULL, &p);
                    
                    // Influence to colour
                    Color colour1 = m_sc2map->getColor("influence1");
                    Color colour2 = m_sc2map->getColor("influence2");
                    
                    colour = Colour::HSVBlend(Colour::ColorToDword(&colour1), Colour::ColorToDword(&colour2), influence / 255.f);
                    
                    // Plot influence
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
			colour = Colour::ColorToDword(m_sc2map->getColor("LoSB"));
			
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
					colour = Colour::ColorToDword(m_sc2map->getColor("terrainElev1"), (int)(255.f * m_controller->GetAlpha1()));
				}
				else if (h == 2)
				{
					colour = Colour::ColorToDword(m_sc2map->getColor("terrainElev2"), (int)(255.f * m_controller->GetAlpha1()));
				}
				else if (h == 3)
				{
					colour = Colour::ColorToDword(m_sc2map->getColor("terrainElev3"), (int)(255.f * m_controller->GetAlpha1()));
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
        colour = Colour::ColorToDword(m_sc2map->getColor("destructable"));
		for (list<Destruct*>::iterator i = m_sc2map->destructs.begin(); i != m_sc2map->destructs.end(); ++i)
		{
			drawRect.Rect.left = mapToImgX((*i)->loc.mtx - 1);
			drawRect.Rect.right = drawRect.Rect.left + 4;
			drawRect.Rect.top = mapToImgY((*i)->loc.mty + 1);
			drawRect.Rect.bottom = drawRect.Rect.top + 4;
            
			drawRect.Color0 = colour;
			drawRect.Color1 = colour;
			drawRect.Color2 = colour;
			drawRect.Color3 = colour;
			g_rm.Draw(drawRect);
		}

        colour = Colour::ColorToDword(m_sc2map->getColor("collapsible"));
		for (list<Collapsible*>::iterator i = m_sc2map->collapsibles.begin(); i != m_sc2map->collapsibles.end(); ++i)
		{
			drawRect.Rect.left = mapToImgX((*i)->loc.mtx - 1);
			drawRect.Rect.right = drawRect.Rect.left + 4;
			drawRect.Rect.top = mapToImgY((*i)->loc.mty + 1);
			drawRect.Rect.bottom = drawRect.Rect.top + 4;

            if ((*i)->state)
            {
                drawRect.Color0 = colour;
                drawRect.Color1 = colour;
                drawRect.Color2 = colour;
                drawRect.Color3 = colour;
                
                // ?
                drawRect.Rect.left -= 4;
                drawRect.Rect.right -= 4;
                drawRect.Rect.top += 4;
                drawRect.Rect.bottom += 4;
            }
            else
            {
			  drawRect.Color0 = 0xff50ff50;
			  drawRect.Color1 = 0xff50ff50;
			  drawRect.Color2 = 0xff50ff50;
			  drawRect.Color3 = 0xff50ff50;
            }
			
            g_rm.Draw(drawRect);
		}
	}
	
	// Pathing
    if (m_controller->ShowPathingLayer() && m_pathC.size())
	{
        DWORD c1 = Colour::ColorToDword(m_sc2map->getColor("shortestPathAir"));
        DWORD c2 = Colour::ColorToDword(m_sc2map->getColor("shortestPathGround"));
        DWORD c3 = Colour::ColorToDword(m_sc2map->getColor("shortestPathCWalk"));
		
        //float d0 = 0.f;
		g_rm.DrawPath(m_pathA, -2, c1);
		g_rm.DrawPath(m_pathG, 0, c2);
		g_rm.DrawPath(m_pathC, 2, c3);

    		
		// Legend
		std::string title = "";
		
    /*
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
    */

		std::string a = "";
		a += m_spawnNameA;
    a += " ";
    a += m_baseNameA;
    a += " to ";
		a += m_spawnNameB;
    a += " ";
    a += m_baseNameB;
		title = a;
		
		g_fm.Draw(m_bufferWidth - 235, 34, 1, title.c_str());
		
		drawRect.Rect.left = m_bufferWidth - 233;
		drawRect.Rect.right = drawRect.Rect.left + 12;
		drawRect.Rect.top = 55;
		drawRect.Rect.bottom = drawRect.Rect.top + 12;
		drawRect.Color0 = c1;
		drawRect.Color1 = c1;
		drawRect.Color2 = c1;
		drawRect.Color3 = c1;
		g_rm.Draw(drawRect);
		
		drawRect.Rect.left = m_bufferWidth - 233;
		drawRect.Rect.right = drawRect.Rect.left + 12;
		drawRect.Rect.top = 79;
		drawRect.Rect.bottom = drawRect.Rect.top + 12;
		drawRect.Color0 = c2;
		drawRect.Color1 = c2;
		drawRect.Color2 = c2;
		drawRect.Color3 = c2;
		g_rm.Draw(drawRect);
		
		drawRect.Rect.left = m_bufferWidth - 233;
		drawRect.Rect.right = drawRect.Rect.left + 12;
		drawRect.Rect.top = 103;
		drawRect.Rect.bottom = drawRect.Rect.top + 12;
		drawRect.Color0 = c3;
		drawRect.Color1 = c3;
		drawRect.Color2 = c3;
		drawRect.Color3 = c3;
		g_rm.Draw(drawRect);
		
		snprintf(sp, 128, "%.1f", m_dA);
		title = std::string("Air: ") + std::string(sp);
		snprintf(sp, 128, "%.1f", m_dA / OVERLORD_SPEED);
		title += std::string(" (Overlord: ") + std::string(sp) + std::string("s)");
		g_fm.Draw(m_bufferWidth - 215, 67, 2, title.c_str());
		
		snprintf(sp, 128, "%.1f", m_dG);
		title = std::string("Ground: ") + std::string(sp);
		snprintf(sp, 128, "%.1f", m_dG / PROBE_SPEED);
		title += std::string(" (Probe: ") + std::string(sp) + std::string("s)");
		g_fm.Draw(m_bufferWidth - 215, 91, 2, title.c_str());
		
		snprintf(sp, 128, "%.1f", m_dC);
		title = std::string("Cliff: ") + std::string(sp);
		snprintf(sp, 128, "%.1f", m_dC / REAPER_SPEED);
		title += std::string(" (Reaper: ") + std::string(sp) + std::string("s)");
		g_fm.Draw(m_bufferWidth - 215, 115, 2, title.c_str());
	}
	
	
	// Bases + Resources
    if (m_controller->ShowPhotonOvercharge())
	{
		for (list<Base*>::iterator i = m_sc2map->bases.begin(); i != m_sc2map->bases.end(); ++i)
		{
			if (isClearNexus((*i)->loc))
			{
				drawRect.Rect.left = mapToImgX((*i)->loc.mtx - 2) - 4 * 2 + 1;
				drawRect.Rect.right = drawRect.Rect.left + 4 * 5 - 1;
				drawRect.Rect.top = mapToImgY((*i)->loc.mty) - 4 * 2;
				drawRect.Rect.bottom = drawRect.Rect.top + 4 * 5 - 1;
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
								mapToImgX((*i)->loc.mtx - 1) - 2,
								mapToImgY((*i)->loc.mty - 1) - 2, 
								4.f * 13.f); // Photon Overcharge range
			}
			else
			{
				drawRect.Rect.left = mapToImgX((*i)->loc.mtx - 2) - 4 * 2 + 1;
				drawRect.Rect.right = drawRect.Rect.left + 4 * 5 - 1;
				drawRect.Rect.top = mapToImgY((*i)->loc.mty) - 4 * 2;
				drawRect.Rect.bottom = drawRect.Rect.top + 4 * 5 - 1;
				drawRect.Color0 = 0xffdda0a0;
				drawRect.Color1 = 0xffdda0a0;
				drawRect.Color2 = 0xffdda0a0;
				drawRect.Color3 = 0xffdda0a0;
				g_rm.Draw(drawRect);

				g_rm.DrawCircle(
								mapToImgX((*i)->loc.mtx - 1) - 2,
								mapToImgY((*i)->loc.mty - 1) - 2, 
								4.f * 13.f); // Photon Overcharge range
			}
		}
	}
    
    // Resources
    {
        
    }
    
    // Geyser efficiency
	if (m_controller->ShowVespeneEfficiency())
    {
        int base_time = 1291;
        int dt = 0;
        unsigned long warningColour;
        
        for (list<Base*>::const_iterator i = m_sc2map->bases.begin(); i != m_sc2map->bases.end(); ++i)
        {
            Base* base = *i;
            for (map<Resource*, int>::const_iterator j = base->geyserEfficiency.begin(); j != base->geyserEfficiency.end(); ++j)
            {
                Resource* resource = j->first;
                dt = j->second - base_time;
                
                if (dt < 0)
                    warningColour = 0xff000000;
                else if (dt == 0)
                    warningColour = 0xff3366ff;
                else if (dt < 15)
                    warningColour = 0xff33cccc;
                else if (dt < 30)
                    warningColour = 0xff99cc00;
                else if (dt < 50)
                    warningColour = 0xffffcc00;
                else if (dt < 90)
                    warningColour = 0xffff6600;
                else if (dt < 150)
                    warningColour = 0xffff0000;
                
                
                drawRect.Rect.left = mapToImgX(resource->loc.mtx - 3);
				drawRect.Rect.right = drawRect.Rect.left + 4 * 3;
				drawRect.Rect.top = mapToImgY(resource->loc.mty + 1);
				drawRect.Rect.bottom = drawRect.Rect.top + 4 * 3;
				drawRect.Color0 = warningColour;
				drawRect.Color1 = warningColour;
				drawRect.Color2 = warningColour;
				drawRect.Color3 = warningColour;
				g_rm.Draw(drawRect);
                
                
            }
        }
    }
	
	// Watchtower
    if (m_controller->ShowWatchTower())
	{
        SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 0xff);

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


bool View::isClearNexus(const point& a_loc)
{
	for (int i = -2; i <= 2; ++i)
	{
		for (int j = -2; j <= 2; ++j)
		{
			if (!IsPathable(a_loc.ptx + i - 1, a_loc.pty + j - 1, PATH_BUILDABLE_MAIN))
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
	m_windowWidth = a_width;
	m_windowHeight = a_height;
}


void View::SetMap(SC2Map* a_map)
{
  m_sc2map = a_map;
  if (m_sc2map)
  {
	  m_bufferWidth = m_sc2map->txDimPlayable * 4 + 32;
	  m_bufferHeight = m_sc2map->tyDimPlayable * 4 + 240;
  }

  releaseBuffer();

  SetPath(0, 0, 1, 0);
}


void View::SetPath(int a_pathSpawnA, int a_pathBaseA, int a_pathSpawnB, int a_pathBaseB)
{
    int spawnCounter = 0;
    Base* A = 0;
    Base* B = 0;
    std::string names[] = { "main", "nat", "3rd", "4th" };

    for (list<StartLoc*>::iterator i = m_sc2map->startLocs.begin(); i != m_sc2map->startLocs.end(); ++i)
    {
        if (spawnCounter == a_pathSpawnA)
        {
            if ((*i)->mainBase && a_pathBaseA == 0)
            {
                A = (*i)->mainBase;
                m_spawnNameA = (*i)->name;
                m_baseNameA = names[a_pathBaseA];
            }
            else if ((*i)->natBase && a_pathBaseA == 1)
            {
                A = (*i)->natBase;
                m_spawnNameA = (*i)->name;
                m_baseNameA = names[a_pathBaseA];
            }
            else if ((*i)->thirdBase && a_pathBaseA == 2)
            {
                A = (*i)->thirdBase;
                m_spawnNameA = (*i)->name;
                m_baseNameA = names[a_pathBaseA];
            }
        }

        if (spawnCounter == a_pathSpawnB)
        {
            if ((*i)->mainBase && a_pathBaseB == 0)
            {
                B = (*i)->mainBase;
                m_spawnNameB = (*i)->name;
                m_baseNameB = names[a_pathBaseB];
            }
            else if ((*i)->natBase && a_pathBaseB == 1)
            {
                B = (*i)->natBase;
                m_spawnNameB = (*i)->name;
                m_baseNameB = names[a_pathBaseB];
            }
            else if ((*i)->thirdBase && a_pathBaseB == 2)
            {
                B = (*i)->thirdBase;
                m_spawnNameB = (*i)->name;
                m_baseNameB = names[a_pathBaseB];
            }
        }

        ++spawnCounter;
    }


    if (A && B)
    {
        m_pathG.clear();
        m_pathC.clear();

        point p0 = A->loc;
        point p1 = B->loc;
    
        {
            m_pathG.push_back(p1);
            m_pathC.push_back(p1);
        }

        m_dG = pathTo(p0, p1, PATH_GROUND_WITHROCKS, m_pathG);
        for (std::list<point>::iterator k = m_pathG.begin(); k != m_pathG.end(); ++k)
        {
            (*k).mx = mapToImgX((*k).mx - 1);
            (*k).my = mapToImgY((*k).my + 1);
        }
      
        m_dC = pathTo(p0, p1, PATH_CWALK_WITHROCKS, m_pathC);
        for (std::list<point>::iterator k = m_pathC.begin(); k != m_pathC.end(); ++k)
        {
            (*k).mx = mapToImgX((*k).mx - 1);
            (*k).my = mapToImgY((*k).my + 1);
        }

		m_dA = sqrt((p0.mx - p1.mx) * (p0.mx - p1.mx) + (p0.my - p1.my) * (p0.my - p1.my));
		m_pathA.clear();

		{
			point pp0 = p0;
			point pp1 = p1;
			pp0.mx = mapToImgX(p0.mx - 1);
			pp0.my = mapToImgY(p0.my + 1);
			pp1.mx = mapToImgX(p1.mx - 1);
			pp1.my = mapToImgY(p1.my + 1);
			m_pathA.push_back(pp0);
			m_pathA.push_back(pp1);
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
    int pitch = 32 * m_bufferWidth;
    void* pixelData = malloc(pitch * m_bufferHeight);
    void* pixelDataFlip = malloc(pitch * m_bufferHeight);
    int bpp = 32;
    Uint32 Rmask = 0, Gmask = 0, Bmask = 0, Amask = 0;
    Uint32 format = SDL_PIXELFORMAT_ARGB8888; //m_targetTex->format;
    
    // HACK Paint map onto backbuffer (SDL is broken - https://bugzilla.libsdl.org/show_bug.cgi?id=2705)
	SDL_SetRenderTarget(m_renderer, NULL);
	SDL_Rect dest = {0, 0, m_bufferWidth, m_bufferHeight};
    SDL_RenderCopy(m_renderer, m_targetTex, 0, &dest);
	SDL_RenderPresent(m_renderer);

	SDL_SetRenderTarget(m_renderer, m_targetTex);
	int readResult = SDL_RenderReadPixels(m_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, pixelData, pitch);
	SDL_SetRenderTarget(m_renderer, NULL);

    //SDL_SetRenderTarget(m_renderer, m_targetTex);
    //int readResult = SDL_RenderReadPixels(m_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, pixelData, pitch);
    //SDL_SetRenderTarget(m_renderer, NULL);
	
    /*
	for (int i = 0; i < m_bufferHeight; ++i)
    {
        memcpy((char*)pixelDataFlip + (m_bufferHeight - i - 1) * pitch, (char*)pixelData + i * pitch, pitch);
    }
	*/
    
    SDL_PixelFormatEnumToMasks(format, &bpp, &Rmask, &Gmask, &Bmask, &Amask);
    
    m_targetSurf = SDL_CreateRGBSurfaceFrom(pixelData, m_bufferWidth, m_bufferHeight, bpp, pitch, Rmask, Gmask, Bmask, Amask);
    
    int saveResult = IMG_SavePNG(m_targetSurf, a_fileName);
    
    SDL_FreeSurface(m_targetSurf);
    m_targetSurf = 0;
    
    free(pixelData);
    free(pixelDataFlip);
}


#ifdef _WINDOWS
HWND View::getHwnd()
{
	struct SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version); 
	SDL_GetWindowWMInfo(m_window, &wmInfo);
	return wmInfo.info.win.window;
}
#endif
