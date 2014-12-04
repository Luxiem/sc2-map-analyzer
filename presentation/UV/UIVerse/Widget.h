#ifndef __WIDGET_H__
#define __WIDGET_H__


#include <memory.h>


namespace UV
{
  typedef struct tagRECT
  {
    long    left;
    long    top;
    long    right;
    long    bottom;
  } RECT;


  class EventArgs
  {
  };


  typedef void (*SystemCommand)(void*, EventArgs&);


  class Declaration
  {
  public:

    RECT Rect;
    int Align;
    int Style;
    unsigned long Resource;

    SystemCommand m_commandPressed;
    SystemCommand m_commandReleased;
    SystemCommand m_commandHover;
    SystemCommand m_commandMouseLeave;

    Declaration()
    {
      memset(this, 0, sizeof(Declaration));
    }

    ~Declaration() {}
  };


  // Rectangle Declaration
  class Declaration2
  {
  public:

    Declaration2()
    {
      Color0 = 0xfffafafa;
      Color1 = 0xffd2d2d2;
      Color2 = 0xfffafafa;
      Color3 = 0xffd2d2d2;

      Fill = true;
    };

    RECT Rect;
    unsigned long Color0;
    unsigned long Color1;
    unsigned long Color2;
    unsigned long Color3;

    bool Fill;
  };


  class Widget
  {
  public:
    Widget() :
      m_parent(0),
      m_userData(0),
      m_textAlignH(ALIGN_NEAR),
      m_textAlignV(ALIGN_NEAR)
    {
    }

    virtual ~Widget() 
    {
      if (m_parent) m_parent->OnChildRemoved(this);
    }

    //
    enum Stretch // ?
    {
      OVERLAP,
      FILL,
    };

    enum Align
    {
      ALIGN_NEAR = 0,
      ALIGN_CENTER,
      ALIGN_FAR,
    };

    // Input
    virtual void OnMouseMove(long a_x, long a_y) = 0;

    virtual bool OnMousePressed(unsigned short a_x, unsigned short a_y) = 0;
    virtual bool OnMouseReleased(unsigned short a_x, unsigned short a_y) = 0;

    virtual bool OnKeyPressed(int a_w, int a_l) { return false; }
    virtual bool OnKeyReleased(int a_w, int a_l) { return false; }
    virtual bool OnChar(int a_w, int a_l) { return false; }

    // Render the widget
    virtual void Draw() = 0;

    // Layout object
    //virtual void SetParent(const Layout& a_parent) = 0;
    virtual void SetPosition(int a_x, int a_y) {}
    virtual void SetSize(int a_width, int a_height) {}
    virtual void Anchor() {}
    virtual void SetMargin() {}
    virtual void SetStretch() {}

    virtual void SetTextAlignH(Align a_align) { m_textAlignH = a_align; }
    virtual void SetTextAlignV(Align a_align) { m_textAlignV = a_align; }

    void SetParent(Widget* a_parent)
    {
      if (m_parent) m_parent->OnChildRemoved(this);
      m_parent = a_parent;
      if (m_parent) m_parent->OnChildAdded(this);
    }

    virtual void GetRect(RECT& a_rect)
    {
      a_rect.top = 0;
      a_rect.bottom = 0;
      a_rect.left = 0;
      a_rect.right = 0;
    }

    virtual void* GetUserData()
    {
      return m_userData;
    }

    virtual void SetUserData(void* a_data)
    {
      m_userData = a_data;
    }
    
  protected:

    // Helper function
    static const bool PointInRect(int a_x, int a_y, const RECT& a_rect)
    {
      return a_x >= a_rect.left && 
             a_x <= a_rect.right &&
             a_y >= a_rect.top &&
             a_y <= a_rect.bottom;
    }

    // Heirarchy
    virtual void OnChildAdded(Widget* a_widget) {}
    virtual void OnChildRemoved(Widget* a_widget) {}

    Widget* m_parent;

    void* m_userData;

    Align m_textAlignH;
    Align m_textAlignV;
  };
  

  class Callback
  {
  public:
    virtual void Execute(UV::Widget* a_caller, UV::EventArgs& a_args) const = 0;
  }; 
}

     
template <class C>
class TCallback : public UV::Callback
{
public:


  typedef void (C::*CallbackFunction)(UV::Widget* a_caller, UV::EventArgs& a_args);


  TCallback()
  {
    m_instance = 0;
    m_fn = 0;
  }


  TCallback(C* a_InstancePointer, CallbackFunction a_FunctionPointer)
  {
    m_instance = a_InstancePointer;
    m_fn = a_FunctionPointer;
  }
     

  virtual void Execute(UV::Widget* a_caller, UV::EventArgs& a_args) const
  {
    if (m_instance)
    {
      if (m_fn)
      { 
        (m_instance->*m_fn)(a_caller, a_args);
      }
    }
    else
    {
      // static fn
    }
  }


  void SetCallback(C* a_InstancePointer, CallbackFunction a_FunctionPointer)
  {
    m_instance = a_InstancePointer;
    m_fn = a_FunctionPointer;
  }

     
private:
  C* m_instance;
  CallbackFunction m_fn;
};



#endif//__WIDGET_H__