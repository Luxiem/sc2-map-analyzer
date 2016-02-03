#pragma once


#include <map>
#include <string>


//struct TTF_Font;
typedef std::map<int, void*> FontList;
typedef FontList::const_iterator FontIter;


class FontManager
{
public:
  FontManager();
  ~FontManager();

  void Init(std::string a_path);

  bool IsInit();

  void Draw(float x, float y, int type, const char *txt);
  void DrawCenter(float x, float y, int type, const char *txt);

protected:

	void LoadFont(int a_type, const char *filename, int size);
	void* GetFont(int a_type);


	FontList m_fonts;

};

