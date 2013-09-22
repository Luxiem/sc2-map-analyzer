#include "Combo.h"
#include "DrawManager.h"
#include "List.h"


namespace UV
{


Combo::Combo()
{
  m_list = new List();
  m_left = 0;
  m_top = 0;
  m_width = 200;
  m_height = 32;
  m_open = false;
  m_hover = false;

  m_list->SetSize(200, 450);

  // TODO Set list callback.. to notify self of events..
  m_list->SetParent(this);

  m_callback = NULL;
}


Combo::~Combo()
{
  delete m_list;
}


void Combo::AddItem(std::string a_item)
{
  m_list->AddItem(a_item);
}


void Combo::OnMouseMove(long a_x, long a_y)
{
  // Rect
  long left = m_left;
  long right = m_left + m_width;
  long top = m_top;
  long bottom = m_top + m_height;
  m_hover = (a_x >= left && right >= a_x &&
             a_y >= top && bottom >= a_y);

  // List
  if (m_open)
    m_list->OnMouseMove(a_x, a_y);
}


bool Combo::OnMousePressed(unsigned short a_x, unsigned short a_y)
{
  if (m_open)
  {
    if (m_list->OnMousePressed(a_x, a_y)) 
    {   
      return true;
    }
  }

  if (m_hover)
  {
    m_open = !m_open;
    return true;
  }
  else
  {
    m_open = false;
  }

  return false;
}


bool Combo::OnMouseReleased(unsigned short a_x, unsigned short a_y)
{
  return m_list->OnMouseReleased(a_x, a_y);
}


void Combo::OnDeviceReset()
{
  m_list->OnDeviceReset();
}


void Combo::OnDeviceLost()
{
  m_list->OnDeviceLost();
}


void Combo::Draw()
{
  // Rect
  UV::Declaration2 decl2 = Declaration2();
  decl2.Rect.left = m_left;
  decl2.Rect.right = m_left + m_width;
  decl2.Rect.top = m_top;
  decl2.Rect.bottom = m_top + m_height;

  decl2.Color0 = 0xfffafafa;
  decl2.Color1 = 0xffd2d2d2;
  DrawManager::GetRectManager()->Draw(decl2);

  decl2.Color0 = m_hover ? 0xff7777ff : 0xff777777;
  decl2.Color1 = m_hover ? 0xff7777ff : 0xff777777;
  DrawManager::GetRectManager()->DrawOutline(decl2);
  
  decl2.Rect.left = m_left + m_width - 16;
  decl2.Rect.right = m_left + m_width;

  decl2.Color0 = 0xfffafafa;
  decl2.Color1 = 0xffd2d2d2;
  DrawManager::GetRectManager()->Draw(decl2);

  decl2.Color0 = m_hover ? 0xff7777ff : 0xff777777;
  decl2.Color1 = m_hover ? 0xff7777ff : 0xff777777;
  DrawManager::GetRectManager()->DrawOutline(decl2);
  
  // Text
  std::string item = m_list->GetSelectedItem();
//  if (item.length())
    //DrawManager::GetFontManager()->Draw(item.c_str(), m_left + 10, m_top - 4);

  // List
  if (m_open)
    m_list->Draw();
}


void Combo::SetPosition(int a_x, int a_y)
{
  m_left = a_x;
  m_top = a_y;

  m_list->SetPosition(a_x, a_y + m_height);
}


void Combo::SetSize(int a_width, int a_height)
{
  m_width = a_width;
  m_height = a_height;

  m_list->SetPosition(m_left, m_top + m_height);
  m_list->SetSize(m_width, -1);
}


void Combo::Select(int a_index)
{
  m_list->Select(a_index);
  
  if (m_callback) m_callback(this);
}


int Combo::GetSelectedIndex()
{
  return m_list->GetSelectedIndex();
}


void Combo::SetCallback( Callback a_callback )
{
  m_callback = a_callback;
}


void Combo::OnItemSelected()
{
  m_open = false;
  if (m_callback) m_callback(this);
}


} // namespace
