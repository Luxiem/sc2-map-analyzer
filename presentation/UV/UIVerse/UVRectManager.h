#ifndef _SPRITEMANAGER_H_
#define _SPRITEMANAGER_H_


#include "Widget.h" // declaration


namespace UV
{
  class DrawSprite;
  class DrawFont;


  class RectManager
  {
  public:
    
    RectManager();
    ~RectManager();

    void Clear();

    void OnDeviceReset(); 

    void OnDeviceLost(); 
    
    virtual void OnResize(unsigned short a_x, unsigned short a_y);

    virtual void Draw(const Declaration2& a_decl);
    
    void DrawOutline(const Declaration2& a_decl);

  protected:

    unsigned short m_height;
    unsigned short m_width;
  };
}


#endif//_SPRITEMANAGER_H_