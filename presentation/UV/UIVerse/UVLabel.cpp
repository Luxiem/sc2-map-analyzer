#include "UVLabel.h"
#include "DrawManager.h"

//#include <Windows.h>
//#include <GL/gl.h> // for set colour
 

namespace UV
{


Label::Label() : Widget()
{
  m_colour = 0xffffffff;
  m_rect.top = 0;
  m_rect.bottom = 0;
  m_rect.left = 0;
  m_rect.right = 0;
}


Label::~Label()
{

}


void Label::Draw()
{ 
  // Set Colour
  //glColor3f(0.f, 0.f, 0.f);

  // TODO break on newline

  // Draw Text 
  if (m_text.size())
  {
    if (DrawManager::GetFontManager())
    {
      // Calc text width
      switch (m_textAlignH)
      {
      case ALIGN_CENTER:
        {
          //float w, h;
          //DrawManager::GetFontManager()->Measure(w, h,  1, m_text.c_str());
          //float o = ((m_rect.right - m_rect.left) - w) / 2;
          //DrawManager::GetFontManager()->Draw(m_rect.left + o, (m_rect.top + m_rect.bottom) / 2 + 8, 1, m_text.c_str());
        }
        break;
      case ALIGN_NEAR:
        {
          DrawManager::GetFontManager()->Draw(m_rect.left + 2, (m_rect.top + m_rect.bottom) / 2 + 8, 1, m_text.c_str());
        }
        break;
      case ALIGN_FAR:
        {
          //float w, h;
          //DrawManager::GetFontManager()->Measure(w, h,  1, m_text.c_str());
          //DrawManager::GetFontManager()->Draw(m_rect.right - w, (m_rect.top + m_rect.bottom) / 2 + 8, 1, m_text.c_str());
        }
        break;
      } // switch
    }
  }

}


void Label::SetText(const char* text, unsigned long colour)
{
  m_text = std::string(text);
  m_colour = colour;
}


void Label::SetPosition(int a_x, int a_y)
{
  int w = m_rect.right - m_rect.left;
  int h = m_rect.bottom - m_rect.top;

  m_rect.left = a_x;
  m_rect.top = a_y;
  m_rect.right = a_x + w;
  m_rect.bottom = a_y + h;
}


void Label::SetSize(int a_width, int a_height)
{
  m_rect.right = m_rect.left + a_width;
  m_rect.bottom = m_rect.top + a_height;
}


}//namespace