#pragma once


#include "Widget.h"
#include <vector>
#include <string>


namespace UV
{
  class Page;


  class Tab : public Widget
  {
  public:
    Tab();
    ~Tab();
    
    void OnMouseMove(long a_x, long a_y);

    bool OnMousePressed(unsigned short a_x, unsigned short a_y);
    bool OnMouseReleased(unsigned short a_x, unsigned short a_y);

    void Draw();

    void SetPosition(int a_x, int a_y);

    void SetSize(int a_width, int a_height);

    void AddTab(const std::string& a_name, Page* a_page);

  protected:

    Declaration m_decl;

    int m_active;
    int m_hover;

    std::vector<std::string> m_tabNames;
    std::vector<Page*> m_pages;
  };
}
