#include "View.h"
#include "Controller.h"
#include "Common.h"
#include <colour.h>
#include "SC2Map.hpp"
#include <sstream>
#include <stdio.h>
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifdef  _WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


//static ImVec4 clear_color = ImColor(114, 144, 154);
static ImVec4 clear_color = ImColor(114, 114, 114);


//
// Declaration to be defined in main.cpp (platform specific)
//
#ifdef _WINDOWS
void LoadMapFile(HWND a_hwnd);
void SaveImageFile(HWND a_hwnd);
#endif


View::View()
{
  m_sc2map = 0;
  settings = 0;

  m_bufferWidth = 0;
  m_bufferHeight = 0;

  m_dG = 0.f;
  m_dC = 0.f;
  m_dA = 0.f;

  m_updateMap = false;

  m_frameBuffer = 0;
  m_renderTexture = 0;

  m_pathSelectA = 0;
  m_pathSelectB = 0;
  m_pathSelectC = 0;
  m_pathSelectD = 0;

  m_beginLoadMap = 0;
  m_logMessage = 0;
  m_setQuit = 0;
}


View::~View()
{
  releaseBuffer();

  // Cleanup
  ImGui_ImplGlfw_Shutdown();
  glfwTerminate();
}


static void drop_callback(GLFWwindow* window, int count, const char** paths)
{
	if (count == 1) 
		Common::beginLoadMap(paths[0]);
}


static void error_callback(int error, const char* description)
{
	Common::Log(description);
}


void View::Init(SettingsMap* a_controller, std::string* a_log, CallbackString a_loadFn, CallbackString a_logFn, CallbackVoid a_quitFn)
{    
  //
  settings = a_controller;
  m_commonLog = a_log;
  m_beginLoadMap = a_loadFn;
  m_logMessage = a_logFn;
  m_setQuit = a_quitFn;
  
  // Initialise GLFW
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
	  return;

  // Create window
  m_glwindow = glfwCreateWindow(1280, 720, "SC2MA", NULL, NULL);
  glfwMakeContextCurrent(m_glwindow);

  // Setup ImGui binding
  ImGui_ImplGlfw_Init(m_glwindow, true);

  // Drag-drop callback
  glfwSetDropCallback(m_glwindow, drop_callback);

  // ..?
  ImGuiIO& io = ImGui::GetIO();
  //io.ImeWindowHandle = window;

  unsigned char* pixels;
  int width, height, bytes_per_pixels;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixels);

  // After context creation, can initialise GLEW
  if (glewInit() != GLEW_OK)
  {
	  /* Problem: glewInit failed, something is seriously wrong. */
	  //fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }
  
  // For FBO target
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Initialise Font Manager 
  m_fm.Init(".");

}


void View::Done()
{
    releaseBuffer();
}


void View::releaseBuffer()
{
	if (m_frameBuffer)
	{
		glDeleteFramebuffers(1, &m_frameBuffer);
		m_frameBuffer = 0;
	}

	if (m_renderTexture)
	{
		glDeleteTextures(1, &m_renderTexture);
		m_renderTexture = 0;
	}

}


