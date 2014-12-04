#pragma once


#include "Widget.h"
#include "ToggleGroup.h"


namespace UV
{
  class ToggleButton : public Widget
  {
  public:
    ToggleButton(ToggleGroup* a_group, Declaration a_decl);
    ~ToggleButton();

    void OnMouseMove(long a_x, long a_y);

    bool OnMousePressed(unsigned short a_x, unsigned short a_y);
    bool OnMouseReleased(unsigned short a_x, unsigned short a_y);

    void OnDeviceReset();

    void Draw();
    
    // Should only be called by ToggleGroup
    void SetToggle(bool a_toggle);

    void GetRect(RECT& a_rect);

  protected:

    bool MouseOver(unsigned short a_x, unsigned short a_y);

    Declaration m_decl;

    ToggleGroup* m_group;

    bool m_toggle;

    bool m_hover;
  };
}
