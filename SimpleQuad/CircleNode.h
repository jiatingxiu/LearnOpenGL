#pragma once

#include "BaseNode.h"

//Concrete class for 2D Circle
class CircleNode : public BaseNode
{
	//OutterRadius is always 0.5
	float m_InnerRadius;
	float m_BeginAngle;
	float m_EndAngle;
public:
	CircleNode(void);
	virtual ~CircleNode(void);

	void SetInnerRadius(float radius);
	void GetInnerRadius(float &radius);

	void SetBeginAngle(float angleInDegree);
	void GetBeginAngle(float &angleInDegree);

	void SetEndAngle(float angleInDegree);
	void GetEndAngle(float &angleInDegree);
	
protected:		
	virtual void drawSelf();
};

