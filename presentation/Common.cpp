#include "Common.h"
#include "SC2Map.hpp"
#include "View.h"
#include "Controller.h"
#include <stdio.h>
#include <math.h>
#include <colour.h>


#if defined(WIN32) || defined(_WIN32) 
#define PATH_SEPARATOR "\\" 
#else 
#define PATH_SEPARATOR "/" 
#endif 


//
// Declaration to be defined in main.cpp (platform specific)
//
void SaveImageFile();


static Common* s_instance = NULL;


Common::Common()
{
	s_sc2map = 0;
	s_controller = 0;
	s_view = 0;

	siegeMap = 0;
	siege_processBlock = 0;
	siege_processUncover = 0;
	s_genPathType = 0;
	siege_processBlock2 = 0;

	m_quit = false;

	s_instance = this;
}


Common::~Common()
{

}


void Common::ReloadConfig(std::string a_path)
{
  std::string toolPath = a_path;
  readConfigFiles( false, NULL, &configInternal );
  readConfigFiles( true, &toolPath, &configUserGlobal );
}


void Common::Init(std::string a_path)
{
  // Config reading
  std::string toolPath = a_path;
  initConfigReading();
  readConfigFiles( false, NULL, &configInternal );
  readConfigFiles( true, &toolPath, &configUserGlobal );

  // Objects
  s_controller = NewSettingsMap();
  s_view = new View();
  s_view->Init(s_controller, &common_log, &beginLoadMap, &Log, &SetQuit);
}


void Common::beginLoadMap_impl(std::string a_fileName)
{
	string ext1( ".s2ma" );
	string ext2( ".SC2Map" );
	
	string::size_type loc1 = a_fileName.rfind( ext1 );
	string::size_type loc2 = a_fileName.rfind( ext2 );
	
	// if either search for an extension doesn't come
	// up with the location where the extension should be...
	if( loc1 != a_fileName.size() - ext1.size() &&
	   loc2 != a_fileName.size() - ext2.size() )
	{
		// not a Starcraft 2 Map Archive...
		return;
	}
	
	// find last dot, last back slash
	string::size_type locDot    = a_fileName.rfind( "." );
	string::size_type locBSlash = a_fileName.rfind( PATH_SEPARATOR );
	
	string path( a_fileName, 0,             locBSlash + 1              );
	string file( a_fileName, locBSlash + 1, locDot - locBSlash - 1     );
	string ext ( a_fileName, locDot,        a_fileName.size() - locDot );
	
	string localOutputPath( path );
	
	string fileWithExt( file );
	fileWithExt.append( ext );
	
	// Init map object
	if (s_sc2map) delete s_sc2map;
    s_sc2map = new SC2Map(
                          "",
                          localOutputPath,
                          path,
                          file,
                          fileWithExt );
	
	// Begin load
	s_sc2map->readMap();
	
	s_sc2map->countPathableCells();
	s_sc2map->prepShortestPaths();
	s_sc2map->identifyBases();
	s_sc2map->computeOpenness();
	s_sc2map->locateChokes();
	s_sc2map->analyzeBases();
	  
    // Apply loaded map to view
    s_view->SetMap(s_sc2map);

    // Colours from SC2Map files to settings
	s_controller->colour_default = Colour::ColorToDword(s_sc2map->getColor("default"));
	
	DWORD oplo1 = Colour::ColorToDword(s_sc2map->getColor("oplo1"));
	DWORD oplo2 = Colour::ColorToDword(s_sc2map->getColor("oplo2"));
	DWORD oplo3 = Colour::ColorToDword(s_sc2map->getColor("oplo3"));

	s_controller->colours_openness = { oplo1, oplo2, oplo3 };
}


// calls Plot(x, y) in a line between endpoints
void Common::DDA(
				int x1,
				int y1,
				int x2,
				int y2,
				PlotFunc Plot)
{
	
	if (x1 == x2)
	{
		//
		for (int j = y1; j != y2; y1 > y2 ? --j : ++j)
		{
			Plot(x1, j);
		}
		return;
	}
	
	if (y1 == y2)
	{
		//
		for (int i = x1; i != x2; x1 > x2 ? --i : ++i)
		{
			Plot(i, y1);
		}
		return;
	}
	
	float m = (float)(y1 - y2) / (float)(x1 - x2);
	float c = (float)y1 - m * (float)x1;
	
	if (m < 1.f)
	{
		// scan x
		
		for (int i = x1; i != x2; x1 > x2 ? --i : ++i)
		{
			int y = m * i + c;
			Plot(i, y);
		}
	}
	else
	{
		// scan y
		for (int j = y1; j != y2; y1 > y2 ? --j : ++j)
		{
			int x = (1.f / m) * (j - c);
			Plot(x, j);
		}
	}
}


void Common::PlotSiege(int a_x, int a_y)
{
	bool path = IsPathable(a_x, a_y, PATH_GROUND_WITHROCKS_NORESOURCES);
	
	if (!path)
	{
		++siege_processBlock;
	}
	else if (siege_processBlock >= 1)
	{
		siege_processUncover = true;
	}
}


