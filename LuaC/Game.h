#pragma once
#include "stdafx.h"
#include "Scripting.h"

class Game
{
public:
	Game(void);
	~Game(void);

	void Init();
	void MainLoop();

	bool m_inMotion;
	int m_oldX;
	int m_oldY;

	float m_angleX;
	float m_angleY;

private:
	GLuint m_vertexShader;
	GLuint m_fragmentShader;
	GLuint m_shaderProgram;

	bool CreateVertexShader(const char *source);
	bool CreateFragmentShader(const char *source);


	GLfloat g_3dMatrix[16];
	GLfloat g_HudMatrix[16];

	void glutDisplay();
	void glutReshape(int w, int h);
	void glutKeyboard(unsigned char c, int p1, int p2);
	void glutSpecial(int c, int p1, int p2);
	void glutMouse(int button, int state, int x, int y);
	void glutMotion(int x, int y);

	int m_windowHeight;
	int m_windowWidth;


	Scripting m_scripting;

	void Scripting_OnInstallFragmentShader(Scripting*, EventArgs<char *>);
	void Scripting_OnInstallVertexShader(Scripting*, EventArgs<char *>);
};

