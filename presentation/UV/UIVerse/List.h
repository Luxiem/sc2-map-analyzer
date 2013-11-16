#pragma once


#include "Widget.h"
#include <vector>
#include <string>


namespace UV
{
  class Slider;
  class Combo; // TODO remove


  class List : public Widget
  {
  public:
    List();
    ~List();

    void AddItem(std::string a_item) { m_items.push_back(a_item); }

    void OnMouseMove(long a_x, long a_y);

    bool OnMousePressed(unsigned short a_x, unsigned short a_y);
    bool OnMouseReleased(unsigned short a_x, unsigned short a_y);

    void Draw();

    void SetPosition(int a_x, int a_y);

    void SetSize(int a_width, int a_height);

    std::string GetSelectedItem();

    int GetSelectedIndex();

    void Select(int a_index);

    void SetCallback(Callback* a_callback);

    void SetParent(Combo* a_combo);

    unsigned int GetItemCount();

    void Clear();

  protected:

    std::vector<std::string> m_items;

    Slider* m_scrollbar;
    bool m_showScrollBar;

    Declaration m_decl;

    int m_mouseOverIndex;
    int m_selectedIndex;

    int m_left;
    int m_top;
    int m_width;
    int m_height;

    int m_itemHeight;

    Callback* m_callback;

    Combo* m_parent;
  };
} // namespace