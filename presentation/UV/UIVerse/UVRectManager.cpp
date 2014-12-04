#include "UVRectManager.h"
#include "DrawManager.h"


//--------------------------------------------------------------------------------------
namespace UV {


RectManager::RectManager()
{
  m_width = 1;
  m_height = 1;
}


RectManager::~RectManager()
{
  Clear();
}


void RectManager::OnDeviceLost()
{
}


void RectManager::OnDeviceReset()
{
}


void RectManager::OnResize(unsigned short a_x, unsigned short a_y)
{
  m_width = a_x;
  m_height = a_y;
}


void RectManager::Clear()
{
}


void RectManager::Draw(const Declaration2& a_decl)
{
}


void RectManager::DrawOutline(const Declaration2& a_decl)
{
}


} // namespace
