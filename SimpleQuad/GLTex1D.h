#pragma once
class GLTex1D
{
private:
	unsigned int m_TexID;
	unsigned int m_TextureUnit;

	int m_TexSize;
	int m_TexFormat;
	int m_TexType;

public:
	GLTex1D(void);
	~GLTex1D(void);

	//Get the ID of texture object
	int GetID()
	{
		return m_TexID;
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
	void Create(int texSize);
	void Create(int texSize, void* texData);
	void Create(int texSize, int texInternalFormat, int texFormat, int texType);
	void Create(int texSize, int texInternalFormat, int texFormat, int texType, void* texData);
	void Create(int texSize, int calmpMode, int minFilter, int magFilter, int texInternalFormat, int texFormat, int texType);
	void Create(int texSize, int calmpMode, int minFilter, int magFilter, int texInternalFormat, int texFormat, int texType, void* texData);

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

};

