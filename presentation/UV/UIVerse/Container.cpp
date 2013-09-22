#include "Container.h"


namespace UV
{


Container::Container()
{
}


Container::~Container()
{
}


void Container::Add(Widget* a_element)
{
  m_elements.push_back(a_element);
}


void Container::Draw()
{
  for (size_t i = 0; i < m_elements.size(); ++i)
  {
    m_elements[i]->Draw();
  }
}


void Container::OnMouseMove(long a_x, long a_y)
{
  for (size_t i = 0; i < m_elements.size(); ++i)
  {
    m_elements[i]->OnMouseMove(a_x, a_y);
  }
}


bool Container::OnMousePressed(unsigned short a_x, unsigned short a_y)
{
  for (size_t i = 0; i < m_elements.size(); ++i)
  {
    if (m_elements[i]->OnMousePressed(a_x, a_y)) return true;
  }

  return false;
}


bool Container::OnMouseReleased(unsigned short a_x, unsigned short a_y)
{
  for (size_t i = 0; i < m_elements.size(); ++i)
  {
    if (m_elements[i]->OnMouseReleased(a_x, a_y)) return true;
  }

  return false;
}


bool Container::OnKeyPressed(unsigned short a_w, unsigned short a_l)
{
  for (size_t i = 0; i < m_elements.size(); ++i)
  {
    if (m_elements[i]->OnKeyPressed(a_w, a_l)) return true;
  }

  return false;
}


bool Container::OnKeyReleased(unsigned short a_w, unsigned short a_l)
{
  for (size_t i = 0; i < m_elements.size(); ++i)
  {
    if (m_elements[i]->OnKeyReleased(a_w, a_l)) return true;
  }

  return false;
}


bool Container::OnChar(int a_w, int a_l)
{
  for (size_t i = 0; i < m_elements.size(); ++i)
  {
    if (m_elements[i]->OnChar(a_w, a_l)) return true;
  }

  return false;
}


void Container::GetRect(RECT& a_rect)
{
  for (size_t i = 0; i < m_elements.size(); ++i)
  {
    RECT rect;
    m_elements[i]->GetRect(rect);

    // Merge rect | a_rect
    if (rect.top < a_rect.top) a_rect.top = rect.top;
    if (rect.bottom > a_rect.bottom) a_rect.bottom = rect.bottom;
    if (rect.left < a_rect.left) a_rect.left = rect.left;
    if (rect.right > a_rect.right) a_rect.right = rect.right;
  }
}

} // namespace
