#include "FontManager.h"
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "Common.h"

FontManager::FontManager()
{
    m_height = 0;
    m_renderer = 0;
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


void FontManager::Init(SDL_Renderer* a_renderer, std::string a_path)
{
    m_renderer = a_renderer;
    m_path = a_path;
    
  if (!IsInit())
  {
      TTF_Init();
      
	  //LoadFont(0, "Arial Narrow Bold.ttf", 12);
	  //LoadFont(1, "Arial Narrow Bold.ttf", 16);
	  //LoadFont(2, "Arial Narrow Bold.ttf", 14);
	  LoadFont(0, "FreeSansBold.ttf", 12);
	  LoadFont(1, "FreeSansBold.ttf", 16);
	  LoadFont(2, "FreeSansBold.ttf", 14);
  }
}


bool FontManager::IsInit()
{
  return TTF_WasInit();
}


TTF_Font* FontManager::LoadFont(int a_type, const char *filename, int size)
{
	using namespace std;
	
	FontIter result = m_fonts.find(a_type);
	if(result != m_fonts.end())
	{
		return result->second;
	}
	
	string path = m_path;
	
	string fullname = /*path + "/" + */string(filename);
    //string fullname = string("/Library/Fonts/") + string(filename);
    
    TTF_Font* font = TTF_OpenFont(fullname.c_str(), size);
	
	m_fonts[a_type] = font;
	
	return font;
}


TTF_Font* FontManager::GetFont(int a_type)
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
	TTF_Font* font = GetFont(type);

	if (font)
	{

        SDL_Color col = { 0x00, 0x00, 0x00, 0xff };
        
        {
        SDL_Surface* s = TTF_RenderText_Blended(font, txt, col);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, s);

        int iW, iH;
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);

        SDL_Rect dest = {(int)x, (int)y - iH, iW, iH};
        SDL_RenderCopy(m_renderer, texture, 0, &dest);
        
        SDL_FreeSurface(s);
        SDL_DestroyTexture(texture);
        }
        
        if (type > 0)
        {
            //col = (SDL_Color){ 0xff, 0xff, 0xff, 0xff };
			col.a = 0xff;
			col.r = 0xff;
			col.g = 0xff;
			col.b = 0xff;

            SDL_Surface* s = TTF_RenderText_Blended(font, txt, col);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, s);
            
            int iW, iH;
            SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
            
            SDL_Rect dest = {(int)x - 1, (int)y - iH - 1, iW, iH};
            SDL_RenderCopy(m_renderer, texture, 0, &dest);
            
            SDL_FreeSurface(s);
            SDL_DestroyTexture(texture);
        }
	}
}

