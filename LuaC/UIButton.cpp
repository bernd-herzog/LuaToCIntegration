#include "stdafx.h"
#include "UIButton.h"


UIButton::UIButton(void)
{
}


UIButton::~UIButton(void)
{
}


/* static */ int UIButton::lua_Index(lua_State *L)
{
	// -1: string "SetPosition" // 2nd parameter
	// -2: userdata UITextField // first parameter

	UIButton *obj = static_cast<UIButton *>(luaL_checkudata(L, -2, "UIButton"));
	const char *func = luaL_checkstring(L, -1);

	/*if (strcmp(func, "SetPosition") == 0)
	{
		lua_pushcfunction(L, GetWrapper2(LuaFunction_UITextField_SetPosition, &UITextField::lua_SetPosition, obj));
		return 1;
	}*/

	return 0; //UIElement::lua_Index(L);
}

/* virtual */ void UIButton::Render()
{

}