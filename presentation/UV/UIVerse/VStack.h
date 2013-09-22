#pragma once


#include "Stack.h"


namespace UV
{
  class VStack : public Stack
  {
  public:
    VStack();
    ~VStack();

    void SetPosition(int a_x, int a_y);

    void GetRect(RECT& a_rect);

  protected:

    void OnChildAdded(Widget* a_widget);
    void OnChildRemoved(Widget* a_widget);

    int m_x;
    int m_y;
  };
}
