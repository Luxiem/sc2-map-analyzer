#pragma once


#include "Widget.h"


#include <vector>


typedef struct IDirect3DDevice9 *LPDIRECT3DDEVICE9, *PDIRECT3DDEVICE9;


namespace UV
{
  class Container : public Widget
  {
  public:
    Container();
    virtual ~Container();

    virtual void Add(Widget* a_element);

    virtual void Draw();

    virtual void OnMouseMove(long a_x, long a_y);
    virtual bool OnMousePressed(unsigned short a_x, unsigned short a_y);
    virtual bool OnMouseReleased(unsigned short a_x, unsigned short a_y);

    virtual bool OnKeyPressed(unsigned short a_x, unsigned short a_y);
    virtual bool OnKeyReleased(unsigned short a_x, unsigned short a_y);
    virtual bool OnChar(int a_w, int a_l);
    
    virtual void GetRect(RECT& a_rect);

  protected:

    virtual void OnChildAdded(Widget* a_widget) {}
    virtual void OnChildRemoved(Widget* a_widget) {}

    // Elements contained in this page
    std::vector<Widget*> m_elements;
  };
}
