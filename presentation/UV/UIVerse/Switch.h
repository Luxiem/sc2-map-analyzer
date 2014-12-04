#pragma once


#include "Widget.h"
#include <string>


namespace UV
{
  class Switch : public Widget
  {
  public:
    Switch(Declaration a_decl);
    ~Switch();

    void OnMouseMove(long a_x, long a_y);

    bool OnMousePressed(unsigned short a_x, unsigned short a_y);
    bool OnMouseReleased(unsigned short a_x, unsigned short a_y);

    void Draw();

    void SetText(const char* text, unsigned long colour);

    void Show() { m_show = true; }

    void Hide() { m_show = false; }

    void Toggle() { m_toggle = !m_toggle; }
    bool GetToggle() { return m_toggle; }
    void SetToggle(bool a_toggle) { m_toggle = a_toggle; }

    void SetPosition(int a_x, int a_y);

    void GetRect(RECT& a_rect);

  protected:

    bool MouseOver(unsigned short a_x, unsigned short a_y);

    bool m_show;

    enum SwitchState
    {
      NORMAL,
      HOVER,
      PRESSED,
    };

    SwitchState m_state;
    Declaration m_decl;
    RECT m_rect;
    std::string m_text;
    unsigned long m_colour;
    bool m_toggle;
  };
}