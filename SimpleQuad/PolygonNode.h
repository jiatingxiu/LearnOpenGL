#pragma once

#include "BaseNode.h"
#include <vector>

struct Point2f
{
	float x;
	float y;
};
//Concrete class for 2D Rectangle
class PolygonNode : public BaseNode
{
	std::vector<Point2f> m_Points;
public:
	PolygonNode(void);
	virtual ~PolygonNode(void);

	void AddPoint(const Point2f &point);
	void RemoveLastPoint();
	void ClearAllPoints();
	
	
protected:	
	virtual void drawSelf();
};

