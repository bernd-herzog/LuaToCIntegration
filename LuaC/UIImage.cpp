#include "stdafx.h"
#include "UIImage.h"


UIImage::UIImage(void)
{
}


UIImage::~UIImage(void)
{
}


/* static */ int UIImage::lua_Index(lua_State *L)
{
	// -1: string "SetPosition" // 2nd parameter
	// -2: userdata UITextField // first parameter

	UIImage *obj = static_cast<UIImage *>(luaL_checkudata(L, -2, "UIImage"));
	const char *func = luaL_checkstring(L, -1);

	/*if (strcmp(func, "SetPosition") == 0)
	{
		lua_pushcfunction(L, GetWrapper2(LuaFunction_UITextField_SetPosition, &UITextField::lua_SetPosition, obj));
		return 1;
	}*/

	return 0; //UIElement::lua_Index(L);
}

/* virtual */ void UIImage::Render()
{

}