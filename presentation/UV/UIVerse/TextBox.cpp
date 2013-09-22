#include "TextBox.h"
#include "DrawManager.h"
#include "RectManager.h"
#include "SpriteManager.h"


#define COLOR_SELECTION           0xffafd9e4
#define COLOR_SELECTION_IANCTIVE  0xffb9bbba


namespace UV
{


TextBox::TextBox()
{
  m_width = 0;
  m_height = 0;
  memset((void*)&m_rect, 0, sizeof(RECT));

  m_hover = false;
  m_selected = false;
  m_dragText = false;

  m_cursorStart = 0;
  m_cursorEnd = 0;

  m_pixelOffset = 0;

  m_callback = NULL;
}


TextBox::~TextBox()
{
}


void TextBox::OnMouseMove(long a_x, long a_y)
{
  m_hover = PointInRect(a_x, a_y, m_rect);
}


bool TextBox::OnMousePressed(unsigned short a_x, unsigned short a_y)
{
  
  return false;
}


bool TextBox::OnMouseReleased(unsigned short a_x, unsigned short a_y)
{
  if (m_dragText)
  {
    m_dragText = false;
    return true;
  }

  return false;
}


bool TextBox::OnKeyPressed(int a_w, int a_l)
{
  if (m_selected)
  {
    /*switch (a_w)
    {
    case VK_HOME:
      m_cursorStart = 0;
      m_cursorEnd = 0;
      ScrollToChar(m_cursorStart);
      break;

    case VK_END:
      m_cursorStart = m_text.length();
      m_cursorEnd = m_text.length();
      ScrollToChar(m_cursorStart);
      break;

    case VK_LEFT:
      m_cursorStart -= 1;
      if (m_cursorStart < 0) m_cursorStart = 0;
      m_cursorEnd = m_cursorStart;
      ScrollToChar(m_cursorEnd);
      break;

    case VK_RIGHT:
      m_cursorEnd += 1;
      if (m_cursorEnd > m_text.length()) m_cursorEnd = m_text.length();
      m_cursorStart = m_cursorEnd;
      ScrollToChar(m_cursorStart);
      break;
      
    case VK_BACK:
      if (m_cursorStart != m_cursorEnd)
      {
        m_text.erase(m_cursorStart, m_cursorEnd - m_cursorStart);
        m_cursorEnd = m_cursorStart;
        ScrollToChar(m_cursorStart);
      }
      else
      {
        if (m_cursorStart > 0)
        {
          m_text.erase(m_cursorStart - 1, 1);
          --m_cursorStart;
          m_cursorEnd = m_cursorStart;
          ScrollToChar(m_cursorStart);
        }
      }
      break;

    case VK_DELETE:
      if (m_cursorStart != m_cursorEnd)
      {
        m_text.erase(m_cursorStart, m_cursorEnd - m_cursorStart);
        m_cursorEnd = m_cursorStart;
        ScrollToChar(m_cursorStart);
      }
      else
      {
        if (m_cursorStart < m_text.length())
        {
          m_text.erase(m_cursorStart, 1);
          ScrollToChar(m_cursorStart);
        }
      }
      break;

    default:
      return false;
      break;
    }*/

    return true;
  }

  return false;
}


bool TextBox::OnChar(int a_w, int a_l)
{
	/*
  if (m_selected)
  {
    switch (a_w)
    {
    case VK_HOME:
      m_cursorStart = 0;
      m_cursorEnd = 0;
      ScrollToChar(m_cursorStart);
      break;

    case VK_END:
      m_cursorStart = m_text.length();
      m_cursorEnd = m_text.length();
      ScrollToChar(m_cursorStart);
      break;

    case VK_LEFT:
      m_cursorStart -= 1;
      if (m_cursorStart < 0) m_cursorStart = 0;
      m_cursorEnd = m_cursorStart;
      ScrollToChar(m_cursorEnd);
      break;

    case VK_RIGHT:
      m_cursorEnd += 1;
      if (m_cursorEnd > m_text.length()) m_cursorEnd = m_text.length();
      m_cursorStart = m_cursorEnd;
      ScrollToChar(m_cursorStart);
      break;

    case VK_RETURN:
      if (m_callback) m_callback(this);
      m_selected = false;
      break;

    case VK_BACK:
      if (m_cursorStart != m_cursorEnd)
      {
        m_text.erase(m_cursorStart, m_cursorEnd - m_cursorStart);
        m_cursorEnd = m_cursorStart;
        ScrollToChar(m_cursorStart);
      }
      else
      {
        if (m_cursorStart > 0)
        {
          m_text.erase(m_cursorStart - 1, 1);
          --m_cursorStart;
          m_cursorEnd = m_cursorStart;
          ScrollToChar(m_cursorStart);
        }
      }
      break;

    case VK_DELETE:
      if (m_cursorStart != m_cursorEnd)
      {
        m_text.erase(m_cursorStart, m_cursorEnd - m_cursorStart);
        m_cursorEnd = m_cursorStart;
        ScrollToChar(m_cursorStart);
      }
      else
      {
        if (m_cursorStart < m_text.length())
        {
          m_text.erase(m_cursorStart, 1);
          ScrollToChar(m_cursorStart);
        }
      }
      break;

    // Junk characters we don't want in the string
    case 26:  // Ctrl Z
    case 2:   // Ctrl B
    case 14:  // Ctrl N
    case 19:  // Ctrl S
    case 4:   // Ctrl D
    case 6:   // Ctrl F
    case 7:   // Ctrl G
    case 10:  // Ctrl J
    case 11:  // Ctrl K
    case 12:  // Ctrl L
    case 17:  // Ctrl Q
    case 23:  // Ctrl W
    case 5:   // Ctrl E
    case 18:  // Ctrl R
    case 20:  // Ctrl T
    case 25:  // Ctrl Y
    case 21:  // Ctrl U
    case 9:   // Ctrl I
    case 15:  // Ctrl O
    case 16:  // Ctrl P
    case 27:  // Ctrl [
    case 29:  // Ctrl ]
    case 28:  // Ctrl \ 
        break;
        
    default:
      {
        // Erase selection
        if (m_cursorStart != m_cursorEnd)
        {
          m_text.erase(m_cursorStart, m_cursorEnd);
          m_cursorEnd = m_cursorStart;
        }

        m_text.insert(m_cursorStart, 1, (char)a_w);
        m_cursorStart += 1;
        m_cursorEnd = m_cursorStart;

        ScrollToChar(m_cursorStart);

        break;
      }
    }

    return true;
  }
*/
  return false;
}


void TextBox::OnDeviceReset()
{
}


void TextBox::OnDeviceLost()
{
}


void TextBox::Draw()
{
}


void TextBox::SetPosition(int a_x, int a_y)
{
  m_rect.left = a_x;
  m_rect.top = a_y;
  m_rect.right = a_x + m_width;
  m_rect.bottom = a_y + m_height;
}


void TextBox::SetSize(int a_width, int a_height)
{
  m_width = a_width;
  m_height = a_height;
  m_rect.right = m_rect.left + m_width;
  m_rect.bottom = m_rect.bottom + m_height;
}


void TextBox::SetText(const char* a_text)
{
  m_text = std::string(a_text);
}


const char* TextBox::GetText()
{
  return m_text.c_str();
}


void TextBox::SetCallback(Callback a_callback)
{
  m_callback = a_callback;
}


void TextBox::ScrollToChar(int a_ch)
{
}


} // namespace
