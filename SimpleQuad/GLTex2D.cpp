#include "GLTex2D.h"
#include <GL/glew.h>


GLTex2D::GLTex2D(void)
{
	m_TexID = 0;
	m_TextureUnit = 0;
	m_MipmapLevels = 1;
}


GLTex2D::~GLTex2D(void)
{
	Delete();
}


void GLTex2D::Create(int texWidth, int texHeight)
{
	Create(texWidth, texHeight, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 0);
}

void GLTex2D::Create(int texWidth, int texHeight, void* texData)
{
	Create(texWidth, texHeight, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, texData); 
}

void GLTex2D::Create(int texWidth, int texHeight, int texInternalFormat, int texFormat, int texType)
{
	Create(texWidth, texHeight, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, texInternalFormat, texFormat, texType, 0);
}

void GLTex2D::Create(int texWidth, int texHeight, int texInternalFormat, int texFormat, int texType, void* texData)
{
	Create(texWidth, texHeight, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, texInternalFormat, texFormat, texType, texData);
}

void GLTex2D::Create(int texWidth, int texHeight, int clampModeS, int clampModeT, int minFilter, int magFilter, int texInternalFormat, int texFormat, int texType)
{
	Create(texWidth, texHeight, clampModeS, clampModeT, minFilter, magFilter, texInternalFormat, texFormat, texType, 0);
}

void GLTex2D::Create(int texWidth, int texHeight, int clampModeS, int clampModeT, int minFilter, int magFilter, int texInternalFormat, int texFormat, int texType, void* texData)
{
	//Generate name or ID of texture object
	glGenTextures(1, &m_TexID);

	//Bind this texture object
	glBindTexture(GL_TEXTURE_2D, m_TexID);

	//Clamp mode setting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampModeS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampModeT);

	//Filter setting for min and mag
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

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
	glTexImage2D(GL_TEXTURE_2D, 0, texInternalFormat, texWidth, texHeight, 0, texFormat, texType, texData); 
	/*glTexStorage2D(GL_TEXTURE_2D, 1, texInternalFormat, texWidth, texHeight); 
	if(texData)
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texWidth, texHeight, texFormat, texType, texData);*/
	
	//These parameters are needed in Update() function
	m_TexWidth = texWidth;
	m_TexHeight = texHeight;
	m_TexFormat = texFormat;
	m_TexType = texType;
	m_MipmapLevels = 1;
}

void GLTex2D::CreateMipmap(int texWidth, int texHeight, int clampModeS, int clampModeT, int minFilter, int magFilter, int texInternalFormat, int texFormat, int texType, int mipmapLevels, void* baseTexData)
{
	//Generate name or ID of texture object
	glGenTextures(1, &m_TexID);

	//Bind this texture object
	glBindTexture(GL_TEXTURE_2D, m_TexID);

	//Clamp mode setting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampModeS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampModeT);

	//Filter setting for min and mag
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	//GL_TEXTURE_ENV_MODE defaults to GL_MODULATE. We need to modify it to GL_REPLACE.
	//Note: This function is deprecated.
	//States such as GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_MAG_FILTER,
	//GL_TEXTURE_MIN_FILTER are part of the texture object.
	//glTexEnv is part of the texture image unit (TIU), not the texture object.
	//When you set this it will effect any texture attached to the TIU and it only has
	//effect during rendering.
	//You can select a TIU with glActiveTexture(GL_TEXTURE0+i).
	//Also keep in mind that glTexEnvi has no effect when a fragment shader is bound.
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);            

	//Transfer the texture data to GPU memory. You can set the last parameter as null to 
	//setup the texture format only.
	glTexImage2D(GL_TEXTURE_2D, 0, texInternalFormat, texWidth, texHeight, 0, texFormat, texType, baseTexData);
	/*glTexStorage2D(GL_TEXTURE_2D, mipmapLevels, texInternalFormat, texWidth, texHeight); 
	if(baseTexData)
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texWidth, texHeight, texFormat, texType, baseTexData);		
*/

	//These parameters are needed in Update() function
	m_TexWidth = texWidth;
	m_TexHeight = texHeight;
	m_TexFormat = texFormat;
	m_TexType = texType;
	m_MipmapLevels = mipmapLevels;
}

//Delete this texture object
void GLTex2D::Delete()
{
	if (m_TexID != 0)
		glDeleteTextures(1, &m_TexID);                
}

//Bind this texture object
void GLTex2D::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_TexID);
}

//First active the texture unit, then bind the texture object
void GLTex2D::ActiveAndBind()
{
	glActiveTexture(GL_TEXTURE0 + m_TextureUnit); 
	glBindTexture(GL_TEXTURE_2D, m_TexID);		
}

//Unbind this texture object
void GLTex2D::BindToZero()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

//Update using PBO. Copy the contents from PBO buffer to texture object.
//Using offset instead of pointer.
void GLTex2D::Update()
{
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_TexWidth, m_TexHeight, m_TexFormat, m_TexType, 0);		
}

//Update the whole texture using TexSubImage
void GLTex2D::Update(void* texData)
{
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_TexWidth, m_TexHeight, m_TexFormat, m_TexType, texData);		
}


//#include <iostream>
//Read the whole texture to system memory
void GLTex2D::Read(void* imgData)
{
	/*glPixelStorei(GL_PACK_ALIGNMENT, 1);

	int texWidth, texHeight, texInternalFormat,	packAlignment;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &texInternalFormat);
	glGetIntegerv(GL_PACK_ALIGNMENT, &packAlignment);
	std::cout << "Tex Width: " << texWidth << ", Tex Height: " << texHeight << ", Tex Internal Format: " << texInternalFormat << ", mTexFormat: " << m_TexFormat << ", Pixel Pack Alignment: " << packAlignment << std::endl;*/

	glGetTexImage(GL_TEXTURE_2D, 0, m_TexFormat, m_TexType, imgData);

	//std::cout << "glGetError: " << glGetError() << std::endl; 
}

//Read the whole texture to PBO
void GLTex2D::Read()
{
	/*glPixelStorei(GL_PACK_ALIGNMENT, 1);

	int texWidth, texHeight, texInternalFormat,	packAlignment;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &texInternalFormat);
	glGetIntegerv(GL_PACK_ALIGNMENT, &packAlignment);
	std::cout << "Tex Width: " << texWidth << ", Tex Height: " << texHeight << ", Tex Internal Format: " << texInternalFormat << ", mTexFormat: " << m_TexFormat << ", Pixel Pack Alignment: " << packAlignment << std::endl;*/

	glGetTexImage(GL_TEXTURE_2D, 0, m_TexFormat, m_TexType, 0);

	//std::cout << "glGetError: " << glGetError() << std::endl; 
}