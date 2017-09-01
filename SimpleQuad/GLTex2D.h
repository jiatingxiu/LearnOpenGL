#pragma once
class GLTex2D
{
private:
	unsigned int m_TexID;
    unsigned int m_TextureUnit;

	int m_TexWidth;
	int m_TexHeight;
	int m_MipmapLevels;
	int m_TexFormat;
	int m_TexType;
    
public:
	GLTex2D(void);
	~GLTex2D(void);

	//Get the ID of texture object
	int GetID()
	{
		return m_TexID;
	}

	//Get the width of texture object
	int GetWidth()
	{
		return m_TexWidth;
	}

	//Get the height of texture object
	int GetHeight()
	{
		return m_TexHeight;
	}

	//Get the format of texture object
	int GetFormat()
	{
		return m_TexFormat;
	}

	//Get the type of texture object
	int GetType()
	{
		return m_TexType;
	}

	//Get the texture unit
    int GetTexUnit()
	{
		return m_TextureUnit;
	}

	//Set the texture unit
	void SetTexUnit(int texUnit)
	{
		m_TextureUnit = texUnit;
	}

	//Create the texture object using the default parameter.
	//Internal format --- RGB8, ClampToEdge, Linear
	void Create(int texWidth, int texHeight);
	void Create(int texWidth, int texHeight, void* texData);
	void Create(int texWidth, int texHeight, int texInternalFormat, int texFormat, int texType);
	void Create(int texWidth, int texHeight, int texInternalFormat, int texFormat, int texType, void* texData);
	void Create(int texWidth, int texHeight, int calmpModeS, int clampModeT, int minFilter, int magFilter, int texInternalFormat, int texFormat, int texType);
	void Create(int texWidth, int texHeight, int calmpModeS, int clampModeT, int minFilter, int magFilter, int texInternalFormat, int texFormat, int texType, void* texData);
	void CreateMipmap(int texWidth, int texHeight, int clampModeS, int clampModeT, int minFilter, int magFilter, int texInternalFormat, int texFormat, int texType, int mipmapLevels, void* baseTexData);

	//Delete this texture object
	void Delete();

	//Bind this texture object
	void Bind();
	//First active the texture unit, then bind the texture object
	void ActiveAndBind();
	//Unbind this texture object
	void BindToZero();

	//Update using PBO. Copy the contents from PBO buffer to texture object.
    //Using offset instead of pointer.
	void Update();
	//Update the whole texture using TexSubImage
	void Update(void* texData); 
       
	//Read the whole texture to system memory
	void Read(void* imgData);
	//Read the whole texture to PBO
	void Read();
};