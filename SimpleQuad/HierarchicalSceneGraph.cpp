// SimpleQuad.cpp : 定义控制台应用程序的入口点。
//
#include <GL/glew.h>  //Must before freeglut.h
#include <GL/freeglut.h>
#include <stdio.h>

//OpenCV headers
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//Scene graph
#include "ImageNode.h"
#include "ColorBarNode.h"
#include "PolygonNode.h"
#include "CircleNode.h"
#include "AxisNode.h"

//Global variables
int g_WinWidth = 800;
int g_WinHeight = 768;
int g_ImageWidth = 640;
int g_ImageHeight = 512;

//Scene Graph 
BaseNode *g_SceneRootNode = NULL;
BaseNode *g_SelectedModel = NULL;
ImageNode *g_ImageNode = NULL;
RectNode *g_ROILinearNode = NULL;
CircleNode *g_ROIConvexNode = NULL;
AxisNode *g_ImageAxisNode = NULL;
AxisNode *g_ROILinearAxisNode = NULL;
AxisNode *g_ROIConvexAxisNode = NULL;
PolygonNode *g_PolygonNode = NULL;
ColorBarNode *g_BModeColorBarNode = NULL;
ColorBarNode *g_CModeColorBarNode = NULL;
ColorBarNode *g_MModeColorBarNode = NULL;

//Colormap related
int g_BModeColormapCount = 29;
int g_BModeColormapCurrentIndex = 0;
unsigned char **g_BModeColormaps = NULL;
int g_CModeColormapCount = 9;
int g_CModeColormapCurrentIndex = 0;
unsigned char **g_CModeColormaps = NULL;
int g_MModeColormapCount = 9;
int g_MModeColormapCurrentIndex = 0;
unsigned char **g_MModeColormaps = NULL;

//Mouse related
bool g_LeftButtonDown = false;
int g_OldMousePosX;
int g_OldMousePosY;

//Capture real-time video or Image sequences
cv::VideoCapture *g_Capture;

void initDataFile()
{
	//g_Capture = new cv::VideoCapture(0);  //0 means default camera
	g_Capture = new cv::VideoCapture("Data\\%04d.bmp");
	if (!g_Capture->isOpened())
		printf("Can't open default camera!");

	g_ImageWidth = (int)g_Capture->get(CV_CAP_PROP_FRAME_WIDTH);
	g_ImageHeight = (int)g_Capture->get(CV_CAP_PROP_FRAME_HEIGHT);
	printf("Video Width: %d, Height: %d\n", g_ImageWidth, g_ImageHeight);

	//Set these properties to image node	
	g_ImageNode->SetImageSize(g_ImageWidth, g_ImageHeight);
}

