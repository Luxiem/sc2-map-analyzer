#pragma once


#include "UVRectManager.h"
#include "UVFontManager.h"


namespace UV
{
  class DrawManager
  {
  public:
    static void Init(FontManager* a_fm = 0, RectManager* a_rm = 0);

    static void ShutDown();

    static RectManager* GetRectManager() { return s_rm; }
    static FontManager* GetFontManager() { return s_fm; }

    static void OnDeviceReset();

  protected:
    static RectManager* s_rm;
    static FontManager* s_fm;
  };

}
