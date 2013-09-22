#pragma once


#include "Container.h"


namespace UV
{
  class Stack : public Container
  {
  public:
    Stack();
    ~Stack();

    void Add(Widget* a_widget);

    // - Inherit from Widget -
    void OnMouseMove(long a_x, long a_y);

    bool OnMousePressed(unsigned short a_x, unsigned short a_y);
    bool OnMouseReleased(unsigned short a_x, unsigned short a_y);

    void Draw();

    virtual void GetRect(RECT& a_rect) { Container::GetRect(a_rect); }

  protected:

    virtual void OnChildAdded(Widget* a_widget);
    virtual void OnChildRemoved(Widget* a_widget);
  };
}
