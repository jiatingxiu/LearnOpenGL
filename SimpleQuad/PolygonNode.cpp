#include <math.h>
#include <GL/glew.h>
#include "PolygonNode.h"


PolygonNode::PolygonNode(void)
{
	
}


PolygonNode::~PolygonNode(void)
{
	
}

void PolygonNode::drawSelf()
{
	//Draw a polygon related to parent's coordinate space
	glBegin(GL_POLYGON);
	{
		glColor3f(m_ColorR, m_ColorG, m_ColorB);
		//Travesal and draw each point
		for (std::vector<Point2f>::iterator it = m_Points.begin(); it != m_Points.end(); ++it)
			glVertex2f((*it).x, (*it).y);
	}
	glEnd();
}

void PolygonNode::AddPoint(const Point2f & point)
{
	m_Points.push_back(point);
}

void PolygonNode::RemoveLastPoint()
{
	m_Points.pop_back();
}

void PolygonNode::ClearAllPoints()
{
	m_Points.clear();
}
