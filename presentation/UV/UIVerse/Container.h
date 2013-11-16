#pragma once


#include "Widget.h"
#include <vector>


namespace UV
{
  class Container : public Widget
  {
  public:
    Container();
    virtual ~Container();

    virtual void Add(Widget* a_element);

    virtual void Clear() {}

    virtual void Draw();

    virtual bool OnLeftMouseDown(unsigned short a_x, unsigned short a_y) { return OnMousePressed(a_x, a_y); }
    virtual bool OnLeftMouseUp(unsigned short a_x, unsigned short a_y) { return OnMouseReleased(a_x, a_y); }
    virtual bool OnScrollWheel(unsigned short a_delta) { return false; }

    virtual void OnMouseMove(long a_x, long a_y);
    virtual bool OnMousePressed(unsigned short a_x, unsigned short a_y);
    virtual bool OnMouseReleased(unsigned short a_x, unsigned short a_y);

    virtual bool OnKeyPressed(unsigned short a_x, unsigned short a_y);
    virtual bool OnKeyReleased(unsigned short a_x, unsigned short a_y);
    virtual bool OnChar(int a_w, int a_l);
    
    virtual void GetRect(RECT& a_rect);

    virtual void SetBorder(int a_borderWidth, unsigned long a_borderColour);

  protected:

    virtual void OnChildAdded(Widget* a_widget) {}
    virtual void OnChildRemoved(Widget* a_widget) {}

    // Elements contained in this page
    std::vector<Widget*> m_elements;

    int m_borderWidth;
    unsigned long m_borderColour;
  };
}
