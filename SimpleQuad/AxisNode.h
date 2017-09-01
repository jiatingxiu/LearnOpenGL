#pragma once

#include "BaseNode.h"

//Concrete class for 2D Circle
class AxisNode : public BaseNode
{
	
public:
	AxisNode(void);
	virtual ~AxisNode(void);
	
protected:		
	virtual void drawSelf();
};

