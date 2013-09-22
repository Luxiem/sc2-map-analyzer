#include "DrawManager.h"


namespace UV
{
  RectManager* DrawManager::s_rm = 0;
  FontManager* DrawManager::s_fm = 0;


  void DrawManager::Init(FontManager* a_fm, RectManager* a_rm)
  {
    s_rm = a_rm;

    s_fm = a_fm;
  }


  void DrawManager::ShutDown()
  {
  }


  void DrawManager::OnDeviceReset()
  {
  }

}
