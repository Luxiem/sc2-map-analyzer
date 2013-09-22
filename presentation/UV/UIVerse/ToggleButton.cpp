#include "ToggleButton.h"
#include "DrawManager.h"


namespace UV
{


ToggleButton::ToggleButton(ToggleGroup* a_group, Declaration a_decl)
{
  m_decl = a_decl;
  m_group = a_group;
  m_toggle = false;

  m_group->Add(this);
}


ToggleButton::~ToggleButton()
{

}


void ToggleButton::OnMouseMove(long a_x, long a_y)
{
  RECT m_rect;
  m_rect.left = m_decl.HMargin;
  m_rect.top = m_decl.VMargin;
  m_rect.right = m_rect.left + 48;
  m_rect.bottom = m_rect.top + 48;

  m_hover = (a_x >= m_rect.left && m_rect.right >= a_x &&
             a_y >= m_rect.top && m_rect.bottom >= a_y);
}


bool ToggleButton::OnMousePressed(unsigned short a_x, unsigned short a_y)
{
  if (m_hover)
  {
    m_group->SetToggled(this);
    OnMouseMove(a_x, a_y);
    return true;
  }

  return false;
}


bool ToggleButton::OnMouseReleased( unsigned short a_x, unsigned short a_y )
{
  return false;
}


void ToggleButton::OnDeviceReset()
{

}


void ToggleButton::Draw()
{
}


void ToggleButton::SetToggle(bool a_toggle)
{
  m_toggle = a_toggle;
}


void ToggleButton::GetRect(RECT& a_rect)
{
  a_rect = m_decl.Rect;
}


} // namespace
