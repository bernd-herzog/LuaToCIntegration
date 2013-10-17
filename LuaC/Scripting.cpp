#include "stdafx.h"
#include "Scripting.h"
#include "CppWrapper.h"

#include "UIButton.h"
#include "UILabel.h"
#include "UIImage.h"
#include "UITextField.h"

#include "LuaBind.h"
#include <functional>

MakeWrapper(LuaErrorWrapper, (lua_State *L), (L), Scripting, int);

MakeWrapper(LuaFunctionSetGodMode, (lua_State *L), (L), Scripting, int);

MakeWrapper(LuaFunction_RegisterEvent, (lua_State *L), (L), Scripting, int);

Scripting::Scripting(void)
{
}


Scripting::~Scripting(void)
{
}

void Scripting::Init(void)
{
	luaState = luaL_newstate();
	luaL_openlibs(luaState);

	lua_pushcfunction(luaState, GetWrapper(LuaFunctionSetGodMode, &Scripting::lua_SetGodMode));
	lua_setglobal(luaState, "SetGodMode");

	lua_pushcfunction(luaState, GetWrapper(LuaFunction_RegisterEvent, &Scripting::lua_RegisterEvent));
	lua_setglobal(luaState, "RegisterEvent");

	std::function<void (UIElement *)> func = [this](UIElement *field){
		m_uiElements.push_back(field);
	};

	UIElement::Init(luaState);
	UITextField::Init(luaState, func);
	UILabel::Init(luaState, func);

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
		glRasterPos2f(20.0f, 20.0f);

		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char *)GetError());
	}

	for (UIElement * var : this->m_uiElements)
	{
		var->Render();
	}
}


int Scripting::lua_SetGodMode(lua_State *L)
{
	this->SetGodeModeEvent.OnEvent(this, EventArgs<int>(0));

	return 0;
}

void Scripting::MouseEvent(int button, int state, int x, int y){

}

void Scripting::KeyboardEvent(unsigned char c, int p1, int p2){

}

int Scripting::lua_RegisterEvent(lua_State *L)
{
	return 0;
}

