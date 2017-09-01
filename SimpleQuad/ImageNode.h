#pragma once

#include "RectNode.h"

//Forward declaration
class GLTex2D;

//Concrete class for 2D Image
class ImageNode : public RectNode
{
	int m_ImageWidth;
	int m_ImageHeight;
	bool m_ImageSizeModified;
	//OpenGL Textures
	GLTex2D* m_TexImage;
public:
	ImageNode(void);
	virtual ~ImageNode(void);
	
	void SetImageSize(int imageWidth, int imageHeight);
	void SetImageData(unsigned char *imageData);

protected:
	virtual void drawSelf();

private:
	void initTexture();

};

