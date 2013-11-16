#pragma once


namespace UV
{

  class FontManager
  {
  public:
    
    virtual void Draw(float x, float y, int type, const char *fmt) {}

    virtual void Init() {}

    int m_height;

  };

} // namespace