void Common::PlotSiege2(int a_x, int a_y)
{
	bool path = IsPathable(a_x, a_y, PATH_GROUND_WITHROCKS_NORESOURCES);
	
	if (!path)
	{
		++siege_processBlock2;
	}
}


float Common::isSiegeable(int a_x, int a_y, int a_innerRadius, int a_radius)
{
	if (!IsPathable(a_x, a_y, PATH_GROUND_NOROCKS)) return false;
	
	int n = 0;
	int m = 0;
	
	for (int i = -a_radius; i <= a_radius; ++i)
	{
		for (int j = -a_radius; j <= a_radius; ++j)
		{
			if (abs(i) <= 2) continue;
			if (abs(j) <= 2) continue;
			
			int x = a_x + i;
			int y = a_y + j;
			
			if (x <= 0) continue;
			if (x >= s_sc2map->cxDimPlayable) continue;
			if (y <= 0) continue;
			if (y >= s_sc2map->cyDimPlayable) continue;
			
			if (!IsPathable(x, y, PATH_GROUND_NOROCKS)) continue;
			
			float dist = sqrt((float)(i * i + j * j));
			
			if (dist < (float)a_innerRadius) continue;
			if (floor(dist) > a_radius) continue;
			
			// reset variables
			siege_processBlock = 0;
			siege_processUncover = false;
			
			// Draw line between points
			//DDA(a_x, a_y, x, y, &PlotSiege);
			
			// Check results
			if (siege_processUncover)
			{
				// confirm that 8 surrounding grid points are blocked by pathing
				bool clear = false;
				for (int ii = -1; ii <= 1; ++ii)
				{
					for (int jj = -1; jj <= 1; ++jj)
					{
						if (ii == 0 && jj == 0) continue;
						if (a_x + ii < 0 || a_x + ii >= s_sc2map->cxDimPlayable) continue;
						if (a_y + jj < 0 || a_y + jj >= s_sc2map->cyDimPlayable) continue;
						if (x + ii < 0 || x + ii >= s_sc2map->cxDimPlayable) continue;
						if (y + jj < 0 || y + jj >= s_sc2map->cyDimPlayable) continue;
						siege_processBlock2 = 0;
			//			DDA(a_x + ii, a_y + jj, x + ii, y + jj, &PlotSiege2);
						if (siege_processBlock2 == 0) clear = true;
					}
				}
				
				if (clear)
					n = n + 0;
				else
					n = n + 2;
			}
			++m;
		}
	}	
	
	if (n == 0) return 0.f;
	
	return (float)(n);
}


void Common::GenerateSiegeMap(int a_type)
{
	s_genPathType = a_type;
	
	// create bool map fo entire area
	if (siegeMap) delete[] siegeMap;
	
	siegeMap = new float[s_sc2map->cxDimPlayable * s_sc2map->cyDimPlayable];
	
	for (int i = 0; i < s_sc2map->cxDimPlayable; ++i)
	{
		for (int j = 0; j < s_sc2map->cyDimPlayable; ++j)
		{
			if (a_type == 1) siegeMap[i + j * s_sc2map->cxDimPlayable] = isSiegeable(i, j, 0, 8); // blink
			if (a_type == 2) siegeMap[i + j * s_sc2map->cxDimPlayable] = isSiegeable(i, j, 10, 13); // tank
		}
	}
}


void Common::SaveImage_impl(std::string a_fileName)
{
	if (a_fileName.size() <= 4 || a_fileName.substr(a_fileName.size() - 4, 4) != std::string(".png"))
	{
		a_fileName += ".png";
	}
	
    s_view->SaveImage(a_fileName.c_str());
}


bool Common::Quit() { return m_quit; }


void Common::DrawScreen()
{
	s_view->DrawScreen();
}


void Common::SetPath(int a_pathSpawnA, int a_pathBaseA, int a_pathSpawnB, int a_pathBaseB)
{
  s_view->SetPath(a_pathSpawnA, a_pathBaseA, a_pathSpawnB, a_pathBaseB);
}


bool Common::IsPathable(int i, int j, int t)
{
  if (s_sc2map == 0 || s_sc2map->mapPathing == 0) return false;
  if (t >= NUM_PATH_TYPES) return false;
  if (i < 0) return false;
  if (i >= s_sc2map->cxDimPlayable) return false;
  if (j < 0) return false;
  if (j >= s_sc2map->cyDimPlayable) return false;
  return s_sc2map->mapPathing[NUM_PATH_TYPES * (j * s_sc2map->cxDimPlayable + i) + t];
}


void Common::Log_impl(const char* a_msg)
{
	common_log.append(a_msg);
	common_log.append("\n");
}


void Common::Log(const char* a_msg) { s_instance->Log_impl(a_msg); }
void Common::SetQuit() { s_instance->SetQuit_impl(); }
void Common::beginLoadMap(const char* a_fileName) { s_instance->beginLoadMap_impl(a_fileName); }
void Common::SaveImage(const char* a_fileName) { s_instance->SaveImage_impl(a_fileName); }
