#include "Slider.h"
#include "DrawManager.h"


namespace UV
{

  
Slider::Slider(ORIENTATION a_orientation)
{
  m_callback = 0;

  m_hoverState = NONE;
  m_click = false;
  m_vertical = a_orientation == VERTICAL;
  m_width = 16;
  m_height = 16;

  m_value = 0.0;
  m_min = 0.0;
  m_max = 1.0;
  m_smallStep = 0.01;
  m_largeStep = 0.2;
  m_scale = 0.2;

  AdjustHandle();
}


Slider::~Slider()
{
}


void Slider::OnMouseMove(long a_x, long a_y)
{
  if (m_click)
  {
    if (m_hoverState == HANDLE)
    {
      if (m_vertical)
      {
        int top = a_y + m_clickOffset;
        double value = (double)(top - m_rectLess.bottom) / (double)(m_rectMore.top - m_rectLess.bottom + m_rectBar.top - m_rectBar.bottom);
        SetValue(value);
      }
      else
      {
        int left = a_x + m_clickOffset;
        double value = (double)(left - m_rectLess.right) / (double)(m_rectMore.left - m_rectLess.right + m_rectBar.left - m_rectBar.right);
        SetValue(value);
      }
    }
  }
  else
  {
    m_hoverState = NONE;

    // Collide less (small)
    if (Collide(m_rectLess, a_x, a_y))
    {
      m_hoverState = LESS_LARGE;
      return;
    }

    // Collide more (small)
    if (Collide(m_rectMore, a_x, a_y))
    {
      m_hoverState = MORE_LARGE;
      return;
    }

    // Collide handle
    if (Collide(m_rectBar, a_x, a_y))
    {
      m_hoverState = HANDLE;
      return;
    }  

    // Collide less (large)
    {
      RECT rect;
      rect.left = m_rectLess.left;
      rect.top = m_rectLess.top;
      rect.right = m_vertical ? m_rectLess.left : m_rectBar.left;
      rect.bottom = m_vertical ? m_rectBar.top : m_rectLess.bottom;
      if (Collide(rect, a_x, a_y))
      {
        m_hoverState = LESS_SMALL;
        return;
      }
    }

    // Collide more (large)
    {
      RECT rect;

      rect.left = m_rectMore.left;
      rect.top = m_rectMore.top;
      rect.right = m_vertical ? m_rectMore.left : m_rectBar.left;
      rect.bottom = m_vertical ? m_rectBar.top : m_rectMore.bottom;

      rect.left = m_vertical ? m_rectMore.left : m_rectBar.right;
      rect.top = m_vertical ? m_rectMore.top : m_rectBar.top;
      rect.right = m_rectMore.left;
      rect.bottom = m_vertical ? m_rectBar.top : m_rectMore.bottom;

      if (Collide(rect, a_x, a_y))
      {
        m_hoverState = MORE_SMALL;
        return;
      }  
    }
  }
}


bool Slider::OnMousePressed(unsigned short a_x, unsigned short a_y)
{
  switch (m_hoverState)
  {
  case LESS_SMALL:
    m_click = true;
    SetValue(m_value - m_smallStep);
    break;

  case LESS_LARGE:
    SetValue(m_value - m_largeStep);
    break;

  case HANDLE:
    m_click = true;
    m_clickOffset = m_vertical ? 
      m_rectBar.top - a_y :
      m_rectBar.left - a_x;
    break;

  case MORE_LARGE:
    SetValue(m_value + m_largeStep);
    break;

  case MORE_SMALL:
    SetValue(m_value + m_smallStep);
    m_click = true;
    break;

  case NONE:
  default:
    return false;
    break;
  };

  return true;
}


bool Slider::OnMouseReleased(unsigned short a_x, unsigned short a_y)
{
  m_click = false;
  return false;
}


void Slider::OnDeviceReset()
{

}


void Slider::Draw()
{
  Declaration2 decl = Declaration2();
  
  // background
  decl.Rect.left = m_rectLess.left;
  decl.Rect.top = m_rectLess.top;
  decl.Rect.right = m_rectMore.right;
  decl.Rect.bottom = m_rectMore.bottom;
  if (m_hoverState == LESS_SMALL || m_hoverState == MORE_SMALL)
  {
    decl.Color0 = 0xff555555;
    decl.Color1 = 0xff555555;
    decl.Color2 = 0xff555555;
    decl.Color3 = 0xff555555;
  }
  else
  {
    decl.Color0 = 0xff555555;
    decl.Color1 = 0xff555555;
    decl.Color2 = 0xff555555;
    decl.Color3 = 0xff555555;
  }

  decl.Fill = true;
  DrawManager::GetRectManager()->Draw(decl);

  // less button
  decl.Rect = m_rectLess;
  if (m_hoverState == LESS_LARGE)
  {
    decl.Color0 = 0xffd2d2d2;
    decl.Color1 = 0xfffafafa;
    decl.Color2 = 0xffd2d2d2;
    decl.Color3 = 0xfffafafa;
  }
  else
  {
    decl.Color0 = 0xffd0d0d0;
    decl.Color1 = 0xffa0a0a0;
    decl.Color2 = 0xffd0d0d0;
    decl.Color3 = 0xffa0a0a0;
  }
  decl.Fill = true;
  DrawManager::GetRectManager()->Draw(decl);

  decl.Rect.left += 1;
  decl.Rect.bottom  -= 1;
  decl.Color0 = 0xff555555;
  decl.Color1 = 0xff555555;
  decl.Color2 = 0xff555555;
  decl.Color3 = 0xff555555;
  decl.Fill = false;
  DrawManager::GetRectManager()->Draw(decl);

  // more button
  decl.Rect = m_rectMore;
  if (m_hoverState == MORE_LARGE)
  {
    decl.Color0 = 0xffd2d2d2;
    decl.Color1 = 0xfffafafa;
    decl.Color2 = 0xffd2d2d2;
    decl.Color3 = 0xfffafafa;
  }
  else
  {
    decl.Color0 = 0xffd0d0d0;
    decl.Color1 = 0xffa0a0a0;
    decl.Color2 = 0xffd0d0d0;
    decl.Color3 = 0xffa0a0a0;
  }
  decl.Fill = true;
  DrawManager::GetRectManager()->Draw(decl);

  decl.Rect.left += 1;
  decl.Rect.bottom  -= 1;
  decl.Color0 = 0xff555555;
  decl.Color1 = 0xff555555;
  decl.Color2 = 0xff555555;
  decl.Color3 = 0xff555555;
  decl.Fill = false;
  DrawManager::GetRectManager()->Draw(decl);

  // handle
  decl.Rect = m_rectBar;
  if (m_hoverState == HANDLE)
  {
    decl.Color0 = 0xfffafafa;
    decl.Color1 = 0xffd2d2d2;
    decl.Color2 = 0xfffafafa;
    decl.Color3 = 0xffd2d2d2;
  }
  else
  {
    decl.Color0 = 0xffd0d0d0;
    decl.Color1 = 0xffa0a0a0;
    decl.Color2 = 0xffd0d0d0;
    decl.Color3 = 0xffa0a0a0;
  }
  decl.Fill = true;
  DrawManager::GetRectManager()->Draw(decl);

}


void Slider::SetPosition(int a_x, int a_y)
{
  m_rectLess.left = a_x;
  m_rectLess.top = a_y;
  m_rectLess.right = a_x + (m_vertical ? m_width : m_height);
  m_rectLess.bottom = a_y + (m_vertical ? m_width : m_height);

  m_rectMore.left = a_x + (m_vertical ? 0 : (m_width - m_height));
  m_rectMore.top = a_y + (m_vertical ? (m_height - m_width) : 0);
  m_rectMore.right = m_rectMore.left + (m_vertical ? m_width : m_height);
  m_rectMore.bottom = m_rectMore.top + (m_vertical ? m_width : m_height);

  AdjustHandle();
}


void Slider::SetSize(int a_width, int a_height)
{
  if (a_width > 0)
    m_width = (unsigned int)a_width;

  if (a_height > 0)
    m_height = (unsigned int)a_height;

  SetPosition(m_rectLess.left, m_rectLess.top);
}


void Slider::Resize()
{
}


bool Slider::Collide(const RECT& a_rect, long a_x, long a_y)
{
  return (a_x >= a_rect.left && a_rect.right >= a_x &&
          a_y >= a_rect.top && a_rect.bottom >= a_y);
}


double Slider::GetValue()
{
  return m_value;
}


void Slider::SetValue(double a_value)
{
  m_value = a_value;
  if (m_value < m_min) m_value = m_min;
  if (m_value > m_max) m_value = m_max;

  if (m_callback)
  {
    EventArgs args;
    m_callback(this, args);
  }

  AdjustHandle();
}


void Slider::SetDataRange(double a_min, double a_max)
{
  m_min = a_min;
  m_max = a_max;

  if (m_value < m_min) m_value = m_min;
  if (m_value > m_max) m_value = m_max;

  AdjustHandle();
}


void Slider::SetScale(double a_scale)
{
  m_scale = a_scale;

  AdjustHandle();
}


void Slider::AdjustHandle()
{
  int size = m_vertical ? 
    m_scale * (m_height - 2 * (m_vertical ? m_width : m_height)) :
    m_scale * (m_width - 2 * (m_vertical ? m_width : m_height));

  m_rectBar.left = m_vertical ?
    m_rectLess.left + 1 :
    m_rectLess.right + (m_value - m_min) / (m_max - m_min) * (m_width - 2 * (m_vertical ? m_width : m_height) - size);
  m_rectBar.top = m_vertical ?
    m_rectLess.bottom + (m_value - m_min) / (m_max - m_min) * (m_height - 2 * (m_vertical ? m_width : m_height) - size) :
    m_rectLess.top + 1;
  m_rectBar.right = m_vertical ? 
    m_rectLess.right - 1 :
    m_rectBar.left + size;
  m_rectBar.bottom = m_vertical ? 
    m_rectBar.top + size :
    m_rectLess.bottom - 1;
}


void Slider::GetRect(RECT& a_rect)
{
  a_rect.top = m_rectBar.top;
  a_rect.bottom = m_rectBar.bottom;
  a_rect.left = m_rectBar.left;
  a_rect.right = m_rectBar.right;
}


}
