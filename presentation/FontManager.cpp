#include "FontManager.h"
#include <ft2build.h>
#include <windows.h>		//(the GL headers need it)
#include <GL/gl.h>
#include <GL/glu.h>
#include "FTGL/ftgl.h"




FontManager::FontManager()
{
}


FontManager::~FontManager()
{
}


void FontManager::Init(std::string a_path)
{
   if (!IsInit())
  {   	
	  

	  //LoadFont(0, "Arial Narrow Bold.ttf", 12);
	  //LoadFont(1, "Arial Narrow Bold.ttf", 16);
	  //LoadFont(2, "Arial Narrow Bold.ttf", 14);
	  //LoadFont(0, "FreeSansBold.ttf", 24);
	  //LoadFont(1, "FreeSansBold.ttf", 16);
	  //LoadFont(2, "FreeSansBold.ttf", 14);
	  LoadFont(0, "arial.ttf", 12);
	  LoadFont(1, "arial.ttf", 24);
	  LoadFont(2, "arial.ttf", 14);
  }
}


bool FontManager::IsInit()
{
	return m_fonts.size() > 0;
}


void FontManager::LoadFont(int a_type, const char *a_filename, int a_size)
{
	using namespace std;
	
	FontIter result = m_fonts.find(a_type);
	if(result != m_fonts.end())
	{
		return;
	}

	FTFont* font = new FTPixmapFont(a_filename);
	font->Depth(3.);
	font->Outset(-.5, 1.5);
	font->CharMap(ft_encoding_unicode);
	font->FaceSize(a_size);
	
	m_fonts[a_type] = font;
}


void* FontManager::GetFont(int a_type)
{
	FontIter result = m_fonts.find(a_type);
	if(result != m_fonts.end())
	{
		return result->second;
	}
    
    return NULL;
}


void FontManager::Draw(float x, float y, int type, const char *txt)
{
	FTFont* font = (FTFont*) m_fonts[type];

	glColor4f(1.0, 1.0, 1.0, 1.0);
	
	glRasterPos2i(x, y);
	font->Render(txt);
}


void FontManager::DrawCenter(float x, float y, int type, const char *txt)
{
	FTFont* font = (FTFont*)m_fonts[type];
	FTBBox box = font->BBox(txt);
	x = x - 0.5 * (box.Upper().X()- box.Lower().X());

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glRasterPos2i(x, y);
	font->Render(txt);
}
