#include "FBORenderTexture.h"
#include <exception>

/**
*	Create the FBO render texture initializing all the stuff that we need
*/
FBORenderTexture::FBORenderTexture(int _dWidth, int _dHeight)
{	
	// Save extensions
	m_width  = _dWidth;
	m_height = _dHeight;

	// Generate the OGL resources for what we need
	glGenFramebuffersEXT(1, &m_fbo);
	glGenRenderbuffersEXT(1, &m_diffuseRT);

	// Bind the FBO so that the next operations will be bound to it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

	// Bind the diffuse render target
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_diffuseRT);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, m_width, m_height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, m_diffuseRT);

	// Generate and bind the OGL texture for diffuse
	glGenTextures(1, &m_diffuseTexture);
	glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Attach the texture to the FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_diffuseTexture, 0);

	// Check if all worked fine and unbind the FBO
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if( status != GL_FRAMEBUFFER_COMPLETE_EXT)
		throw new std::exception("Can't initialize an FBO render texture. FBO initialization failed.");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

/**
*	Destructor
*/
FBORenderTexture::~FBORenderTexture()
{
	glDeleteTextures(1, &m_diffuseTexture);
	glDeleteFramebuffersEXT(1, &m_fbo);
	glDeleteRenderbuffersEXT(1, &m_diffuseRT);
}

/**
*	Start rendering to the texture
*	Both color and depth buffers are cleared.
*/
void FBORenderTexture::start()
{
	// Bind our FBO and set the viewport to the proper size
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,m_width, m_height);

	// Clear the render targets
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
}

/**
*	Stop rendering to this texture.
*/
void FBORenderTexture::stop()
{	
	// Stop acquiring and unbind the FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glPopAttrib();
}

/**
*	Show the texture to screen. It is just for debug purposes.
*/
void FBORenderTexture::showTexture(unsigned int i, float fSizeX, float fSizeY, float x, float y) const 
{
	GLuint texture = m_diffuseTexture;

	//glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Render the quad
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
	glTexCoord2f( 0, 1 );
	glVertex3f(    x,                 y + (float)m_height, 0.0f);
	glTexCoord2f( 0, 0 );
	glVertex3f(    x,                 y, 0.0f);
	glTexCoord2f( 1, 0 );
	glVertex3f(   x + (float)m_width, y, 0.0f);
	glTexCoord2f( 1, 1 );
	glVertex3f(   x + (float)m_width, y + (float)m_height, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}