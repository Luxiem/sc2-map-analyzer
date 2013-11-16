#include "List.h"
#include "DrawManager.h"
#include "Slider.h"
#include "Combo.h" // Callback hack


namespace UV
{


List::List()
{
  m_mouseOverIndex = -1;
  m_selectedIndex = -1;

  //m_selection = NULL;
  //m_hover = NULL;

  m_left = 0;
  m_top = 0;

  m_width = 200;
  m_height = 32;

  m_showScrollBar = true;
  m_scrollbar = new Slider(Slider::VERTICAL);

  m_callback = NULL;

  m_parent = NULL;
  
  m_itemHeight = 20;
}
  

List::~List()
{
  //if (m_selection) m_selection->Release();
  //if (m_hover) m_hover->Release();
  delete m_scrollbar;
}


void List::OnMouseMove(long a_x, long a_y)
{
  int totalHeight = m_itemHeight * (m_items.size() + 1);
  double scroll = m_scrollbar->GetValue();
  int offset = totalHeight > m_height ? -(int)((double)(totalHeight - m_height) * scroll) : 0;

  // check mouse over item
  m_mouseOverIndex = -1;
  int y = a_y - offset;

  if (a_x >= m_left && 
      a_x <= m_left + m_width &&
      y >= m_top)
  {
    size_t i = (size_t)((float)(y - m_top) / (float)m_itemHeight);
    if (i < m_items.size())
    {
      m_mouseOverIndex = (int)i;
      //a_handled = true;
    }
  }

  if (m_showScrollBar)
  {
    m_scrollbar->OnMouseMove(a_x, a_y);
  }
}


bool List::OnMousePressed(unsigned short a_x, unsigned short a_y)
{ 
  if (m_showScrollBar)
  {
    if (m_scrollbar->OnMousePressed(a_x, a_y))
      return true;
  }

  // select mouse over item
  int totalHeight = m_itemHeight * (m_items.size() + 1);
  double scroll = m_scrollbar->GetValue();
  int offset = totalHeight > m_height ? -(int)((double)(totalHeight - m_height) * scroll) : 0;
  int y = a_y - offset;
  if (a_x >= m_left && 
      a_x <= m_left + m_width &&
      y >= m_top)
  {
    size_t i = (size_t)((float)(y - m_top) / (float)m_itemHeight);
    if (i < m_items.size())
    {
      m_selectedIndex = (int)i;
      if (m_callback)
      {
        EventArgs args;
        m_callback->Execute(this, args);
      }
      if (m_parent) m_parent->OnItemSelected();
      return true;
    }
  }

  return false;
}


bool List::OnMouseReleased(unsigned short a_x, unsigned short a_y)
{
  if (m_showScrollBar)
  {
    return m_scrollbar->OnMouseReleased(a_x, a_y);
  }
  return false;
}


void List::Draw()
{
  int totalHeight = m_itemHeight * (m_items.size() + 1);
  double scroll = m_scrollbar->GetValue();
  int offset = totalHeight > m_height ? -(int)((double)(totalHeight - m_height) * scroll) : 0;
  RectManager* rm = DrawManager::GetRectManager();

  m_decl.Align = 1;
  m_decl.m_commandPressed = NULL;
  m_decl.m_commandReleased = NULL;
  m_decl.Rect.left = m_left;
  m_decl.Rect.right = m_left + m_width;
  m_decl.Rect.top = m_top;
  m_decl.Rect.bottom = m_top + m_height;
  m_decl.Resource = 0;
  m_decl.Style = 0;

  Declaration2 decl2 = Declaration2();
  decl2.Rect = m_decl.Rect;

  decl2.Color0 = 0xfffafafa;
  decl2.Color1 = 0xfffafafa;
  rm->Draw(decl2);

  decl2.Color0 = 0xff777777;
  decl2.Color1 = 0xff777777;
  rm->DrawOutline(decl2);

  if (m_mouseOverIndex >= 0)
  {
    //DrawManager::GetSpriteManager()->DrawBanner(
    decl2.Rect.top = m_top + m_mouseOverIndex * 20;
    decl2.Rect.bottom = decl2.Rect.top + 20;

    decl2.Color0 = 0xfffafa77;
    decl2.Color1 = 0xfffafa77;
    decl2.Color2 = 0xfffafa77;
    decl2.Color3 = 0xfffafa77;
    rm->Draw(decl2);
  }

  if (m_selectedIndex >= 0)
  {
    //DrawManager::GetSpriteManager()->DrawBanner(
    decl2.Rect.top = m_top + m_selectedIndex * 20;
    decl2.Rect.bottom = decl2.Rect.top + 20;

    decl2.Color0 = 0xffdadaff;
    decl2.Color1 = 0xffdadaff;
    decl2.Color2 = 0xffdadaff;
    decl2.Color3 = 0xffdadaff;
    rm->Draw(decl2);
  }

  for (unsigned int i = 0; i < m_items.size(); ++i)
  {
    RECT rect;
    rect.left = m_left + 10;
    rect.top = m_top + 15 + m_itemHeight * i + offset;
    rect.right = m_left + m_width - 16;
    rect.bottom = rect.top + m_itemHeight;
    
    if (rect.bottom < m_top) continue;
    if (rect.top > m_top + m_height) continue;


    if (rect.top < m_top) rect.top = m_top;
    if (rect.bottom > m_top + m_height) rect.bottom = m_top + m_height;

    DrawManager::GetFontManager()->Draw(rect.left, rect.top, 0, m_items[i].c_str());
  }

  if (m_showScrollBar)
  {
    m_scrollbar->Draw();
  }
}


void List::SetPosition(int a_x, int a_y)
{
  m_left = a_x;
  m_top = a_y;

  m_scrollbar->SetPosition(a_x + m_width - 16, a_y);
  m_scrollbar->SetSize(16, m_height);
}


void List::SetSize(int a_width, int a_height)
{
  if (a_height == -1)
  {
    a_height = m_itemHeight * m_items.size();
    m_showScrollBar = false;
  }

  m_width = a_width;
  m_height = a_height;

  m_scrollbar->SetPosition(m_left + m_width - 16, m_top);
  m_scrollbar->SetSize(16, a_height);
}


std::string List::GetSelectedItem()
{
  if (m_selectedIndex >= 0 && (unsigned int)m_selectedIndex < m_items.size())
  {
    return m_items[m_selectedIndex];
  }

  return "";
}


void List::Select(int a_index)
{
  m_selectedIndex = a_index;

  if (m_callback)
  {
    EventArgs args;
    m_callback->Execute(this, args);
  }

  if (m_parent) m_parent->OnItemSelected();
}


int List::GetSelectedIndex()
{
  return m_selectedIndex;
}


void List::SetParent(Combo* a_combo)
{
  m_parent = a_combo;
}


void List::SetCallback(Callback* a_callback)
{
  m_callback = a_callback;
}


unsigned int List::GetItemCount()
{
  return m_items.size();
}


void List::Clear()
{
  m_items.clear();
}


} // namespace
