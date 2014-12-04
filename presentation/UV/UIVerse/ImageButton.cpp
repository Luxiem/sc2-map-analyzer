#include "ImageButton.h"
#include "ImageManager.h"
#include "DrawManager.h"


static UV::ImageManager* s_imageManager = 0;


namespace UV
{


ImageButton::ImageButton(Declaration a_decl) : Button(a_decl)
{
  if (s_imageManager == 0) s_imageManager = new ImageManager();


  m_handle = 0;
}


ImageButton::~ImageButton()
{

}


void ImageButton::SetImageFile(const std::string& a_filename)
{
	m_handle = s_imageManager->AddImage(a_filename);
}


void ImageButton::Draw()
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
  if (m_handle)
  {
	s_imageManager->Draw(m_handle, m_rect.left + 6, m_rect.top + 6);
  }

}


} // namespace