#include "VStack.h"


namespace UV
{


VStack::VStack()
{
  m_x = 0;
  m_y = 0;
}


VStack::~VStack()
{
}


void VStack::SetPosition(int a_x, int a_y)
{
  m_x = a_x;
  m_y = a_y;
}


void VStack::OnChildAdded(Widget* a_widget)
{
  // Calculate current rect
  RECT rect1;
  GetRect(rect1);

  // Calc rect to add
  RECT rect2;
  a_widget->GetRect(rect2);

  // Place 
  a_widget->SetPosition(
      m_x,
      rect1.bottom + 12);

  // Recalculate final rect
  Stack::OnChildAdded(a_widget);
}


void VStack::OnChildRemoved(Widget* a_widget)
{
  Stack::OnChildRemoved(a_widget); 
}


void VStack::GetRect(RECT& a_rect)
{
  a_rect.left = m_x;
  a_rect.right = m_x;
  a_rect.top = m_y;
  a_rect.bottom = m_y;
  Stack::GetRect(a_rect);
}


} // namespace
