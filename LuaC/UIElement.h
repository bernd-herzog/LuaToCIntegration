#pragma once

#include "LuaBind.h"

class UIElement
{
public:
	UIElement(void);
	~UIElement(void);

	//static int lua_Index(lua_State *L);

	int lua_SetPosition(lua_State *L);
	int lua_SetSize(lua_State *L);
	int lua_Show(lua_State *L);
	int lua_RegisterEvent(lua_State *L);

	static void Init(lua_State *L);

	typedef UIElement Base;

	virtual void Render() = 0;
};