void initColormap(void)
{
	//Init memory for BMode colormap
	g_BModeColormaps = new unsigned char*[g_BModeColormapCount];
	for (int i = 0; i < g_BModeColormapCount; i++)
		g_BModeColormaps[i] = new unsigned char[256 * 3];  //RGB format
	//Init memory for CMode colormap
	g_CModeColormaps = new unsigned char*[g_CModeColormapCount];
	for (int i = 0; i < g_CModeColormapCount; i++)
		g_CModeColormaps[i] = new unsigned char[256 * 3];  //RGB format
	//Init memory for MMode colormap
	g_MModeColormaps = new unsigned char*[g_MModeColormapCount];
	for (int i = 0; i < g_MModeColormapCount; i++)
		g_MModeColormaps[i] = new unsigned char[256 * 3];  //RGB format

	//Read the colormap from file
	FILE *fp = NULL;
	char fileName[100];
	int r, g, b;
	//Read BMode colormap files
	for (int i = 0; i < g_BModeColormapCount; i++)
	{
		sprintf(fileName, "ColorMap/bmap/ColorMap_Type%d.txt", i + 1);
		fp = fopen(fileName, "rt");

		for (int j = 0; j < 256; j++)
		{
			fscanf(fp, "%d %d %d\n", &r, &g, &b);
			g_BModeColormaps[i][j * 3 + 0] = (unsigned char)r;
			g_BModeColormaps[i][j * 3 + 1] = (unsigned char)g;
			g_BModeColormaps[i][j * 3 + 2] = (unsigned char)b;
		}

		fclose(fp);
	}
	//Read CMode colormap files
	for (int i = 0; i < g_CModeColormapCount; i++)
	{
		sprintf(fileName, "ColorMap/cmap/ColorMap_Type%d.txt", i + 1);
		fp = fopen(fileName, "rt");

		for (int j = 0; j < 256; j++)
		{
			fscanf(fp, "%d %d %d\n", &r, &g, &b);
			g_CModeColormaps[i][j * 3 + 0] = (unsigned char)r;
			g_CModeColormaps[i][j * 3 + 1] = (unsigned char)g;
			g_CModeColormaps[i][j * 3 + 2] = (unsigned char)b;
		}

		fclose(fp);
	}
	//Read MMode colormap files
	for (int i = 0; i < g_MModeColormapCount; i++)
	{
		sprintf(fileName, "ColorMap/mmap/ColorMap_Type%d.txt", i + 1);
		fp = fopen(fileName, "rt");

		for (int j = 0; j < 256; j++)
		{
			fscanf(fp, "%d %d %d\n", &r, &g, &b);
			g_MModeColormaps[i][j * 3 + 0] = (unsigned char)r;
			g_MModeColormaps[i][j * 3 + 1] = (unsigned char)g;
			g_MModeColormaps[i][j * 3 + 2] = (unsigned char)b;
		}

		fclose(fp);
	}

	//Set palette data (Colormap) to each node
	g_BModeColorBarNode->SetPalette(g_BModeColormaps[g_BModeColormapCurrentIndex]);
	g_CModeColorBarNode->SetPalette(g_CModeColormaps[g_CModeColormapCurrentIndex]);
	g_MModeColorBarNode->SetPalette(g_MModeColormaps[g_MModeColormapCurrentIndex]);
}

void initScene()
{
	//Root node
	g_SceneRootNode = new BaseNode;

	//Simulated Ultrasound Image Region
	g_ImageNode = new ImageNode;
	g_ImageNode->SetPosition(g_ImageWidth*0.5f, g_ImageHeight*0.5f);
	g_ImageNode->SetSize((float)g_ImageWidth, (float)g_ImageHeight);
	//Add to scene graph
	g_SceneRootNode->AddChildNode(g_ImageNode);

	//Image Local Axis for test or debug
	g_ImageAxisNode = new AxisNode;
	g_ImageAxisNode->SetColor(1.0f, 0.0f, 0.0f);
	//Add to scene graph
	g_ImageNode->AddChildNode(g_ImageAxisNode);

	//Simulated Ultrasound ROI for Linear Probe
	g_ROILinearNode = new RectNode;
	g_ROILinearNode->SetColor(1.0f, 1.0f, 0.0f);
	g_ROILinearNode->SetDrawMode(BaseNode::LINE);
	g_ROILinearNode->SetPosition(0.1f, 0.2f);
	g_ROILinearNode->SetSize(0.2f, 0.4f);
	//Add to scene graph
	g_ImageNode->AddChildNode(g_ROILinearNode);

	//Linear ROI Local Axis for test or debug
	g_ROILinearAxisNode = new AxisNode;
	g_ROILinearAxisNode->SetColor(1.0f, 1.0f, 0.0f);
	//Add to scene graph
	g_ROILinearNode->AddChildNode(g_ROILinearAxisNode);

	//Simulated Ultrasound ROI for Convex Probe
	g_ROIConvexNode = new CircleNode;
	g_ROIConvexNode->SetColor(0.0f, 0.0f, 1.0f);
	g_ROIConvexNode->SetDrawMode(BaseNode::LINE);
	g_ROIConvexNode->SetPosition(0.0f, 0.1f);
	g_ROIConvexNode->SetBeginAngle(240.0f);
	g_ROIConvexNode->SetEndAngle(300.0f);
	g_ROIConvexNode->SetInnerRadius(0.1f);
	//g_ROIConvexNode->SetSize(0.2f, 0.2f);
	//Add to scene graph
	g_ImageNode->AddChildNode(g_ROIConvexNode);

	//Convex ROI Local Axis for test or debug
	g_ROIConvexAxisNode = new AxisNode;
	g_ROIConvexAxisNode->SetColor(0.0f, 0.0f, 1.0f);
	//Add to scene graph
	g_ROIConvexNode->AddChildNode(g_ROIConvexAxisNode);

	//Simulated Ultrasound measurement item
	g_PolygonNode = new PolygonNode;
	g_PolygonNode->SetColor(0.0f, 1.0f, 0.0f);
	g_PolygonNode->SetDrawMode(BaseNode::LINE);
	//Add to scene graph
	g_ImageNode->AddChildNode(g_PolygonNode);

	//Simulated Ultrasound Color Bar for BMode
	g_BModeColorBarNode = new ColorBarNode;
	g_BModeColorBarNode->SetPosition(g_ImageWidth*0.5f + 300, g_ImageHeight*0.5f + 180);
	g_BModeColorBarNode->SetSize(20.0f, 128.0f);
	//Add to scene graph
	g_SceneRootNode->AddChildNode(g_BModeColorBarNode);

	//Simulated Ultrasound Color Bar for CMode
	g_CModeColorBarNode = new ColorBarNode;
	g_CModeColorBarNode->SetPosition(g_ImageWidth*0.5f + 326, g_ImageHeight*0.5f + 180);
	g_CModeColorBarNode->SetSize(20.0f, 128.0f);
	//Add to scene graph
	g_SceneRootNode->AddChildNode(g_CModeColorBarNode);

	//Simulated Ultrasound Color Bar for MMode
	g_MModeColorBarNode = new ColorBarNode;
	g_MModeColorBarNode->SetPosition(g_ImageWidth*0.5f + 326, g_ImageHeight*0.5f + 40);
	g_MModeColorBarNode->SetSize(20.0f, 128.0f);
	//Add to scene graph
	g_SceneRootNode->AddChildNode(g_MModeColorBarNode);

	g_SelectedModel = g_ImageNode;
}

