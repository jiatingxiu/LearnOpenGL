#include <GL/glew.h>
#include "AxisNode.h"


AxisNode::AxisNode(void)
{
	
}


AxisNode::~AxisNode(void)
{
	
}

void AxisNode::drawSelf()
{
	//Draw three lines to represent x or y axis
	glBegin(GL_LINES);
	{
		glColor3f(m_ColorR, m_ColorG, m_ColorB);
		//Draw x axis		
		glVertex2f(-0.3f, 0.0f);
		glVertex2f(0.3f, 0.0f);
		glVertex2f(0.3f, 0.0f);
		glVertex2f(0.29f, 0.01f);
		glVertex2f(0.3f, 0.0f);
		glVertex2f(0.29f, -0.01f);
		
		//Dray y axis		
		glVertex2f(0.0f, -0.3f);
		glVertex2f(0.0f, 0.3f);
		glVertex2f(0.0f, 0.3f);
		glVertex2f(-0.01f, 0.29f);
		glVertex2f(0.0f, 0.3f);
		glVertex2f(0.01f, 0.29f);
	}	
	glEnd();
}