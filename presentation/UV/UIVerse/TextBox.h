#pragma once


#include "Widget.h"
#include <string>


namespace UV
{
  class List;
  class SpriteManager;


  // Combo label+list element, aka drop-down box
  class TextBox : public Widget
  {
  public:
    TextBox();
    ~TextBox();
    
    void OnMouseMove(long a_x, long a_y);

    bool OnMousePressed(unsigned short a_x, unsigned short a_y);
    bool OnMouseReleased(unsigned short a_x, unsigned short a_y);

    bool OnKeyPressed(int a_w, int a_l);
    bool OnChar(int a_w, int a_l);

    void OnDeviceReset();
    void OnDeviceLost();

    void Draw();

    void SetPosition(int a_x, int a_y);

    void SetSize(int a_width, int a_height);

    void SetText(const char* a_text);

    const char* GetText();

    void SetCallback(Callback* a_callback);

  protected:

    void ScrollToChar(int a_ch);

    List* m_list;

    int m_width;
    int m_height;
    RECT m_rect;

    int m_cursorStart;
    int m_cursorEnd;

    bool m_hover;
    bool m_selected;

    bool m_dragText;
    int m_dragBegin;

    int m_pixelOffset;

    std::string m_text;

    Callback* m_callback;

  };
}
