#ifndef __IMAGEBUTTON_H__
#define __IMAGEBUTTON_H__


#include "Button.h"


namespace UV
{

  class ImageButton : public Button
  {
  public:
    ImageButton(Declaration a_decl);
    ~ImageButton();

	void SetImageFile(const std::string& a_filename);

	void Draw();

  protected:

	  unsigned long m_handle;

  };
}


#endif//__IMAGEBUTTON_H__