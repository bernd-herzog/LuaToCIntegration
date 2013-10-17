#include "stdafx.h"
#include "Scripting.h"
#include "CppWrapper.h"

#include "UIButton.h"
#include "UILabel.h"
#include "UIImage.h"
#include "UITextField.h"

MakeWrapper(LuaErrorWrapper, (lua_State *L), (L), Scripting, int);

MakeWrapper(LuaFunctionCreateButton, (lua_State *L), (L), Scripting, int);
MakeWrapper(LuaFunctionCreateLabel, (lua_State *L), (L), Scripting, int);
MakeWrapper(LuaFunctionCreateTextField, (lua_State *L), (L), Scripting, int);
MakeWrapper(LuaFunctionCreateImage, (lua_State *L), (L), Scripting, int);
MakeWrapper(LuaFunctionSetGodMode, (lua_State *L), (L), Scripting, int);

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


	lua_pushcfunction(luaState, GetWrapper(LuaFunctionCreateButton, &Scripting::lua_CreateButton));
	lua_setglobal(luaState, "CreateButton");

	lua_pushcfunction(luaState, GetWrapper(LuaFunctionCreateLabel, &Scripting::lua_CreateLabel));
	lua_setglobal(luaState, "CreateLabel");

	lua_pushcfunction(luaState, GetWrapper(LuaFunctionCreateTextField, &Scripting::lua_CreateTextField));
	lua_setglobal(luaState, "CreateTextField");

	lua_pushcfunction(luaState, GetWrapper(LuaFunctionCreateImage, &Scripting::lua_CreateImage));
	lua_setglobal(luaState, "CreateImage");

	lua_pushcfunction(luaState, GetWrapper(LuaFunctionSetGodMode, &Scripting::lua_SetGodMode));
	lua_setglobal(luaState, "SetGodMode");

	luaL_newmetatable(luaState, "UIButton");
	lua_pop(luaState, 1);

	luaL_newmetatable(luaState, "UIImage");
	lua_pop(luaState, 1);

	luaL_newmetatable(luaState, "UILabel");
	

	lua_pop(luaState, 1);

	luaL_newmetatable(luaState, "UITextField");

	lua_pushcfunction(luaState, NULL);
	lua_setfield(luaState, -2, "SetPosition");

	lua_pop(luaState, 1);

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

int Scripting::lua_CreateButton(lua_State *L)
{
	UIButton *button = new UIButton();
	m_uiElements.push_back(button);

	UIButton **b = static_cast<UIButton **>(lua_newuserdata(L, sizeof(UIButton *)));

	*b = button;

	luaL_getmetatable(luaState, "UIButton");
	lua_setmetatable(L, -2);


	return 1;
}

int Scripting::lua_CreateLabel(lua_State *L)
{
	UILabel *button = new UILabel();
	m_uiElements.push_back(button);

	UILabel **b = static_cast<UILabel **>(lua_newuserdata(L, sizeof(UILabel *)));

	*b = button;

	luaL_getmetatable(luaState, "UILabel");
	lua_setmetatable(L, -2);


	return 1;
}

int Scripting::lua_CreateTextField(lua_State *L)
{
	UITextField *button = new UITextField();
	m_uiElements.push_back(button);

	UITextField **b = static_cast<UITextField **>(lua_newuserdata(L, sizeof(UITextField *)));

	*b = button;

	luaL_getmetatable(luaState, "UITextField");
	lua_setmetatable(L, -2);


	return 1;
}

int Scripting::lua_CreateImage(lua_State *L)
{
	UIImage *button = new UIImage();
	m_uiElements.push_back(button);

	UIImage **b = static_cast<UIImage **>(lua_newuserdata(L, sizeof(UIImage *)));

	*b = button;

	luaL_getmetatable(luaState, "UIImage");
	lua_setmetatable(L, -2);


	return 1;
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
