#include <math.h>
#include <GL/glew.h>
#include "ColorBarNode.h"
#include "GLTex1D.h"


ColorBarNode::ColorBarNode(void)
{
	//Initialize Textures
	m_TexPalette = new GLTex1D();
	m_TexPalette->SetTexUnit(0);
	initTexture();
}


ColorBarNode::~ColorBarNode(void)
{
	//Delete textures
	delete m_TexPalette;
}

void ColorBarNode::SetPalette(unsigned char *paletteRGB)
{
	//Update texture
	m_TexPalette->ActiveAndBind();
	m_TexPalette->Update(paletteRGB);
	m_TexPalette->BindToZero();
}

void ColorBarNode::drawSelf()
{
	//OpenGL doesn't allow enabling 2D and 1D Texture at the same time
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_1D);
	//Bind the texture object
	m_TexPalette->ActiveAndBind();
	//Draw a textured Square centered in origin and unit length
	glBegin(GL_QUADS);
	{
		glColor3f(m_ColorR, m_ColorG, m_ColorB);
		glTexCoord1f(1.0f);
		glVertex2f(0.5f, 0.5f);
		glTexCoord1f(1.0f);
		glVertex2f(-0.5f, 0.5f);
		glTexCoord1f(0.0f);
		glVertex2f(-0.5f, -0.5f);
		glTexCoord1f(0.0f);
		glVertex2f(0.5f, -0.5f);
	}
	glEnd();
	//Restore the default texture state
	m_TexPalette->BindToZero();
}

void ColorBarNode::initTexture()
{
	m_TexPalette->Delete();
	m_TexPalette->Create(256, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE);
}
