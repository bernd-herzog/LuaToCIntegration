#include "stdafx.h"
#include "UIElement.h"
#include "CppWrapper.h"

MakeWrapper(LuaFunction_UIElement_SetPosition, (lua_State *L), (L), UIElement, int);
MakeWrapper(LuaFunction_UIElement_SetSize, (lua_State *L), (L), UIElement, int);
MakeWrapper(LuaFunction_UIElement_Show, (lua_State *L), (L), UIElement, int);
MakeWrapper(LuaFunction_UIElement_RegisterEvent, (lua_State *L), (L), UIElement, int);


UIElement::UIElement(void)
{
}


UIElement::~UIElement(void)
{
}

void UIElement::Init(lua_State *L)
{
	typedef LuaBinding<UIElement> LuaBinding;

	static bool once = false;
	if (!once)
	{
		once = true;
		LuaBinding::registerFunc<&UIElement::lua_RegisterEvent>("RegisterEvent");
		LuaBinding::registerFunc<&UIElement::lua_SetPosition>("SetPosition");
		LuaBinding::registerFunc<&UIElement::lua_SetSize>("SetSize");
		LuaBinding::registerFunc<&UIElement::lua_Show>("Show");
	}
}
//
///* static */ int UIElement::lua_Index(lua_State *L)
//{
//	// -1: string "SetPosition" // 2nd parameter
//	// -2: userdata UITextField // first parameter
//
//	UIElement *obj = static_cast<UIElement *>(lua_touserdata(L, -2));
//	const char *func = luaL_checkstring(L, -1);
//
//	if (strcmp(func, "SetPosition") == 0)
//	{
//		lua_pushcfunction(L, GetWrapper2(LuaFunction_UIElement_SetPosition, &UIElement::lua_SetPosition, obj));
//		return 1;
//	} 
//	else if (strcmp(func, "SetSize") == 0)
//	{
//		lua_pushcfunction(L, GetWrapper2(LuaFunction_UIElement_SetSize, &UIElement::lua_SetSize, obj));
//		return 1;
//	}
//	else if (strcmp(func, "Show") == 0)
//	{
//		lua_pushcfunction(L, GetWrapper2(LuaFunction_UIElement_Show, &UIElement::lua_Show, obj));
//		return 1;
//	}
//	else if (strcmp(func, "RegisterEvent") == 0)
//	{
//		lua_pushcfunction(L, GetWrapper2(LuaFunction_UIElement_RegisterEvent, &UIElement::lua_RegisterEvent, obj));
//		return 1;
//	} 
//
//	/*
//	const char *u = luaL_checkstring(L, -1);
//	lua_pushfstring(L, u);
//	lua_error(L);
//
//	const char *tname = lua_typename(L, lua_type(L, -10));
//	lua_pushfstring(L, tname);
//	lua_error(L);
//
//	if (!lua_istable(L, -5)){
//	lua_pushfstring(L, "no table?");
//	lua_error(L);
//	}
//	*/
//
//
//	return 0;
//}

int UIElement::lua_SetPosition(lua_State *L)
{
	return 0;
}

int UIElement::lua_SetSize(lua_State *L)
{
	return 0;
}

int UIElement::lua_Show(lua_State *L)
{
	return 0;
}

int UIElement::lua_RegisterEvent(lua_State *L)
{
	return 0;
}
