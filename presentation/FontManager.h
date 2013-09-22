#pragma once


#include "UV/UIVerse/UVFontManager.h"
#include <map>
#include <string>


class FTFont;
typedef std::map<std::string, FTFont*> FontList;
typedef FontList::const_iterator FontIter;


class FontManager : public UV::FontManager
{
public:
  FontManager();
  ~FontManager();

  void Init();

  bool IsInit();

  void Draw(float x, float y, int type, const char *txt);

protected:

	FTFont* GetFont(const char *filename, int size);
	
	FontList m_fonts;

};

