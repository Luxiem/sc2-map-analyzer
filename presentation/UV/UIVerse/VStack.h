#pragma once


#include "Stack.h"


namespace UV
{
  class VStack : public Stack
  {
  public:
    VStack();
    ~VStack();

    void GetRect(RECT& a_rect);

    void Space(int y) { m_y += y; }

	void SetAlignmentY(Align a_alignY);

  protected:

    void GetHeight(int& a_height);

    void OnChildAdded(Widget* a_widget);
    void OnChildRemoved(Widget* a_widget);

	Align m_alignY;
  };
}
