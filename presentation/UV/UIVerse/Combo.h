#pragma once


#include "Widget.h"
#include <string>


namespace UV
{
  class List;


  // Combo label+list element, aka drop-down box
  class Combo : public Widget
  {
  public:
    Combo();
    ~Combo();
    
    void AddItem(std::string a_item);

    void OnMouseMove(long a_x, long a_y);

    bool OnMousePressed(unsigned short a_x, unsigned short a_y);
    bool OnMouseReleased(unsigned short a_x, unsigned short a_y);

    void OnDeviceReset();
    void OnDeviceLost();

    void Draw();

    void SetPosition(int a_x, int a_y);

    void SetSize(int a_width, int a_height);

    void Select(int a_index);

    int GetSelectedIndex();

    void SetCallback(Callback a_callback);

    // Callback from list
    void OnItemSelected();

  protected:

    List* m_list;

    int m_left;
    int m_top;

    int m_width;
    int m_height;

    bool m_open;
    bool m_hover;

    Callback m_callback;
  };
}
