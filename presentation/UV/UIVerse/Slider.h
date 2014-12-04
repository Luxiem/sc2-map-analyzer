#pragma once


#include "Widget.h"


namespace UV
{
  class Slider : public Widget
  {
  public:
    enum ORIENTATION
    {
      HORIZONTAL = 0,
      VERTICAL
    };

    Slider(ORIENTATION a_orientation);
    ~Slider();

    void OnMouseMove(long a_x, long a_y);

    bool OnMousePressed(unsigned short a_x, unsigned short a_y);
    bool OnMouseReleased(unsigned short a_x, unsigned short a_y);

    void OnDeviceReset();

    void Draw();

    void SetPosition(int a_x, int a_y);
    void SetSize(int a_width, int a_height);

    double GetValue();
    void SetValue(double a_value);
    void SetDataRange(double a_min, double a_max);
    void SetScale(double a_scale);

    SystemCommand m_callback;

    void GetRect(RECT& a_rect);

  protected:

    void Resize();
    bool Collide(const RECT& a_rect, long a_x, long a_y);
    void AdjustHandle();

    RECT m_rectLess;
    RECT m_rectMore;
    RECT m_rectBar;
    enum HOVERSTATE
    {
      NONE = 0,
      LESS_SMALL,
      LESS_LARGE,
      HANDLE,
      MORE_LARGE,
      MORE_SMALL,
    };
    HOVERSTATE m_hoverState;
    bool m_click;
    bool m_vertical;
    unsigned int m_width;
    unsigned int m_height;

    double m_value;
    double m_min;
    double m_max;
    double m_smallStep;
    double m_largeStep;
    double m_scale;
    int m_clickOffset;
  };
}
