#include "Button.h"
#include "DrawManager.h"
#include "ResourceManager.h"


namespace UV
{


Button::Button(Declaration a_decl)
{
  m_state = NORMAL;
  m_rm = 0;
  memset(&m_decl, 0, sizeof(Declaration));
  memset(&m_rect, 0, sizeof(RECT));

  m_decl = a_decl;

  m_rect.left = m_decl.HMargin;
  m_rect.top = m_decl.VMargin;
  
  m_rect.right = m_rect.left + 48;
  m_rect.bottom = m_rect.top + 48;

  m_show = true;
}


Button::~Button()
{

}


bool Button::MouseOver(unsigned short a_x, unsigned short a_y)
{
  return (a_x >= m_rect.left && m_rect.right >= a_x &&
          a_y >= m_rect.top && m_rect.bottom >= a_y);
}


void Button::OnMouseMove(long a_x, long a_y)
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


bool Button::OnMousePressed(unsigned short a_x, unsigned short a_y)
{
  if (!m_show) return false;

  if (m_state == HOVER)
  {
    m_state = PRESSED;
  }

  return false;
}


bool Button::OnMouseReleased(unsigned short a_x, unsigned short a_y)
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
      return true;
    }
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


void Button::OnDeviceReset()
{
  
}


void Button::Draw()
{
  if (!m_show) return;

  RectManager* a_rm = DrawManager::GetRectManager();

  // TODO Calculate layout rect if parent is set

  m_decl.Rect = m_rect;

  Declaration2 decl2;
  decl2.Rect = m_rect;

  // Fill
  decl2.Fill = true;
  switch (m_state)
  {
      case NORMAL:
        decl2.Color0 = 0xffd0d0d0;
        decl2.Color1 = 0xffa0a0a0;
        decl2.Color2 = 0xffd0d0d0;
        decl2.Color3 = 0xffa0a0a0;
      break;

      case HOVER:
        decl2.Color0 = 0xfffafafa;
        decl2.Color1 = 0xffd2d2d2;
        decl2.Color2 = 0xfffafafa;
        decl2.Color3 = 0xffd2d2d2;
      break;

      case PRESSED:
        decl2.Color0 = 0xfffafafa;
        decl2.Color1 = 0xffd2d2d2;
        decl2.Color2 = 0xfffafafa;
        decl2.Color3 = 0xffd2d2d2;
      break;

      case TOGGLED_NORMAL:
        decl2.Color0 = 0xffff0000;
        decl2.Color1 = 0xffff0000;
        decl2.Color2 = 0xffff0000;
        decl2.Color3 = 0xffff0000;
      break;

      case TOGGLED_HOVER:
        decl2.Color0 = 0xffff0000;
        decl2.Color1 = 0xffff0000;
        decl2.Color2 = 0xffff0000;
        decl2.Color3 = 0xffff0000;
      break;

      case TOGGLED_PRESSED:
        decl2.Color0 = 0xffff0000;
        decl2.Color1 = 0xffff0000;
        decl2.Color2 = 0xffff0000;
        decl2.Color3 = 0xffff0000;
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
  
  //
  if (m_text.size())
  {
    if (DrawManager::GetFontManager())
    {
      DrawManager::GetFontManager()->Draw(m_rect.left + 2, (m_rect.top + m_rect.bottom) / 2 + 8, 0, m_text.c_str());
    }
  }
}


void Button::SetText(const char* text, unsigned long colour)
{
  /*m_rect.right = m_rect.left + 120;
  m_rect.bottom = m_rect.top + 28;*/

  m_text = std::string(text);
  m_colour = colour;
}


void Button::SetSize(int a_width, int a_height) 
{ 
  m_rect.right = m_rect.left + a_width;
  m_rect.bottom = m_rect.top + a_height;
}


} // namespace