void View::createBuffer()
{
	releaseBuffer();

	glEnable(GL_TEXTURE_2D);

	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	glGenTextures(1, &m_renderTexture);

	glBindTexture(GL_TEXTURE_2D, m_renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_bufferWidth, m_bufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
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
	gluOrtho2D(0, m_bufferWidth, 0, m_bufferHeight);
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


std::map<string, Footprint*>* getInnerMap_edit(Config* config, int fp_type)
{
	if (config->type2name2foot.find(fp_type) == config->type2name2foot.end())
		return NULL;

	return config->type2name2foot[fp_type];
}


void View::DrawScreen()
{
	// Check the window
	if (glfwWindowShouldClose(m_glwindow))
	{
		Common::SetQuit();
		return;
	}

	// useful
	int windowWidth, windowHeight;
	glfwGetFramebufferSize(m_glwindow, &windowWidth, &windowHeight);


	glfwPollEvents();

	// Map BG...

	if (m_frameBuffer == 0) 
		createBuffer();

	if (m_frameBuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			float r, g, b;
			Colour::ToRGB(settings->colour_background, r, g, b);

			glViewport(0, 0, m_bufferWidth, m_bufferHeight);
			glClearColor(b, g, r, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			pushScreenCoordinateMatrix();
			
			m_rm.OnResize(m_bufferWidth, m_bufferHeight);
			DrawMap();

			popScreenCoordinateMatrix();
		}
	}
	
	// Calculate IM...
	ImGui_ImplGlfw_NewFrame();

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SetWindowPos(ImVec2(8, 8));

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::Button("Open Map..."))
	{
#ifdef _WINDOWS
		LoadMapFile(glfwGetWin32Window(m_glwindow));
#endif
	}
	
	if (m_sc2map)
	{
		if (ImGui::Button("Save Image..."))
		{
			SaveImageFile(glfwGetWin32Window(m_glwindow));
		}

		std::string basesString;
		for (list<StartLoc*>::iterator i = m_sc2map->startLocs.begin(); i != m_sc2map->startLocs.end(); ++i)
		{
			basesString = basesString + std::string((*i)->name);
			basesString.push_back('\0');
		}

		if (ImGui::CollapsingHeader("Map Layers", 0, true, true))
		{
			ImGui::Checkbox("Bases", &settings->layer_bases);
			ImGui::Checkbox("Buildable", &settings->layer_buildable);
			ImGui::Checkbox("Destructable", &settings->layer_destructable);
			ImGui::Checkbox("Gas balance", &settings->layer_gas_balance);
			ImGui::Checkbox("Height", &settings->layer_height);
			ImGui::Checkbox("Influence", &settings->layer_influence);
			if (settings->layer_influence)
			{
				ImGui::Combo("Base influence", &settings->spawn_influence, basesString.c_str());
			}
			ImGui::Checkbox("Line-of-sight blockers", &settings->layer_los_blockers);
			ImGui::Checkbox("Openness", &settings->layer_openness);
			ImGui::Checkbox("Pathable", &settings->layer_pathable);
			//ImGui::Checkbox("", settings->layer_pathing);
			ImGui::Checkbox("Photon overcharge", &settings->layer_photon_overcharge);
			//ImGui::Checkbox("", settings->layer_siegeable);
			ImGui::Checkbox("Watchtowers", &settings->layer_watchtowers);
		}

		// Pathing section
		if (ImGui::CollapsingHeader("Pathing") && m_sc2map)
		{
			ImGui::Checkbox("Show path", &settings->layer_pathing);

			char* text_base = " Main \0 Nat \0 3rd \0";

			// From:
			bool A = ImGui::Combo("From", &m_pathSelectA, basesString.c_str());
			bool B = ImGui::Combo("Base", &m_pathSelectB, text_base);
			// To:
			bool C = ImGui::Combo("To", &m_pathSelectC, basesString.c_str());
			bool D = ImGui::Combo(" ", &m_pathSelectD, text_base);

			if (A || B || C || D)
			{
				SetPath(m_pathSelectA, m_pathSelectB, m_pathSelectC, m_pathSelectD);
			}
		}

		if (ImGui::CollapsingHeader("Colours"))
		{

			ImGui::ColorEdit3("Clear colour", (float*)&clear_color);
			ImGui::ColorEdit3("Background colour", settings->colour_background);

			ImGui::Text("Openness");
			size_t n = settings->colours_openness.size();
			for (int i = 0; i < n; ++i)
			{
				unsigned long t = settings->colours_openness[i];

				char s[256];
				sprintf(s, "%d.", i);
				if (ImGui::ColorEdit3(s, t))
					settings->colours_openness[i] = t;
			}

			if (ImGui::Button("- Remove openness colour"))
			{
				if (n > 0)
					settings->colours_openness.pop_back();
			}

			if (ImGui::Button("+ Add openness colour"))
			{
				if (n == 0)
					settings->colours_openness.push_back(settings->colour_default);
				else
					settings->colours_openness.push_back(settings->colours_openness[n - 1]);
			}
		}

		if (ImGui::CollapsingHeader("Log", 0, true, true))
		{
			ImGui::TextWrapped(m_commonLog->c_str());
		}

		if (ImGui::CollapsingHeader("Footprints", 0, true, true))
		{
			map<string, Footprint*>* innerMap;
			for (int i = 0; i < FootprintTypesSize; ++i)
			{
				FootprintTypes fp_type = (FootprintTypes)(1 << i);
				innerMap = getInnerMap_edit(&(configUserGlobal), fp_type);
				if (innerMap)
				{
					ImGui::Text("Type 1 << %i", i);

					for (map<string, Footprint*>::iterator mapIter = innerMap->begin(); mapIter != innerMap->end(); ++mapIter)
					{
						ImGui::Text("-   %s", (*mapIter).first.c_str());
					}

				}
			}
		}
	}
	else
	{
		ImGui::Text("Drag and drop SC2MAP file here.");
	}

	ImGui::End();


	// Rendering everything to window
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, windowWidth, 0, windowHeight);
	glPopAttrib();


	if (m_frameBuffer)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_renderTexture);

		glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 1.f);

		int x = (windowWidth - m_bufferWidth - 270) / 2 + 270;
		int y = (windowHeight - m_bufferHeight) / 2;

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, 1.0f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + m_bufferWidth, y, 1.0f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + m_bufferWidth, y + m_bufferHeight, 1.0f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + m_bufferHeight, 1.0f);  // Top Left Of The Texture and Quad

		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	ImGui::Render();
		
	glfwSwapBuffers(m_glwindow);

	glFlush();
}


