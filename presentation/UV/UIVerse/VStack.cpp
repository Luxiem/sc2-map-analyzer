#include "VStack.h"


namespace UV
{


VStack::VStack()
{
  m_x = 0;
  m_y = 0;

  m_alignY = ALIGN_NEAR;
}


VStack::~VStack()
{
}


void VStack::OnChildAdded(Widget* a_widget)
{
  // Calculate current rect
  int h = 0;
  GetHeight(h);

  // Place 
  a_widget->SetPosition(
      m_x,
      m_y + h + 12);

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
  a_rect.left += m_x;
  a_rect.right += m_x;
  a_rect.top += m_y;
  a_rect.bottom += m_y;
}


void VStack::GetHeight(int& a_height)
{
  UV::RECT rect;
  rect.left = m_x;
  rect.right = m_x;
  rect.top = m_y;
  rect.bottom = m_y;

  for (size_t i = 0; i < m_elements.size(); ++i)
  {
    m_elements[i]->GetRect(rect);
    a_height += (rect.bottom - rect.top);
    a_height += 12;// padding
  }
}


void VStack::SetAlignmentY(Align a_alignY)
{
	m_alignY = a_alignY;
}


} // namespace
