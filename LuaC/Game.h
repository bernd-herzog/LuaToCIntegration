#pragma once

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

	GLfloat g_3dMatrix[16];
	GLfloat g_HudMatrix[16];

	void glutDisplay();
	void glutReshape(int w, int h);
	void glutKeyboard(unsigned char c, int p1, int p2);
	void glutMouse(int button, int state, int x, int y);
	void glutMotion(int x, int y);


	Scripting m_scripting;
	void Scripting_OnSetGodMode(Scripting*, EventArgs<int>);
};

