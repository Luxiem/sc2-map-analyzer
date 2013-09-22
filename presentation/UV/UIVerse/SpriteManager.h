#pragma once


#include "Widget.h" // declaration
#include <map>


namespace UV
{
  class DrawSprite;
  class DrawFont;
  class Banner;


  class SpriteManager
  {
  public:
    
    SpriteManager();
    ~SpriteManager();
    
  protected:

    std::map<int, Banner*> m_banners;
  };
}
