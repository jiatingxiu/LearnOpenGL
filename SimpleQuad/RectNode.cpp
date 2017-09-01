#include "RectNode.h"

#include <math.h>
#include <GL/glew.h>


RectNode::RectNode(void) : m_SteerAngle(0.0f)
{
	
}


RectNode::~RectNode(void)
{
	
}

void RectNode::SetSteerAngle(float sa)
{
	m_SteerAngle = sa;
}

void RectNode::GetSteerAngle(float& sa)
{
	sa = m_SteerAngle;
}

void RectNode::transformSelf()
{
	//Calculate steer angle matrix
	float steerAngleInRadian = m_SteerAngle*3.1415926f / 180.0f;
	float sinAngle = sinf(steerAngleInRadian);
	float cosAngle = cosf(steerAngleInRadian);
	//OpenGL matrix is 4*4 matrix. It's memory layout is column order.
	//Column 1's index: 0,1,2,3, Column 2's index: 4,5,6,7
	//Column 3's index: 8,9,10,11, Column 4's index: 12,13,14,15
	float steerMatrix[16];
	//First column
	steerMatrix[0] = 1.0f;
	steerMatrix[1] = 0.0f;
	steerMatrix[2] = 0.0f;
	steerMatrix[3] = 0.0f;
	//Second column
	steerMatrix[4] = sinAngle;
	steerMatrix[5] = cosAngle;
	steerMatrix[6] = 0.0f;
	steerMatrix[7] = 0.0f;
	//Third column
	steerMatrix[8] = 0.0f;
	steerMatrix[9] = 0.0f;
	steerMatrix[10] = 1.0f;
	steerMatrix[11] = 0.0f;
	//Last column
	steerMatrix[12] = -m_SizeY*0.5f*sinAngle;
	steerMatrix[13] = m_SizeY*0.5f*(1.0f - cosAngle);
	steerMatrix[14] = 0.0f;
	steerMatrix[15] = 1.0f;

	glTranslatef(m_PositionX, m_PositionY, 0.0f);
	glRotatef(m_Angle, 0.0f, 0.0f, 1.0f);
	glMultMatrixf(steerMatrix);
	glScalef(m_LRInvert ? -m_SizeX : m_SizeX, m_UDInvert ? -m_SizeY : m_SizeY, 1.0f);
}

void RectNode::drawSelf()
{
	//Draw a Square centered in origin and unit length
	glBegin(GL_QUADS);
	{
		glColor3f(m_ColorR, m_ColorG, m_ColorB);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
	}
	glEnd();
}
