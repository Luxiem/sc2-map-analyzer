#ifndef __BUTTON_H__
#define __BUTTON_H__


#include "Widget.h"
#include <string>


namespace UV
{
  class ResourceManager;


  class Button : public Widget
  {
  public:
    Button(Declaration a_decl);
    ~Button();

    void OnMouseMove(long a_x, long a_y);

    bool OnMousePressed(unsigned short a_x, unsigned short a_y);
    bool OnMouseReleased(unsigned short a_x, unsigned short a_y);

    void OnDeviceReset();

    void Draw();

    void SetText(const char* text, unsigned long colour);

    void Show() { m_show = true; }

    void Hide() { m_show = false; }

    void SetSize(int a_width, int a_height);

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

    ResourceManager* m_rm;
    Declaration m_decl;
    RECT m_rect;
    std::string m_text;
    unsigned long m_colour;
  };
}


#endif//__BUTTON_H__