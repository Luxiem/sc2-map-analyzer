#pragma once


#include <vector>
#include "UVFontManager.h"
#include "UVRectManager.h"


namespace UV
{
  class Page;
  class Group;
  class Widget;

  class Core
  {
  public:
    Core();
    ~Core();
    
	  void OnClientAreaChanged(int a_x, int a_y);

    void OnPaint();

    void SetFontManager(FontManager* a_fm);
    void SetRectManager(RectManager* a_rm);
    
    void Show() {}
    void Hide() {}

    Page* CreatePage();

	  bool OnLeftMouseDown(int a_x, int a_y);
	  void OnLeftMouseUp(int a_x, int a_y);
	  void OnMouseMove(int a_x, int a_y);
	  void OnScrollWheel(int a_delta);

  protected:

    Page* m_page;

    FontManager* m_fm;
    RectManager* m_rm;
  };
}
