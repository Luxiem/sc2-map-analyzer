#include "UVTab.h"
#include "Page.h"
#include "DrawManager.h"


namespace UV
{


Tab::Tab()
{
  m_hover = -1;
  m_active = -1;
}


Tab::~Tab()
{
}

    
void Tab::OnMouseMove(long a_x, long a_y)
{
  if (m_pages.size() == 0) return;

  // Check mouseover item
  int offset = m_decl.Rect.left;
  int y = a_y - offset;

  if (a_x >= m_decl.Rect.left && 
      a_x <= m_decl.Rect.right &&
      a_y >= m_decl.Rect.top && 
      a_y <= m_decl.Rect.bottom)
  {
    m_hover = (a_x - m_decl.Rect.left) * m_pages.size() / (m_decl.Rect.right - m_decl.Rect.left);
  }
  else
  {
    m_hover = -1;

    // Page
    if (m_pages.size() && m_active >= 0)
    {
      if (m_pages[m_active])
      {
        m_pages[m_active]->OnMouseMove(a_x, a_y);
      }
    }
  }
}


bool Tab::OnMousePressed(unsigned short a_x, unsigned short a_y)
{
  if (m_pages.size() == 0) return false;

  // Check mouseover item
  int offset = m_decl.Rect.left;
  int y = a_y - offset;

  if (a_x >= m_decl.Rect.left && 
      a_x <= m_decl.Rect.right &&
      a_y >= m_decl.Rect.top && 
      a_y <= m_decl.Rect.bottom)
  {
    m_active = (a_x - m_decl.Rect.left) * m_pages.size() / (m_decl.Rect.right - m_decl.Rect.left);
  }
  else
  {
    // Page
    if (m_pages.size() && m_active >= 0)
    {
      if (m_pages[m_active])
      {
        m_pages[m_active]->OnMousePressed(a_x, a_y);
      }
    }
  }

  return false;
}


bool Tab::OnMouseReleased(unsigned short a_x, unsigned short a_y)
{
  // Page
  if (m_pages.size() && m_active >= 0)
  {
    if (m_pages[m_active])
    {
      m_pages[m_active]->OnMouseReleased(a_x, a_y);
    }
  }

  return false;
}


void Tab::Draw()
{
  // Background
  Declaration2 decl2 = Declaration2();
  decl2.Rect = m_decl.Rect;

  decl2.Color0 = 0xff555555;
  decl2.Color1 = 0xff555555;
  decl2.Color2 = 0xff555555;
  decl2.Color3 = 0xff555555;
  DrawManager::GetRectManager()->Draw(decl2);

  // Tabs
  if (m_tabNames.size())
  {
    int x = 0;
    int y = 0;
    int size = (m_decl.Rect.right - m_decl.Rect.left) / m_tabNames.size();
    for (int i = 0; i < m_tabNames.size(); ++i)
    {    
      RECT rect;
      rect.left = m_decl.Rect.left + 4 + i * size;
      rect.top = m_decl.Rect.top + 14;
      rect.right = rect.left + size - 4;
      rect.bottom = m_decl.Rect.bottom;
      
      decl2.Rect = rect;
      decl2.Rect.bottom -= 1;

      decl2.Color0 = 0xff555555;
      decl2.Color1 = 0xff555555;
      decl2.Color2 = 0xff555555;
      decl2.Color3 = 0xff555555;
      decl2.Fill = false;
      DrawManager::GetRectManager()->Draw(decl2);

      if (i == m_active)
      {
        decl2.Color0 = 0xff8a8a8a;
        decl2.Color1 = 0xff8a8a8a;
        decl2.Color2 = 0xff8a8a8a;
        decl2.Color3 = 0xff8a8a8a;
      }
      else if (i == m_hover)
      {
        decl2.Color0 = 0xffd2d2d2;
        decl2.Color1 = 0xffd2d2d2;
        decl2.Color2 = 0xffd2d2d2;
        decl2.Color3 = 0xffd2d2d2;
      }
      else
      {
        decl2.Color0 = 0xff555555;
        decl2.Color1 = 0xff555555;
        decl2.Color2 = 0xff555555;
        decl2.Color3 = 0xff555555;
      }

      decl2.Rect = rect;
      decl2.Fill = true;
      DrawManager::GetRectManager()->Draw(decl2);


      DrawManager::GetFontManager()->Draw(rect.left + 6, rect.top + 18, 0, m_tabNames[i].c_str());
    }
  }

  // Page
  if (m_pages.size() && m_active >= 0)
  {
    if (m_pages[m_active])
    {
      m_pages[m_active]->Draw();
    }
  }
}


void Tab::SetPosition(int a_x, int a_y)
{
  int w = m_decl.Rect.right - m_decl.Rect.left;
  int h = m_decl.Rect.bottom - m_decl.Rect.top;

  m_decl.Rect.left = a_x;
  m_decl.Rect.top = a_y;

  m_decl.Rect.right = m_decl.Rect.left + w;
  m_decl.Rect.bottom = m_decl.Rect.top + h;
}


void Tab::SetSize(int a_width, int a_height)
{
  m_decl.Rect.right = m_decl.Rect.left + a_width;
  m_decl.Rect.bottom = m_decl.Rect.top + a_height;
}


void Tab::AddTab(const std::string& a_name, Page* a_page)
{
  m_tabNames.push_back(a_name);
  m_pages.push_back(a_page);

  if (m_active == -1)
    m_active = m_tabNames.size() - 1;
}


} // namespace