void View::DrawMap()
{
	if (m_sc2map == 0) return;

	// Title
	m_fm.DrawCenter(0.5 * m_bufferWidth, m_bufferHeight - 40, 1, m_sc2map->mapName.c_str());

	// Legend
	char sp[128];
	std::string a, b;
	int x1 = 24;
	int x2 = 144;
	int y1 = m_bufferHeight - 60;
	
	// Playable area
	a = std::string("Playable area");
	snprintf(sp, 128, "%d", m_sc2map->cxDimPlayable);
	b = std::string(sp);
	b += std::string(" x ");
	snprintf(sp, 128, "%d", m_sc2map->cyDimPlayable);
	b += std::string(sp);
	
	m_fm.Draw(x1, y1, 2, a.c_str());
	m_fm.Draw(x2, y1, 2, b.c_str());
	y1 += -20;
	
	// # Bases
	a = std::string("Bases identified");
	snprintf(sp, 128, "%d", (int)m_sc2map->bases.size());
	b = std::string(sp);
	
	m_fm.Draw(x1, y1, 2, a.c_str());
	m_fm.Draw(x2, y1, 2, b.c_str());
	y1 += -20;	
	
	// Total resources
	a = std::string("Total resources");
	snprintf(sp, 128, "%d", (int)m_sc2map->totalMinerals);
	b = std::string(sp) +
	std::string(" M   ");
	snprintf(sp, 128, "%d", (int)m_sc2map->totalVespeneGas);
	b += std::string(sp) +
	std::string(" G   ");
	
	m_fm.Draw(x1, y1, 2, a.c_str());
	m_fm.Draw(x2, y1, 2, b.c_str());
	y1 += -20;	
	
	// Footer
    a = std::string("SC Map Analyser for LotV, algorithms ");
    a += std::string(QUOTEMACRO(VALG));
    m_fm.DrawCenter(0.5 * m_bufferWidth, 20, 2, a.c_str());
	


	// Map grids:


		
	// Openness
	unsigned long colour = 0;
	Declaration2 drawRect;
	for (int i = 0; i < m_sc2map->cxDimPlayable; ++i)
	{
		for (int j = 0; j < m_sc2map->cyDimPlayable; ++j)
		{ 
			colour = settings->colour_default;

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
				m_rm.Draw(drawRect);
			}
			
			// Openness
            if (settings->layer_openness)
			{
				//float openness = sc2map.GetOpenness(i, j, PATH_GROUND_NOROCKS) / 14.0;
				float openness = m_sc2map->mapOpenness[NUM_PATH_TYPES*(j * m_sc2map->cxDimPlayable + i) + PATH_GROUND_NOROCKS];
				openness /= 14.f;
				if (openness > 1.f) openness = 1.f;
				
				if (openness > 0)
				{
                    DWORD blendedColour = 0;

                    //char h = m_sc2map->mapHeight[j * m_sc2map->txDimPlayable + i];
                    //switch (h)
                    //{
                    //    case 1:
                    //        color1 = m_sc2map->getColor("oplo1");
                    //        color2 = m_sc2map->getColor("oplo2");
                    //        color3 = m_sc2map->getColor("oplo3");
                    //        break;
                    //    case 2:
                    //        color1 = m_sc2map->getColor("opmid1");
                    //        color2 = m_sc2map->getColor("opmid2");
                    //        color3 = m_sc2map->getColor("opmid3");
                    //        break;
                    //    case 3:
                    //        color1 = m_sc2map->getColor("ophi1");
                    //        color2 = m_sc2map->getColor("ophi2");
                    //        color3 = m_sc2map->getColor("ophi3");
                    //        break;
                    //}

					int N = settings->colours_openness.size();
					if (N >= 2)
					{
						int n = (int)(openness * (N-1));

						if (n == N - 1)
						{
							blendedColour = settings->colours_openness[n];
						}
						else
						{
							DWORD color1 = settings->colours_openness[n];
							DWORD color2 = settings->colours_openness[n + 1];

							float a = (float)(n) / (float)(N - 1);

							float t = (openness - a) * (N - 1);
							
							blendedColour = Colour::RGBBlend(color1, color2, t);
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
						m_rm.Draw(drawRect);
					}
				}
			}
            
            // Influence
            if (settings->layer_influence)
            {
                if (IsPathable(i, j, PATH_GROUND_WITHROCKS_NORESOURCES))
                {
					int s = settings->spawn_influence;
                    
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
                    m_rm.Draw(drawRect);
                }
            }
		}
	}
	
	// Show LoS Blockers
    if (settings->layer_los_blockers)
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
			m_rm.Draw(drawRect);
		}
	}
	
	// Show Siegable 
    if (settings->layer_siegeable)
	{
	}
	
	// Height
    if (settings->layer_height)
	{
		char h = 0;
		for (int i = 0; i < m_sc2map->cxDimPlayable; ++i)
		{
			for (int j = 0; j < m_sc2map->cyDimPlayable; ++j)
			{
				h = m_sc2map->mapHeight[j * m_sc2map->txDimPlayable + i];
				
				if (h == 1)
				{
					colour = Colour::ColorToDword(m_sc2map->getColor("terrainElev1"), (int)(255));
				}
				else if (h == 2)
				{
					colour = Colour::ColorToDword(m_sc2map->getColor("terrainElev2"), (int)(255));
				}
				else if (h == 3)
				{
					colour = Colour::ColorToDword(m_sc2map->getColor("terrainElev3"), (int)(255));
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
					m_rm.Draw(drawRect);
					
					// Cross 2
					drawRect.Rect.left = hx;
					drawRect.Rect.right = hx + 1;
					drawRect.Rect.top = hy;
					drawRect.Rect.bottom = hy + 2;
					m_rm.Draw(drawRect);
					
					// Cross 3
					drawRect.Rect.left = hx + 3;
					drawRect.Rect.right = hx + 4;
					drawRect.Rect.top = hy;
					drawRect.Rect.bottom = hy + 1;
					m_rm.Draw(drawRect);
					
					// Cross 4
					drawRect.Rect.left = hx;
					drawRect.Rect.right = hx + 1;
					drawRect.Rect.top = hy + 3;
					drawRect.Rect.bottom = hy + 4;
					m_rm.Draw(drawRect);
				}
			}
		}    
	}	
	
	// Destruct
    if (settings->layer_destructable)
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
			m_rm.Draw(drawRect);
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
			
            m_rm.Draw(drawRect);
		}
	}
	
	// Pathing
    if (settings->layer_pathing)
	{
		DWORD c1 = Colour::ColorToDword(m_sc2map->getColor("shortestPathAir"));
		DWORD c2 = Colour::ColorToDword(m_sc2map->getColor("shortestPathGround"));
		DWORD c3 = Colour::ColorToDword(m_sc2map->getColor("shortestPathCWalk"));
		
        float d0 = 0.f;
		m_rm.DrawPath(m_pathA, -2, c1);
		m_rm.DrawPath(m_pathG, 0, c2);
		m_rm.DrawPath(m_pathC, 2, c3);

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
		
		//g_fm.Draw(m_bufferWidth - 235, 34, 1, title.c_str());
		/*
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
		g_rm.Draw(drawRect);*/
		
		snprintf(sp, 128, "%.1f", m_dA);
		title = std::string("Air: ") + std::string(sp);
		snprintf(sp, 128, "%.1f", m_dA / OVERLORD_SPEED);
		title += std::string(" (Overlord: ") + std::string(sp) + std::string("s)");
		//g_fm.Draw(m_bufferWidth - 215, 67, 2, title.c_str());
		
		snprintf(sp, 128, "%.1f", m_dG);
		title = std::string("Ground: ") + std::string(sp);
		snprintf(sp, 128, "%.1f", m_dG / PROBE_SPEED);
		title += std::string(" (Probe: ") + std::string(sp) + std::string("s)");
		//g_fm.Draw(m_bufferWidth - 215, 91, 2, title.c_str());
		
		snprintf(sp, 128, "%.1f", m_dC);
		title = std::string("Cliff: ") + std::string(sp);
		snprintf(sp, 128, "%.1f", m_dC / REAPER_SPEED);
		title += std::string(" (Reaper: ") + std::string(sp) + std::string("s)");
		//g_fm.Draw(m_bufferWidth - 215, 115, 2, title.c_str());
	}
	
	
	// Bases + Resources
    if (settings->layer_photon_overcharge)
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
				m_rm.Draw(drawRect);
				
				drawRect.Color0 = 0xffffffff;
				drawRect.Color1 = 0xffffffff;
				drawRect.Color2 = 0xffffffff;
				drawRect.Color3 = 0xffffffff;
				drawRect.Fill = false;
				m_rm.Draw(drawRect);
				
				drawRect.Fill = true;
				
				m_rm.DrawCircle(mapToImgX((*i)->loc.mtx - 1) - 2,
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
				m_rm.Draw(drawRect);

				m_rm.DrawCircle(
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
	if (settings->layer_gas_balance)
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
				m_rm.Draw(drawRect);
                                
            }
        }
    }
	
	// Watchtower
    if (settings->layer_watchtowers)
	{
		for (list<Watchtower*>::iterator i = m_sc2map->watchtowers.begin(); i != m_sc2map->watchtowers.end(); ++i)
		{
			m_rm.DrawCircle(mapToImgX((*i)->loc.mtx - 1), mapToImgY((*i)->loc.mty), 4.f * (*i)->range); // sight range
			//g_rm.DrawCircle(mapToImgX((*i)->loc.mtx - 1), mapToImgY((*i)->loc.mty), 4.f * 2.5f); // activation range
		}
	}
	
	// Start Locations
    if (settings->layer_bases)
	{
		for (list<StartLoc*>::iterator i = m_sc2map->startLocs.begin(); i != m_sc2map->startLocs.end(); ++i)
		{
			std::string title = "";
			
			if ((*i)->mainBase)
			{	
				int basex = mapToImgX((*i)->mainBase->loc.mtx);
				int basey = mapToImgY((*i)->mainBase->loc.mty);
				
				title = std::string("Main @ ") + std::string((*i)->name);
				m_fm.Draw(basex - 26, m_bufferHeight - basey - 12, 2, title.c_str());
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
				m_fm.Draw(basex - 26, m_bufferHeight - basey - 12, 2, title.c_str());
			}
			
			if ((*i)->thirdBase)
			{
				title = "Third";
				int basex = mapToImgX((*i)->thirdBase->loc.mtx);
				int basey = mapToImgY((*i)->thirdBase->loc.mty);
				m_fm.Draw(basex - 26, m_bufferHeight - basey - 12, 2, title.c_str());
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


void flipY(int w, int h, unsigned char* a_pixels)
{
	float* pixels = (float*)a_pixels;
	for (int i = 0; i < w; ++i)
	{
		for (int j = 0; j < h / 2; ++j)
		{
			float c = pixels[j * w + i];
			pixels[j * w + i] = pixels[(h - j - 1) * w + i];
			pixels[(h - j - 1) * w + i] = c;
		}
	}
}


void View::SaveImage(const char* a_fileName)
{
	int w = m_bufferWidth;
	int h = m_bufferHeight;
	int comp = 4;
	int stride = 4 * m_bufferWidth;

	unsigned char* pixelbuffer = new unsigned char[h * stride];
	GLvoid* pixels = (GLvoid*)pixelbuffer;

	if (m_frameBuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer); // Set FBO
		glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		flipY(w, h, pixelbuffer);

		stbi_write_png(a_fileName, w, h, comp, (void*)pixels, stride);
	}

	delete pixelbuffer;
}
