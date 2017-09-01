#include <math.h>
#include <GL/glew.h>
#include "CircleNode.h"


CircleNode::CircleNode(void) : m_InnerRadius(0.0f), m_BeginAngle(0.0f), m_EndAngle(360.0f)
{
	
}


CircleNode::~CircleNode(void)
{
	
}

void CircleNode::SetInnerRadius(float radius)
{
	if (radius < 0.0f)
		radius = 0.0f;
	if (radius > 0.5f)
		radius = 0.5f;
	m_InnerRadius = radius;
}

void CircleNode::GetInnerRadius(float & radius)
{
	radius = m_InnerRadius;
}

void CircleNode::SetBeginAngle(float angleInDegree)
{
	m_BeginAngle = angleInDegree;
}

void CircleNode::GetBeginAngle(float & angleInDegree)
{
	angleInDegree = m_BeginAngle;
}

void CircleNode::SetEndAngle(float angleInDegree)
{
	m_EndAngle = angleInDegree;
}

void CircleNode::GetEndAngle(float & angleInDegree)
{
	angleInDegree = m_EndAngle;
}

void CircleNode::drawSelf()
{
	if (m_DrawMode == FILL)
	{
		//Draw a QUAD STRIP related to parent's coordinate space
		glBegin(GL_QUAD_STRIP);
		{
			glColor3f(m_ColorR, m_ColorG, m_ColorB);
			//Draw an arc, 2 points per degree
			int totalPoints = (int)(2.0f*(m_EndAngle - m_BeginAngle));
			float angleStep = (m_EndAngle - m_BeginAngle) / totalPoints;  // = 0.5
			for (int i = 0; i <= totalPoints; i++)
			{
				//From degree to radian
				float currentAngle = (m_BeginAngle + i*angleStep)*3.1415926f / 180.0f;
				float cosAngle = cosf(currentAngle);
				float sinAngle = sinf(currentAngle);
				glVertex2f(m_InnerRadius*cosAngle, m_InnerRadius*sinAngle);
				glVertex2f(0.5f*cosAngle, 0.5f*sinAngle);
			}

		}
		glEnd();
	}
	else
	{
		//Draw a LINE LOOP related to parent's coordinate space
		glBegin(GL_LINE_LOOP);
		{
			glColor3f(m_ColorR, m_ColorG, m_ColorB);
			//Draw an arc, 2 points per degree
			int totalPoints = (int)(2.0f*(m_EndAngle - m_BeginAngle));
			float angleStep = (m_EndAngle - m_BeginAngle) / totalPoints;  // = 0.5
			float *outterPointsX = new float[totalPoints + 1];
			float *outterPointsY = new float[totalPoints + 1];
			for (int i = 0; i <= totalPoints; i++)
			{
				//From degree to radian
				float currentAngle = (m_BeginAngle + i*angleStep)*3.1415926f / 180.0f;
				float cosAngle = cosf(currentAngle);
				float sinAngle = sinf(currentAngle);
				glVertex2f(m_InnerRadius*cosAngle, m_InnerRadius*sinAngle);
				outterPointsX[i] = 0.5f*cosAngle;
				outterPointsY[i] = 0.5f*sinAngle;				
			}
			for (int i = totalPoints; i >= 0; i--)
			{
				glVertex2f(outterPointsX[i], outterPointsY[i]);				 
			}

		}
		glEnd();
	}
	
}