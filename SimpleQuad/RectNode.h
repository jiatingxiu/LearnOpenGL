#pragma once

#include "BaseNode.h"

//Concrete class for 2D Rectangle
class RectNode : public BaseNode
{
	float m_SteerAngle;

public:
	RectNode(void);
	virtual ~RectNode(void);
	
	void SetSteerAngle(float sa);
	void GetSteerAngle(float& sa);
	
protected:
	virtual void transformSelf();
	virtual void drawSelf();
};

