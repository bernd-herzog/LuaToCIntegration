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

	Event<Scripting, char *> InstallFragmentShader;
	Event<Scripting, char *> InstallVertexShader;

	bool MouseEvent(int button, int state, int x, int y);
	void KeyboardEvent(unsigned char c, int p1, int p2);
	void SpecialEvent(int c, int p1, int p2);

	void SetUiSize(SIZE rect);

	void PostLogEvent(char *);

	int m_luaEventHandlerId;
	std::set<std::string> m_registeredEvents;


private:
	SIZE _uiSize;

	lua_State *luaState;

	const char *luaError;
	jmp_buf jumpBuffer;

	int LUA_FUNCTION lua_atPanicFunctuon(lua_State *L);

	//Called from LUA
	int LUA_FUNCTION lua_RegisterEvent(lua_State *L);
	int LUA_FUNCTION lua_SetEventHandler(lua_State *L);

	int LUA_FUNCTION lua_InstallFragmentShader(lua_State *L);
	int LUA_FUNCTION lua_InstallVertexShader(lua_State *L);

	vector<UIElement *> m_uiElements;		
	UITextField *m_focus;

};

