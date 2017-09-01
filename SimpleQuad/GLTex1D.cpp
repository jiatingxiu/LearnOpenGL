#include "GLTex1D.h"
#include <GL/glew.h>

GLTex1D::GLTex1D(void)
{
	m_TexID = 0;
	m_TextureUnit = 0;
}


GLTex1D::~GLTex1D(void)
{
	Delete();
}

void GLTex1D::Create(int texSize)
{
	Create(texSize, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 0);
}

void GLTex1D::Create(int texSize, void* texData)
{
	Create(texSize, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, texData); 
}

void GLTex1D::Create(int texSize, int texInternalFormat, int texFormat, int texType)
{
	Create(texSize, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, texInternalFormat, texFormat, texType, 0);
}

void GLTex1D::Create(int texSize, int texInternalFormat, int texFormat, int texType, void* texData)
{
	Create(texSize, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, texInternalFormat, texFormat, texType, texData);
}

void GLTex1D::Create(int texSize, int clampMode, int minFilter, int magFilter, int texInternalFormat, int texFormat, int texType)
{
	Create(texSize, clampMode, minFilter, magFilter, texInternalFormat, texFormat, texType, 0);
}

void GLTex1D::Create(int texSize, int clampMode, int minFilter, int magFilter, int texInternalFormat, int texFormat, int texType, void* texData)
{
	//Generate name or ID of texture object
	glGenTextures(1, &m_TexID);

	//Bind this texture object
	glBindTexture(GL_TEXTURE_1D, m_TexID);

	//Clamp mode setting
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, clampMode);

	//Filter setting for min and mag
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, magFilter);

	//GL_TEXTURE_ENV_MODE defaults to GL_MODULATE. We need to modify it to GL_REPLACE.
	//Note: This function is deprecated.
	//States such as GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MAG_FILTER,
	//GL_TEXTURE_MIN_FILTER are part of the texture object.
	//glTexEnv is part of the texture image unit (TIU), not the texture object.
	//When you set this it will effect any texture attached to the TIU and it only has
	//effect during rendering.
	//You can select a TIU with glActiveTexture(GL_TEXTURE0+i).
	//Also keep in mind that glTexEnvi has no effect when a fragment shader is bound.
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);            

	//Transfer the texture data to GPU memory. You can set the last parameter as null to 
	//setup the texture format only.
	glTexImage1D(GL_TEXTURE_1D, 0, texInternalFormat, texSize, 0, texFormat, texType, texData); 

	//These parameters are needed in Update() function
	m_TexSize = texSize;
	m_TexFormat = texFormat;
	m_TexType = texType;
}

//Delete this texture object
void GLTex1D::Delete()
{
	if (m_TexID != 0)
		glDeleteTextures(1, &m_TexID);                
}

//Bind this texture object
void GLTex1D::Bind()
{
	glBindTexture(GL_TEXTURE_1D, m_TexID);
}

//First active the texture unit, then bind the texture object
void GLTex1D::ActiveAndBind()
{
	glActiveTexture(GL_TEXTURE0 + m_TextureUnit); 
	glBindTexture(GL_TEXTURE_1D, m_TexID);		
}

//Unbind this texture object
void GLTex1D::BindToZero()
{
	glBindTexture(GL_TEXTURE_1D, 0);
}

//Update using PBO. Copy the contents from PBO buffer to texture object.
//Using offset instead of pointer.
void GLTex1D::Update()
{
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, m_TexSize, m_TexFormat, m_TexType, 0);		
}

//Update the whole texture using TexSubImage
void GLTex1D::Update(void* texData)
{
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, m_TexSize, m_TexFormat, m_TexType, texData);
}
