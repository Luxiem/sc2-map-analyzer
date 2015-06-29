#include "ImageManager.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Common.h"
#include "View.h"


namespace UV
{


ImageManager::ImageManager()
{

}


ImageManager::~ImageManager()
{

}


unsigned long ImageManager::AddImage(std::string a_filename)
{
	SDL_Surface *image;
	image = IMG_Load(a_filename.c_str());

	m_imageNames.push_back(a_filename);
	m_surfaceData.push_back((void*)image);
	m_imageData.push_back(0);

	return m_imageData.size();
}


void ImageManager::Draw(unsigned long a_id, unsigned long a_x, unsigned long a_y)
{
	int id = a_id - 1;

	SDL_Renderer* renderer = Common::GetView()->Renderer();

	SDL_Surface *surface = (SDL_Surface*)m_surfaceData[id];
	SDL_Texture *tex = (SDL_Texture*)m_imageData[id];

	if (tex == 0)
	{
		tex = SDL_CreateTextureFromSurface(renderer, surface);
		m_imageData[id] = tex;
	}
	
    int width = surface ? surface->w : 0;
    int height = surface ? surface->h : 0;

    SDL_Rect dest = {(int)a_x, (int)a_y, width, height};
    SDL_RenderCopy(renderer, tex, 0, &dest);
}


} // namespace

