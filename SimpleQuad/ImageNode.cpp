
#include <math.h>
#include <GL/glew.h>
#include "ImageNode.h"
#include "GLTex2D.h"


ImageNode::ImageNode(void) : m_ImageWidth(512), m_ImageHeight(512), m_ImageSizeModified(false)
{
	//Initialize Textures
	m_TexImage = new GLTex2D();
	m_TexImage->SetTexUnit(0);
	initTexture();
}


ImageNode::~ImageNode(void)
{
	//Delete textures
	delete m_TexImage;
}

void ImageNode::SetImageSize(int imageWidth, int imageHeight)
{
	if (m_ImageWidth == imageWidth && m_ImageHeight == imageHeight)
		return;

	m_ImageWidth = imageWidth;
	m_ImageHeight = imageHeight;
	//Set dirty flag
	m_ImageSizeModified = true;
}

void ImageNode::SetImageData(unsigned char * imageData)
{
	if (imageData == NULL)
		return;

	if (m_ImageSizeModified)
	{
		//ReInitialize texture
		initTexture();
		
		//Reset the dirty flag
		m_ImageSizeModified = false;
	}

	//Update texture
	m_TexImage->ActiveAndBind();
	m_TexImage->Update(imageData);
	m_TexImage->BindToZero();
}

void ImageNode::drawSelf()
{
	//OpenGL doesn't allow enabling 2D and 1D Texture at the same time
	glDisable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_2D);
	//Bind the texture object
	m_TexImage->ActiveAndBind();
	//Draw a textured Square centered in origin and unit length
	glBegin(GL_QUADS);
	{
		glColor3f(m_ColorR, m_ColorG, m_ColorB);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(0.5f, 0.5f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-0.5f, 0.5f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-0.5f, -0.5f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(0.5f, -0.5f);
	}
	glEnd();
	//Restore the default texture state
	m_TexImage->BindToZero();
}

void ImageNode::initTexture()
{
	m_TexImage->Delete();
	m_TexImage->Create(m_ImageWidth, m_ImageHeight, GL_RGB8, GL_BGR_EXT, GL_UNSIGNED_BYTE);
}
