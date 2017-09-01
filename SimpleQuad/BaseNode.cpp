#include "BaseNode.h"
#include <GL/glew.h>

BaseNode::BaseNode(void) : m_PositionX(0.0f), m_PositionY(0.0f),
m_SizeX(1.0f), m_SizeY(1.0f), m_Angle(0.0f),
m_ColorR(0.0f), m_ColorG(0.0f), m_ColorB(0.0f),
m_LRInvert(false), m_UDInvert(false),
m_DrawMode(FILL), m_ParentNode(NULL)
{
}


BaseNode::~BaseNode(void)
{
	//Recursively delete all the child nodes
	this->ClearAllChildNodes();
}

void BaseNode::SetPosition(float px, float py)
{
	m_PositionX = px;
	m_PositionY = py;
}

void BaseNode::GetPosition(float& px, float& py)
{
	px = m_PositionX;
	py = m_PositionY;
}

void BaseNode::SetSize(float sx, float sy)
{
	m_SizeX = sx;
	m_SizeY = sy;
}

void BaseNode::GetSize(float& sx, float& sy)
{
	sx = m_SizeX;
	sy = m_SizeY;
}

void BaseNode::SetAngle(float angleInDegree)
{
	m_Angle = angleInDegree;
}

void BaseNode::GetAngle(float& angleInDegree)
{
	angleInDegree = m_Angle;
}

void BaseNode::SetLRInvert(bool lrInvert)
{
	m_LRInvert = lrInvert;
}

void BaseNode::GetLRInvert(bool& lrInvert)
{
	lrInvert = m_LRInvert;
}

void BaseNode::SetUDInvert(bool udInvert)
{
	m_UDInvert = udInvert;
}

void BaseNode::GetUDInvert(bool& udInvert)
{
	udInvert = m_UDInvert;
}

void BaseNode::SetColor(float r, float g, float b)
{
	m_ColorR = r;
	m_ColorG = g;
	m_ColorB = b;
}

void BaseNode::GetColor(float& r, float& g, float& b)
{
	r = m_ColorR;
	g = m_ColorG;
	b = m_ColorB;
}

void BaseNode::SetDrawMode(DrawMode mode)
{
	m_DrawMode = mode;
}

void BaseNode::ScreenSpaceToObjectSpace(float screenX, float screenY, float & objectX, float & objectY)
{
	double ox, oy, oz;
	gluUnProject(screenX, screenY, 0.0f, m_ModelViewMatrix, m_ProjectionMatrix, m_Viewport, &ox, &oy, &oz);
	objectX = (float)ox;
	objectY = (float)oy;
}

void BaseNode::ObjectSpaceToScreenSpace(float objectX, float objectY, float & screenX, float & screenY)
{
	double sx, sy, sz;
	gluProject(objectX, objectY, 0.0f, m_ModelViewMatrix, m_ProjectionMatrix, m_Viewport, &sx, &sy, &sz);
	screenX = (float)sx;
	screenY = (float)sy;
}

void BaseNode::Draw()
{
	//Save parent's modelview matrix
	glPushMatrix();

	//Process DrawMode
	if (m_DrawMode == FILL)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (m_DrawMode == LINE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	//First do transformation and draw self
	transformSelf();
	//Get current modelview and projection matrix, viewport
	glGetDoublev(GL_MODELVIEW_MATRIX, m_ModelViewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, m_ProjectionMatrix);
	glGetIntegerv(GL_VIEWPORT, m_Viewport);
	//Then draw self
	drawSelf();

	//Then travesal and draw each child node
	for (std::vector<BaseNode*>::iterator it = m_ChildNodes.begin(); it != m_ChildNodes.end(); ++it)
		(*it)->Draw();

	//Restore parent's modelview matrix
	glPopMatrix();
}

void BaseNode::transformSelf()
{
	glTranslatef(m_PositionX, m_PositionY, 0.0f);
	glRotatef(m_Angle, 0.0f, 0.0f, 1.0f);
	glScalef(m_LRInvert ? -m_SizeX : m_SizeX, m_UDInvert ? -m_SizeY : m_SizeY, 1.0f);
}

void BaseNode::drawSelf()
{
	//Draw nothing. This node can be used as pure transformation node.
}

void BaseNode::AddChildNode(BaseNode *child)
{
	m_ChildNodes.push_back(child);
	child->SetParentNode(this);
}

void BaseNode::RemoveChildNode(BaseNode *child)
{
	std::vector<BaseNode*>::iterator it = std::find(m_ChildNodes.begin(), m_ChildNodes.end(), child);
	if (it != m_ChildNodes.end())
	{
		(*it)->SetParentNode(NULL);
		m_ChildNodes.erase(it);
	}
}

void BaseNode::ClearAllChildNodes()
{
	//First delete each BaseNode
	for (std::vector<BaseNode*>::iterator it = m_ChildNodes.begin(); it != m_ChildNodes.end(); ++it)
		delete *it;
	//Then clear the pointers collection
	m_ChildNodes.clear();
}

BaseNode * BaseNode::GetParentNode()
{
	return m_ParentNode;
}

void BaseNode::SetParentNode(BaseNode *parentNode)
{
	m_ParentNode = parentNode;
}


