#include "Container.h"
#include "DrawManager.h"
#include "RectManager.h"


namespace UV
{


Container::Container() : Widget()
{
  m_borderWidth = 0;
  m_borderColour = 0;

  m_backgroundColour = 0;

  m_x = 0;
  m_y = 0;
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
  if (m_borderWidth)
  {
    RectManager* rm = DrawManager::GetRectManager();
    UV::Declaration2 r;
    GetRect(r.Rect);
    r.Fill = false;
    r.Color0 = m_borderColour;
    r.Color1 = m_borderColour;
    r.Color2 = m_borderColour;
    r.Color3 = m_borderColour;
    rm->Draw(r);
  }

  if (m_backgroundColour)
  {
	RectManager* rm = DrawManager::GetRectManager();
    UV::Declaration2 r;
    //GetRect(r.Rect);
	r.Rect.left = m_x;
	r.Rect.top = m_y;
	r.Rect.right = m_x + 270;
	r.Rect.bottom = m_y + 96;
    r.Fill = true;
    r.Color0 = m_backgroundColour;
    r.Color1 = m_backgroundColour;
    r.Color2 = m_backgroundColour;
    r.Color3 = m_backgroundColour;
    rm->Draw(r);
  }

  // Draw children
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


void Container::SetBorder(int a_borderWidth, unsigned long a_borderColour)
{
  m_borderWidth = a_borderWidth;
  m_borderColour = a_borderColour;
}


void Container::SetPosition(int a_x, int a_y)
{
	// Move all children
    RECT rect;
	long dx;
	long dy;

  for (size_t i = 0; i < m_elements.size(); ++i)
  {
    m_elements[i]->GetRect(rect);

    dx = -m_x + a_x;
	dy = -m_y + a_y;

	m_elements[i]->SetPosition(rect.left + dx, rect.top + dy);
  }

  m_x = a_x;
  m_y = a_y;
}

} // namespace