void init(void)
{
	//Init glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		printf("Error: %s\n", glewGetErrorString(err));
		exit(1);
	}

	//Init OpenGL states.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_1D);

	//Init scene graph
	initScene();

	//Init ultrasound image data file. Depend on image node, must after initScene
	initDataFile();

	//Init colormap. Depend on colormap node, must after initScene
	initColormap();
}

void close()
{
	//Release the resources used by program	
	delete[]g_SceneRootNode;

	for (int i = 0; i < g_BModeColormapCount; i++)
		delete[]g_BModeColormaps[i];
	delete[]g_BModeColormaps;

	for (int i = 0; i < g_CModeColormapCount; i++)
		delete[]g_CModeColormaps[i];
	delete[]g_CModeColormaps;

	for (int i = 0; i < g_MModeColormapCount; i++)
		delete[]g_MModeColormaps[i];
	delete[]g_MModeColormaps;

	//Delete OpenCV video capturer
	delete g_Capture;
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			if (g_LeftButtonDown == false)
				g_PolygonNode->ClearAllPoints();

			g_LeftButtonDown = true;
			//Get object space coordinate
			Point2f pt;
			g_ImageNode->ScreenSpaceToObjectSpace((float)x, (float)(g_WinHeight - y), pt.x, pt.y);
			g_PolygonNode->AddPoint(pt);
			g_PolygonNode->AddPoint(pt);
			//g_OldMousePosX = x;
			//g_OldMousePosY = y;
			glutPostRedisplay();
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN && g_LeftButtonDown == true)
		{
			Point2f pt;
			g_ImageNode->ScreenSpaceToObjectSpace((float)x, (float)(g_WinHeight - y), pt.x, pt.y);
			g_PolygonNode->RemoveLastPoint();
			g_PolygonNode->AddPoint(pt);
			g_LeftButtonDown = false;
			glutPostRedisplay();
		}
	}
}

