#pragma once

#include <vector>
#include "Delegate.h"

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

private:
	lua_State *luaState;

	const char *luaError;
	jmp_buf jumpBuffer;

	int lua_atPanicFunctuon(lua_State *L); 
	int l_cppfunction(lua_State *L);

	//Called from LUA
	int lua_CreateButton(lua_State *L);
	int lua_CreateLabel(lua_State *L);
	int lua_CreateTextField(lua_State *L);
	int lua_CreateImage(lua_State *L);

	int lua_SetGodMode(lua_State *L);

};

