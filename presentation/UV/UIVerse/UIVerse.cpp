#include "UIVerse.h"
#include "Page.h"
#include "DrawManager.h"


namespace UV
{
  Core::Core()
  {
    m_fm = NULL;
    m_rm = NULL;

    DrawManager::Init();
  }


  Core::~Core()
  {
    // delete elements
    for (size_t i = 0; i < m_elements.size(); ++i)
    {
      delete m_elements[i];
    }

    // delete pages
    for (size_t i = 0; i < m_pages.size(); ++i)
    {
      delete m_pages[i];
    }

    DrawManager::ShutDown();
  }


  void Core::OnPaint()
  {
  /*
    // Draw current page
    if (m_page) 
      m_page->Draw();

  */
    Draw();
  }


  void Core::OnClientAreaChanged(int a_x, int a_y)
  {
    if (DrawManager::GetFontManager())
    {
      DrawManager::GetFontManager()->m_height = a_y;
    }

    if (DrawManager::GetRectManager())
    {
      DrawManager::GetRectManager()->OnResize(a_x, a_y);
    }
  }


  Page* Core::CreatePage()
  {    
    Page* page = new Page();
    m_pages.push_back(page);
    return page;
  }


  void Core::SetFontManager(FontManager* a_fm)
  {
    m_fm = a_fm;
    DrawManager::Init(m_fm, m_rm);
  }


  void Core::SetRectManager(RectManager* a_rm)
  {
    m_rm = a_rm;
    DrawManager::Init(m_fm, m_rm);
  }

} // namespace