void dumpInfo(void)
{
	//Print OpenGL info
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//Print glew info
	printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void timer(int value)
{
	if (g_SceneRootNode == NULL)
	{
		//Set the next timer event
		glutTimerFunc(30, timer, 10);
		return;
	}

	cv::Mat oneFrame;
	if (!g_Capture->read(oneFrame))
	{
		//If reach the end of video, loop from start
		printf("End of video, reseting the position...\n");
		g_Capture->set(CV_CAP_PROP_POS_FRAMES, 0);
		//Re-read the first frame
		g_Capture->read(oneFrame);
	}

	//Update image node	
	g_ImageNode->SetImageData(oneFrame.data);

	//Refresh the display
	glutPostRedisplay();

	//Set the next timer event
	glutTimerFunc(30, timer, 10);
}

void drawScene()
{
	g_SceneRootNode->Draw();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Translate the image to center of window
	//glTranslatef((g_WinWidth - g_ImageWidth)*0.5f, (g_WinHeight - g_ImageHeight)*0.5f, 0.0f);

	drawScene();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	printf("w is %d, h is %d\n", w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1, 1);

	glViewport(0, 0, w, h);
	g_WinWidth = w;
	g_WinHeight = h;
}

void keyboard(unsigned char key, int x, int y)
{
	bool lrInvert, udInvert;
	float px, py;
	float objectX0, objectY0;
	float objectX1, objectY1;
	switch (key)
	{
	case 27:
		close();
		exit(0);
		break;

	case '1':
		g_SelectedModel = g_ImageNode;
		break;

	case '2':
		g_SelectedModel = g_ROILinearNode;
		break;

	case '3':
		g_SelectedModel = g_ROIConvexNode;
		break;

	case 'a':
	case 'A':
		g_SelectedModel->GetParentNode()->ScreenSpaceToObjectSpace(0.0f, 0.0f, objectX0, objectY0);
		g_SelectedModel->GetParentNode()->ScreenSpaceToObjectSpace(-1.0f, 0.0f, objectX1, objectY1);
		g_SelectedModel->GetPosition(px, py);
		px += (objectX1 - objectX0);
		py += (objectY1 - objectY0);
		g_SelectedModel->SetPosition(px, py);
		break;

	case 'd':
	case 'D':
		g_SelectedModel->GetParentNode()->ScreenSpaceToObjectSpace(0.0f, 0.0f, objectX0, objectY0);
		g_SelectedModel->GetParentNode()->ScreenSpaceToObjectSpace(1.0f, 0.0f, objectX1, objectY1);
		g_SelectedModel->GetPosition(px, py);
		px += (objectX1 - objectX0);
		py += (objectY1 - objectY0);
		g_SelectedModel->SetPosition(px, py);
		break;

	case 's':
	case 'S':
		g_SelectedModel->GetParentNode()->ScreenSpaceToObjectSpace(0.0f, 0.0f, objectX0, objectY0);
		g_SelectedModel->GetParentNode()->ScreenSpaceToObjectSpace(0.0f, -1.0f, objectX1, objectY1);
		g_SelectedModel->GetPosition(px, py);
		px += (objectX1 - objectX0);
		py += (objectY1 - objectY0);
		g_SelectedModel->SetPosition(px, py);
		break;

	case 'w':
	case 'W':
		g_SelectedModel->GetParentNode()->ScreenSpaceToObjectSpace(0.0f, 0.0f, objectX0, objectY0);
		g_SelectedModel->GetParentNode()->ScreenSpaceToObjectSpace(0.0f, 1.0f, objectX1, objectY1);
		g_SelectedModel->GetPosition(px, py);
		px += (objectX1 - objectX0);
		py += (objectY1 - objectY0);
		g_SelectedModel->SetPosition(px, py);
		break;

	case 'l':
	case 'L':
		g_SelectedModel->GetLRInvert(lrInvert);
		g_SelectedModel->SetLRInvert(!lrInvert);
		break;

	case 'u':
	case 'U':
		g_SelectedModel->GetUDInvert(udInvert);
		g_SelectedModel->SetUDInvert(!udInvert);
		break;

	case 'b':
	case 'B':
		g_BModeColormapCurrentIndex++;
		if (g_BModeColormapCurrentIndex >= g_BModeColormapCount)
			g_BModeColormapCurrentIndex = 0;
		g_BModeColorBarNode->SetPalette(g_BModeColormaps[g_BModeColormapCurrentIndex]);
		break;

	case 'c':
	case 'C':
		g_CModeColormapCurrentIndex++;
		if (g_CModeColormapCurrentIndex >= g_CModeColormapCount)
			g_CModeColormapCurrentIndex = 0;
		g_CModeColorBarNode->SetPalette(g_CModeColormaps[g_CModeColormapCurrentIndex]);
		break;

	case 'm':
	case 'M':
		g_MModeColormapCurrentIndex++;
		if (g_MModeColormapCurrentIndex >= g_MModeColormapCount)
			g_MModeColormapCurrentIndex = 0;
		g_MModeColorBarNode->SetPalette(g_MModeColormaps[g_MModeColormapCurrentIndex]);
		break;
	case 'e':
	case 'E':
		double xS = g_WinWidth*1.0f / g_ImageWidth;
		double yS = g_WinHeight*1.0f / g_ImageHeight;
		g_SceneRootNode->SetSize(xS, yS);
		break;
	}

	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	if (g_SelectedModel == NULL || g_SelectedModel->GetParentNode() == NULL)
		return;

	float px, py, angle, steerAngle, sizex, sizey;
	float objectX0, objectY0;
	float objectX1, objectY1;
	float deltaPixelX = 1.0f;
	float deltaPixelY = 1.0f;
	g_SelectedModel->GetParentNode()->ScreenSpaceToObjectSpace(0.0f, 0.0f, objectX0, objectY0);
	g_SelectedModel->GetParentNode()->ScreenSpaceToObjectSpace(deltaPixelX, deltaPixelY, objectX1, objectY1);
	deltaPixelX = fabsf(objectX1 - objectX0);
	deltaPixelY = fabsf(objectY1 - objectY0);
	deltaPixelX = deltaPixelX > deltaPixelY ? deltaPixelX : deltaPixelY;
	switch (key)
	{
	case GLUT_KEY_LEFT:
		g_SelectedModel->GetPosition(px, py);
		px -= deltaPixelX;
		g_SelectedModel->SetPosition(px, py);
		break;

	case GLUT_KEY_RIGHT:
		g_SelectedModel->GetPosition(px, py);
		px += deltaPixelX;
		g_SelectedModel->SetPosition(px, py);
		break;

	case GLUT_KEY_UP:
		g_SelectedModel->GetPosition(px, py);
		py += deltaPixelX;
		g_SelectedModel->SetPosition(px, py);
		break;

	case GLUT_KEY_DOWN:
		g_SelectedModel->GetPosition(px, py);
		py -= deltaPixelX;
		g_SelectedModel->SetPosition(px, py);
		break;

	case GLUT_KEY_END:
		g_SelectedModel->GetAngle(angle);
		angle -= 1.0f;
		g_SelectedModel->SetAngle(angle);
		break;

	case GLUT_KEY_HOME:
		g_SelectedModel->GetAngle(angle);
		angle += 1.0f;
		g_SelectedModel->SetAngle(angle);
		break;

	case GLUT_KEY_PAGE_DOWN:
		g_SelectedModel->GetSize(sizex, sizey);
		sizex -= deltaPixelX;
		sizey -= deltaPixelX;
		g_SelectedModel->SetSize(sizex, sizey);
		break;

	case GLUT_KEY_PAGE_UP:
		g_SelectedModel->GetSize(sizex, sizey);
		sizex += deltaPixelX;
		sizey += deltaPixelX;
		g_SelectedModel->SetSize(sizex, sizey);
		break;

	case GLUT_KEY_F1:
		if (RectNode *rectNode = dynamic_cast<RectNode*>(g_SelectedModel))
		{
			rectNode->GetSteerAngle(steerAngle);
			steerAngle -= 1.0f;
			rectNode->SetSteerAngle(steerAngle);
		}
		break;

	case GLUT_KEY_F2:
		if (RectNode *rectNode = dynamic_cast<RectNode*>(g_SelectedModel))
		{
			rectNode->GetSteerAngle(steerAngle);
			steerAngle += 1.0f;
			rectNode->SetSteerAngle(steerAngle);
		}
		break;

	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(g_WinWidth, g_WinHeight);
	glutInitWindowPosition(100, 0);
	glutCreateWindow(argv[0]);

	init();
	dumpInfo();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutTimerFunc(30, timer, 10);
	glutMainLoop();
	return 0;
}