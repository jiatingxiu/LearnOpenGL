#pragma once
#include <vector>

//This node is a concrete class. It can be used as a pure transformation node.
class BaseNode
{
public:
	enum DrawMode
	{
		LINE,
		FILL,
		POINTS
	};
protected:
	//Node position in Parent's Coordinate Space
	float m_PositionX;
	float m_PositionY;
	//Node size
	float m_SizeX;
	float m_SizeY;
	//Node orientation
	float m_Angle;
	//Node color
	float m_ColorR;
	float m_ColorG;
	float m_ColorB;
	//LeftRight Invert
	bool m_LRInvert;
	//UpDown Invert
	bool m_UDInvert;
	DrawMode m_DrawMode;

	//ModelView Matrix
	double m_ModelViewMatrix[16];
	//Projection Matrix
	double m_ProjectionMatrix[16];
	//Viewport
	int m_Viewport[4];

	std::vector<BaseNode*> m_ChildNodes;
	BaseNode *m_ParentNode;

public:
	BaseNode(void);
	virtual ~BaseNode(void);

	void SetPosition(float px, float py);
	void GetPosition(float& px, float& py);

	void SetSize(float sx, float sy);
	void GetSize(float& sx, float& sy);

	void SetAngle(float angleInDegree);
	void GetAngle(float& angleInDegree);

	void SetLRInvert(bool lrInvert);
	void GetLRInvert(bool& lrInvert);

	void SetUDInvert(bool udInvert);
	void GetUDInvert(bool& udInvert);

	void SetColor(float r, float g, float b);
	void GetColor(float& r, float& g, float& b);

	void AddChildNode(BaseNode *child);
	void RemoveChildNode(BaseNode *child);
	void ClearAllChildNodes();
	void SetParentNode(BaseNode *parentNode);
	BaseNode* GetParentNode();

	void SetDrawMode(DrawMode mode);

	void ScreenSpaceToObjectSpace(float screenX, float screenY, float &objectX, float &objectY);
	void ObjectSpaceToScreenSpace(float objectX, float objectY, float &screenX, float &screenY);

	void Draw();

protected:
	virtual void transformSelf();
	virtual void drawSelf();

};

