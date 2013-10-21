#include "stdafx.h"
#include "Game.h"
#include "CppWrapper.h"
#include <assert.h>

MakeWrapper(DisplayWrapper, (), (), Game, void);
MakeWrapper(ReshapeWrapper, (int w, int h), (w, h), Game, void);
MakeWrapper(KeyboardWrapper, (unsigned char c, int p1, int p2), (c, p1, p2), Game, void);
MakeWrapper(MouseWrapper, (int button, int state, int x, int y), (button, state, x, y), Game, void);
MakeWrapper(MotionWrapper, (int x, int y), (x, y), Game, void);


Game::Game(void)
{
}



Game::~Game(void)
{
}

void Game::Scripting_OnSetGodMode(Scripting*, EventArgs<int> args)
{
	MessageBoxEx(0, L"", L"TEST", 0, 0);
}

void Game::Init()
{
	//	wglCreateContext()

	int i = 0;
	glutInit(&i, NULL);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(800, 600);
	glutCreateWindow("LUA");
	GLenum err = glewInit();


	const char *venstr = (const char*)glGetString(GL_VENDOR);
	const char *verstr = (const char*)glGetString(GL_VERSION);

	bool is2 = GLEW_VERSION_2_0;
	if (GLEW_OK != err || !is2)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		//		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

		const char *errstr = (const char*)glewGetErrorString(err);
		return;
	}


	glutDisplayFunc(GetWrapper(DisplayWrapper, &Game::glutDisplay));
	glutReshapeFunc(GetWrapper(ReshapeWrapper, &Game::glutReshape));
	glutKeyboardFunc(GetWrapper(KeyboardWrapper, &Game::glutKeyboard));
	glutMouseFunc(GetWrapper(MouseWrapper, &Game::glutMouse));
	glutMotionFunc(GetWrapper(MotionWrapper, &Game::glutMotion));

	
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	assert(glGetError() == GL_NO_ERROR);

	const GLchar *fragmentSource = "void main(void){gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);}";
	const GLchar *vertexSource = "void main(void){gl_Position = ftransform();}";

	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	assert(glGetError() == GL_NO_ERROR);

	glCompileShader(fragmentShader);
	glCompileShader(vertexShader);
	assert(glGetError() == GL_NO_ERROR);

	GLint status;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE){
		return;
	}

	GLuint programm = glCreateProgram();
	assert(glGetError() == GL_NO_ERROR);

	glAttachShader(programm, fragmentShader);
	glAttachShader(programm, vertexShader);
	assert(glGetError() == GL_NO_ERROR);

	glLinkProgram(programm);
	assert(glGetError() == GL_NO_ERROR);

	//glValidateProgram(programm);

	//GLint status;
	glGetProgramiv(programm, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		int loglen;
		char logbuffer[1000];
		glGetProgramInfoLog(programm, sizeof(logbuffer), &loglen, logbuffer);
		return;
	}
	//glGetProgram()

	glUseProgram(programm);
//	assert(glGetError() == GL_NO_ERROR);
	
	err = glGetError();

	const char *errstr = (const char*)gluErrorString(err);

	//const char *errstr = (const char*)glewGetErrorString(err);

	if (err == GL_INVALID_OPERATION){
		return;
	}


	//assert(glGetError() == GL_NO_ERROR);
	

	m_scripting.SetGodeModeEvent.attach<Game, &Game::Scripting_OnSetGodMode>(this);
}

void Game::MainLoop()
{
	glutMainLoop();
}


void Game::glutDisplay()
{

	//init gl for 3d
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(g_3dMatrix);
	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	float x = cosf(m_angleY) * sinf(m_angleX);
	float y = sinf(m_angleY);
	float z = cosf(m_angleY) * cosf(m_angleX);

	gluLookAt(
		x, y, z, //eye
		0.0f, 0.0f, 0.0f, //center
		0.0f, 1.0f, 0.0f);//up

	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);

	//light
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };

	//glCreateShader(GL_VERTEX_SHADER);



	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_FRONT, GL_SPECULAR, light_specular);
	glLightfv(GL_FRONT, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_FRONT, GL_AMBIENT, light_ambient);

	//material
	GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	
	GLfloat mat_emmision[] = { 0.0, 0.0, 0.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emmision);


	//scene
	glutSolidTeapot(0.3);
	//	glutSolidCone(0.2, 0.2, 4, 4);

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	



	//glBegin(GL_TRIANGLES);
	//{
	//	glVertex3f(0.0f, 0.0f, -2.0f);
	//	glVertex3f(1.0f, 0.0f, -2.0f);
	//	glVertex3f(0.0f, 1.0f, -2.0f);
	//}
	//glEnd();

	//init gl for HUD
	glDisable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(
		0.0f, 0.0f, 1.0f, //eye
		0.0f, 0.0f, 0.0f, //center
		0.0f, 1.0f, 0.0f);//up

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
	gluPerspective(65.0, (float)w / h, 0.1f, 100.0f);

	glGetFloatv(GL_PROJECTION_MATRIX, g_3dMatrix);
	//hud matrix
	glLoadIdentity();
	glOrtho(0.0, w, h, 0.0, -1.0, 1.0); // {0,0} ist oben links

	glGetFloatv(GL_PROJECTION_MATRIX, g_HudMatrix);

	glMatrixMode(GL_MODELVIEW);


	m_scripting.Init();
	m_scripting.SetUiSize({ w, h });
	m_scripting.RunScripts();
}

void Game::glutKeyboard(unsigned char c, int p1, int p2)
{
	m_scripting.KeyboardEvent(c, p1, p2);

	/*
	if (c == 'f')
	{
	glutFullScreenToggle();
	}
	if (c == 'e')
	{
	exit(0);
	}*/
}

void Game::glutMouse(int button, int state, int x, int y)
{
	bool handled = m_scripting.MouseEvent(button, state, x, y);

	if (!handled || button == GLUT_UP)
	{
		m_oldX = x;
		m_oldY = y;
		m_inMotion = state == GLUT_DOWN;
	}
}

void Game::glutMotion(int x, int y)
{
	if (m_inMotion){
		if (m_inMotion) {
			int dx = m_oldX - x;
			int dy = m_oldY - y;

			m_angleX += (float)dx / 360 * 3.1415f;
			m_angleY -= (float)dy / 360 * 3.1415f;
		}
		m_oldX = x;
		m_oldY = y;
	}
}
