#pragma once

#include "RectNode.h"

//Forward declaration
class GLTex1D;

//Concrete class for ColorBar
class ColorBarNode : public RectNode
{
	//OpenGL Textures
	GLTex1D *m_TexPalette;
public:
	ColorBarNode(void);
	virtual ~ColorBarNode(void);

	void SetPalette(unsigned char *paletteRGB);

protected:
	virtual void drawSelf();

private:
	void initTexture();
};

