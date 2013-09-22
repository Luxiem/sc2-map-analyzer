#include "FontManager.h"
#include <string>
#include <Windows.h>
#include <GL/gl.h>

#include "FTGL/ftgl.h"


FontManager::FontManager()
{
  m_height = 0;
}


FontManager::~FontManager()
{
	/*
	 FontIter font;
	for(font = m_fonts.begin(); font != m_fonts.end(); font++)
	{
		delete (*font).second;
	}
	
	m_fonts.clear();
	 */
}


void FontManager::Init()
{
  if (!IsInit())
  {
  }
}


bool FontManager::IsInit()
{
  return false;
}


FTFont* FontManager::GetFont(const char *filename, int size)
{
	using namespace std;
	
	char buf[256];
	sprintf(buf, "%s%i", filename, size);
	string fontKey = string(buf);
	
	FontIter result = m_fonts.find(fontKey);
	if(result != m_fonts.end())
	{
		return result->second;
	}
	

	string path = "";
	string fullname = path + string(filename);
	FTFont* font = new FTTextureFont(fullname.c_str());	
  //FTFont* font = new FTPixmapFont(fullname.c_str());	

	if(!font->FaceSize(size))
	{
		delete font;
		return NULL;
	}
	
	m_fonts[fontKey] = font;
	
	return font;
}


void FontManager::Draw(float x, float y, int type, const char *txt)
{
	FTFont* font = 0;
	if (type == 0) font = GetFont("arial.ttf", 12);
	else if (type == 1) font = GetFont("arial.ttf", 16);
	else if (type == 2) font = GetFont("arial.ttf", 14);
	if (font)
	{
		FTPoint point(x, m_height - y);
		
		glColor3f(0.f, 0.f, 0.f);		
		font->Render(txt, -1, point);
		
		if (type > 0)
		{
			FTPoint point2(x - 1.f, m_height - y + 1.f);
			glColor3f(1.f, 1.f, 1.f);
			font->Render(txt, -1, point2);
		}				
	}
}

