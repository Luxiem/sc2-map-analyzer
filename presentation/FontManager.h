#pragma once


#include "UV/UIVerse/UVFontManager.h"
#include <map>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"


//struct TTF_Font;
typedef std::map<int, TTF_Font*> FontList;
typedef FontList::const_iterator FontIter;


class FontManager : public UV::FontManager
{
public:
  FontManager();
  ~FontManager();

    void Init(SDL_Renderer* a_renderer, std::string a_path);

  bool IsInit();

  void Draw(float x, float y, int type, const char *txt);

protected:

	TTF_Font* LoadFont(int a_type, const char* a_filename, int a_size);
    TTF_Font* GetFont(int a_type);
	
    SDL_Renderer* m_renderer;
    
    std::string m_path;
    
	FontList m_fonts;

};

