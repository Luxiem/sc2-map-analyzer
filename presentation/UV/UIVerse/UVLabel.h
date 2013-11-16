#pragma once
#include "Widget.h"
#include <string>

namespace UV
{


class Label : public Widget
{

public:

    Label();
    ~Label();

    void Draw();

    void SetText(const char* text, unsigned long colour);

    void SetPosition(int a_x, int a_y);

    void SetSize(int a_width, int a_height);

    void GetRect(RECT& a_rect)
    {
      a_rect.top = m_rect.top;
      a_rect.bottom = m_rect.bottom;
      a_rect.left = m_rect.left;
      a_rect.right = m_rect.right;
    }

    void OnMouseMove(long a_x, long a_y) {}
    bool OnMousePressed(unsigned short a_x, unsigned short a_y) { return false; }
    bool OnMouseReleased(unsigned short a_x, unsigned short a_y) { return false; }

protected:

    RECT m_rect;
    std::string m_text;
    unsigned long m_colour;
};


}//namespace

