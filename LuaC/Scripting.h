#pragma once

#include <vector>
#include "Delegate.h"
#include "UIElement.h"
#include "UITextField.h"

using namespace std;


class Scripting
{
public:
	Scripting(void);
	~Scripting(void);

	void Init();
	void RunScripts();

	bool HasError();
	const char *GetError();

	void RenderHUD();

	Event<Scripting, int> SetGodeModeEvent;

	bool MouseEvent(int button, int state, int x, int y);
	void KeyboardEvent(unsigned char c, int p1, int p2);

	void SetUiSize(SIZE rect);

private:
	SIZE _uiSize;

	lua_State *luaState;

	const char *luaError;
	jmp_buf jumpBuffer;

	int lua_atPanicFunctuon(lua_State *L); 

	//Called from LUA
	int lua_SetGodMode(lua_State *L);
	int lua_RegisterEvent(lua_State *L);

	vector<UIElement *> m_uiElements;		
	UITextField *m_focus;

};

