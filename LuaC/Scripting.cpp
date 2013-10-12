#include "stdafx.h"
#include "Scripting.h"
#include "CppWrapper.h"

MakeWrapper(LuaErrorWrapper, (lua_State *L), (L), Scripting, int);
MakeWrapper(LuaCppFunction, (lua_State *L), (L), Scripting, int);


Scripting::Scripting(void)
{
}


Scripting::~Scripting(void)
{
}

int Scripting::l_cppfunction(lua_State *L) {
	double arg = luaL_checknumber(L,1);
	lua_pushnumber(L, arg * 0.5);
	return 1;
}


void Scripting::Init(void)
{
	luaState = luaL_newstate();
	luaL_openlibs(luaState);

	lua_pushcfunction(luaState, GetWrapper(LuaCppFunction, &Scripting::l_cppfunction));
	lua_setglobal(luaState, "cppfunction");

}

int Scripting::lua_atPanicFunctuon(lua_State *L)
{
	luaError = lua_tostring(L, -1);
	lua_pop(L, 1);
	longjmp(jumpBuffer, 1);
	return 1;
}

void Scripting::RunScripts(void)
{
	lua_atpanic(luaState, GetWrapper(LuaErrorWrapper, &Scripting::lua_atPanicFunctuon)); 

	int r = setjmp(jumpBuffer);

	if (r == 0)
	{
		if (luaL_loadfile(luaState, "../LuaC/script.lua")) {
			luaError = lua_tostring(luaState, -1);
			lua_pop(luaState, 1);
		}
		else{
			luaError = NULL;
			lua_call(luaState, 0, LUA_MULTRET);
		}
	}
}

bool Scripting::HasError()
{
	return luaError != NULL;
}

const char *Scripting::GetError()
{
	return luaError;
}

void Scripting::RenderHUD()
{
	if (HasError())
	{
		//glRasterPos4f(-1.0f, -1.0f, -5.0f, 1.0f);
		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
		glRasterPos2f(0.10f, 0.10f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char *)GetError());
	}
}