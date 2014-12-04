#pragma once


#include <vector>
#include <string>


namespace UV
{
  class ImageManager
  {
  public:
    ImageManager();
    ~ImageManager();

    unsigned long AddImage(std::string a_filename);

	void Draw(unsigned long a_id, unsigned long a_x, unsigned long a_y);

	
  protected:

    std::vector<std::string> m_imageNames;
	std::vector<void*> m_surfaceData;
	std::vector<void*> m_imageData;


  };
} // namespace