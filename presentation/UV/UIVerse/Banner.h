#pragma once


namespace UV
{
  class Banner
  {
  public:

    Banner(int a_height, double a_cornerRadius, double a_borderThickness, unsigned long a_borderColor, unsigned long a_fillGradientTop, unsigned long a_fillGradientBottom);
    ~Banner();

    void OnDeviceLost();
    void OnDeviceReset();
    void OnDeviceCreated();

  //protected:

    void Create();

    int m_height;
    double m_cornerRadius;
    double m_borderThickness;
    unsigned long m_borderColor;
    unsigned long m_fillGradientTop;
    unsigned long m_fillGradientBottom;
  };
}
