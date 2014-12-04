#ifndef __BUTTON_H__
#define __BUTTON_H__


#include "Widget.h"
#include <string>


namespace UV
{

  class Button : public Widget
  {
  public:
    Button(Declaration a_decl);
    ~Button();

    void OnMouseMove(long a_x, long a_y);

    bool OnMousePressed(unsigned short a_x, unsigned short a_y);
    bool OnMouseReleased(unsigned short a_x, unsigned short a_y);

    virtual void Draw();

    void SetText(const char* text, unsigned long colour);

    void Show() { m_show = true; }

    void Hide() { m_show = false; }

    void SetPosition(int a_x, int a_y);

    void SetSize(int a_width, int a_height);

    void GetRect(RECT& a_rect)
    {
      a_rect.top = m_rect.top;
      a_rect.bottom = m_rect.bottom;
      a_rect.left = m_rect.left;
      a_rect.right = m_rect.right;
    }

    void SetStyle(unsigned long a_style) { m_style = a_style; }

  protected:

    bool MouseOver(unsigned short a_x, unsigned short a_y);

    bool m_show;

    enum ButtonState
    {
      NORMAL,
      HOVER,
      PRESSED,
      TOGGLED_NORMAL,
      TOGGLED_HOVER,
      TOGGLED_PRESSED
    };

    ButtonState m_state;

    Declaration m_decl;
    RECT m_rect;
    std::string m_text;
    unsigned long m_colour;
    unsigned long m_style;
  };
}


#endif//__BUTTON_H__