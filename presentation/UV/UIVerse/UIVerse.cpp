#include "UIVerse.h"
#include "Page.h"
#include "DrawManager.h"


namespace UV
{
  Core::Core()
  {
    m_page = NULL;

    m_fm = NULL;
    m_rm = NULL;

    DrawManager::Init();

    // Grey 'hover'
    //DrawManager::GetSpriteManager()->CreateBanner(0, 30, 3.0, 1.0, D3DCOLOR_XRGB(130, 135, 144), D3DCOLOR_XRGB(211, 211, 211), D3DCOLOR_XRGB(211, 211, 211));

    // Yellow 'select'
    //DrawManager::GetSpriteManager()->CreateBanner(1, 30, 3.0, 1.0, D3DCOLOR_XRGB(130, 135, 144), D3DCOLOR_XRGB(255, 217, 108), D3DCOLOR_XRGB(255, 217, 108));
  }


  Core::~Core()
  {
    if (m_page) delete m_page;

    DrawManager::ShutDown();
  }


  bool Core::OnLeftMouseDown(int a_x, int a_y)
  {
    if (m_page) 
      return m_page->OnMousePressed(a_x, a_y);

    return false;
  }


  void Core::OnLeftMouseUp(int a_x, int a_y)
  {
    if (m_page) 
      m_page->OnMouseReleased(a_x, a_y);
  }


  void Core::OnMouseMove(int a_x, int a_y)
  {
    if (m_page) 
      m_page->OnMouseMove(a_x, a_y);
  }


  void Core::OnScrollWheel(int a_delta)
  {

  }


  void Core::OnPaint()
  {
    // Draw current page
    if (m_page) 
      m_page->Draw();
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
    if (m_page) return m_page;
    m_page = new Page();
    return m_page;
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
