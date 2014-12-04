#pragma once
/**
*	A Frame Buffer Object is used by OpenGL to render into a texture. Specifically this implementation assumes that the
*	rendered model will provide diffuse, position and normal at the same time in a MRT fashion
*/
class FBORenderTexture
{
public:
	// Ctors/Dtors
	FBORenderTexture(int width, int height);
	~FBORenderTexture();

	// Methods
	void	start();
	void	stop();
	void	showTexture(unsigned int i, float fSizeX = 400, float fSizeY = 400, float x = 0, float y = 0) const;

private:

	// Variables
	unsigned int	m_diffuseTexture; // The OpenGL texture for the diffuse render target

	unsigned int	m_width; // FBO width
	unsigned int	m_height; // FBO height
};
