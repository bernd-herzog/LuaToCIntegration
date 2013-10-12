#include "stdafx.h"
#include "Game.h"
#include "CppWrapper.h"


MakeWrapper(DisplayWrapper, (), (), Game, void);
MakeWrapper(ReshapeWrapper, (int w, int h), (w,h), Game, void);
MakeWrapper(KeyboardWrapper, (unsigned char c, int p1, int p2), (c, p1, p2), Game, void);
MakeWrapper(MouseWrapper, (int button, int state, int x, int y), (button, state, x, y), Game, void);


Game::Game(void)
{

}


Game::~Game(void)
{
}


void Game::Init()
{
	int i = 0;
	glutInit(&i, NULL);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(800, 600);
	glutCreateWindow("LUA");

	glutDisplayFunc(GetWrapper(DisplayWrapper, &Game::glutDisplay));
	glutReshapeFunc(GetWrapper(ReshapeWrapper, &Game::glutReshape));
	glutKeyboardFunc(GetWrapper(KeyboardWrapper, &Game::glutKeyboard));
	glutMouseFunc(GetWrapper(MouseWrapper, &Game::glutMouse));


	m_scripting.Init();

}

void Game::MainLoop()
{
	glutMainLoop();
}


void Game::glutDisplay()
{
	m_scripting.RunScripts();

	//init gl for 3d
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(g_3dMatrix);
	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(
		0.0f, 0.0f, 1.0f, //eye
		0.0f, 0.0f, 0.0f, //center
		0.0f, 1.0f, 0.0f);//up

	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

	glBegin(GL_TRIANGLES);
	{
		glVertex3f(0.0f, 0.0f, -2.0f);
		glVertex3f(1.0f, 0.0f, -2.0f);
		glVertex3f(0.0f, 1.0f, -2.0f);
	}
	glEnd();

	//init gl for HUD
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(g_HudMatrix);
	glMatrixMode(GL_MODELVIEW);

	m_scripting.RenderHUD();

	glutSwapBuffers();
	glutPostRedisplay();

}

void Game::glutReshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	//3dMatrix
	glLoadIdentity();
	gluPerspective(65.0, (float)w / h, 1.0f, 10.0f);

	glGetFloatv(GL_PROJECTION_MATRIX, g_3dMatrix);
	//hud matrix
	glLoadIdentity();
	glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

	glGetFloatv(GL_PROJECTION_MATRIX, g_HudMatrix);

	glMatrixMode(GL_MODELVIEW);

}

void Game::glutKeyboard(unsigned char c, int p1, int p2)
{
	if (c == 'f')
	{
		glutFullScreenToggle();
	}
	if (c == 'e')
	{
		exit(0);
	}
}

void Game::glutMouse(int button, int state, int x, int y)
{

}
