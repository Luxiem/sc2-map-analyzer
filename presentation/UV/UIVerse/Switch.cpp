#include "Switch.h"
#include "DrawManager.h"


namespace UV
{


Switch::Switch(Declaration a_decl)
{
  m_state = NORMAL;
  memset(&m_rect, 0, sizeof(RECT));

  m_decl = a_decl;

  m_rect.left = m_decl.HMargin;
  m_rect.top = m_decl.VMargin;
  
  m_rect.right = m_rect.left + 20;
  m_rect.bottom = m_rect.top + 20;

  m_show = true;

  m_toggle = true;
}


Switch::~Switch()
{

}


bool Switch::MouseOver(unsigned short a_x, unsigned short a_y)
{
  return (a_x >= m_rect.left && m_rect.right >= a_x &&
          a_y >= m_rect.top && m_rect.bottom >= a_y);
}


void Switch::OnMouseMove(long a_x, long a_y)
{
  if (!m_show) return;

  if (MouseOver((unsigned short)a_x, (unsigned short)a_y))
  {
    if (m_state == NORMAL)
    {
      m_state = HOVER;
    }
  }
  else
  {
    if (m_state == HOVER)
    {
      m_state = NORMAL;
    }
  }
}


bool Switch::OnMousePressed(unsigned short a_x, unsigned short a_y)
{
  if (!m_show) return false;

  if (m_state == HOVER)
  {
    m_state = PRESSED;
  }

  return false;
}


bool Switch::OnMouseReleased(unsigned short a_x, unsigned short a_y)
{
  if (!m_show) return false;

  if (MouseOver((unsigned short)a_x, (unsigned short)a_y))
  {
    if (m_state != HOVER)
    {
      m_state = HOVER;
    }

    // Execute 'Pressed' command
    if (m_decl.m_commandPressed) 
    {
      EventArgs args;
      m_decl.m_commandPressed(this, args);
    }

    Toggle();

    return true;
  }
  else
  {
    if (m_state != NORMAL)
    {
      m_state = NORMAL;
    }
  }

  return false;
}


void Switch::Draw()
{
  if (!m_show) return;

  RectManager* a_rm = DrawManager::GetRectManager();

  m_decl.Rect = m_rect;

  Declaration2 decl2;
  decl2.Rect = m_rect;

  // Background Fill
  decl2.Fill = true;
  decl2.Rect.left += 1;
  decl2.Rect.right -= 6;
  decl2.Rect.top += 4;
  decl2.Rect.bottom -= 3;
  switch (m_state)
  {
      case NORMAL:
        decl2.Color0 = 0xffd0d0d0;
        decl2.Color1 = 0xffa0a0a0;
        decl2.Color2 = 0xffd0d0d0;
        decl2.Color3 = 0xffa0a0a0;
      break;

      case HOVER:
        decl2.Color0 = 0xffd2d2d2;
        decl2.Color1 = 0xfffafafa;
        decl2.Color2 = 0xffd2d2d2;
        decl2.Color3 = 0xfffafafa;
      break;

      case PRESSED:
        decl2.Color0 = 0xffd2d2d2;
        decl2.Color1 = 0xfffafafa;
        decl2.Color2 = 0xffd2d2d2;
        decl2.Color3 = 0xfffafafa;
      break;
  }

  a_rm->Draw(decl2);

  // Border
  decl2.Color0 = 0xff555555;
  decl2.Color1 = 0xff555555;
  decl2.Color2 = 0xff555555;
  decl2.Color3 = 0xff555555;
  decl2.Fill = false;
  a_rm->Draw(decl2);

  // Toggle
  if (m_toggle)
  {
    decl2.Color0 = 0xff555555;
    decl2.Color1 = 0xff555555;
    decl2.Color2 = 0xff555555;
    decl2.Color3 = 0xff555555;
    decl2.Fill = true;
    decl2.Rect.left += 2;
    decl2.Rect.right -= 3;
    decl2.Rect.top += 3;
    decl2.Rect.bottom -= 2;
    a_rm->Draw(decl2);
  }
  
  //
  if (m_text.size())
  {
    if (DrawManager::GetFontManager())
    {
      DrawManager::GetFontManager()->Draw(
        m_rect.right + 4, 
        m_rect.bottom - 4,
        0, // Font type
        m_text.c_str());
    }
  }
}


void Switch::SetText(const char* text, unsigned long colour)
{
  /*m_rect.right = m_rect.left + 120;
  m_rect.bottom = m_rect.top + 28;*/

  m_text = std::string(text);
  m_colour = colour;
}


void Switch::SetPosition(int a_x, int a_y)
{
  m_decl.HMargin = a_x;
  m_decl.VMargin = a_y;

  m_rect.left = m_decl.HMargin;
  m_rect.top = m_decl.VMargin;
  
  m_rect.right = m_rect.left + 20;
  m_rect.bottom = m_rect.top + 20;
}


void Switch::GetRect(RECT& a_rect)
{
  a_rect.top = m_rect.top;
  a_rect.bottom = m_rect.bottom;
  a_rect.left = m_rect.left;
  a_rect.right = m_rect.right;
}


} // namespace