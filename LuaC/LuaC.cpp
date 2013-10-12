// LuaC.cpp : Definiert den Einstiegspunkt für die Anwendung.
//

#include "stdafx.h"
#include "LuaC.h"

#include "Game.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	Game *theGame = new Game();
	theGame->Init();
	theGame->MainLoop();


	return 0;
}

