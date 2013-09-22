#include "Banner.h"
#include "DrawManager.h"


namespace UV
{

  Banner::Banner(int a_height, double a_cornerRadius, double a_borderThickness, unsigned long a_borderColor, unsigned long a_fillGradientTop, unsigned long a_fillGradientBottom)
  {
    m_height = a_height;
    m_cornerRadius = a_cornerRadius;
    m_borderThickness = a_borderThickness;
    m_borderColor = a_borderColor;
    m_fillGradientTop = a_fillGradientTop;
    m_fillGradientBottom = a_fillGradientBottom;
  }


  Banner::~Banner()
  {
  }


  void Banner::OnDeviceLost()
  {
  }


  void Banner::OnDeviceReset()
  {

  }


  void Banner::OnDeviceCreated()
  {
    Create();
  }


  void Banner::Create()
  {
  }

} // namespace
