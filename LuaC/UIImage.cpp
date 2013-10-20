#include "stdafx.h"
#include "UIImage.h"

void UIImage::Init(lua_State *L, std::function<void (UIElement *)> func)
{
	typedef LuaBinding<UIImage> LuaBinding;


	static bool once = false;
	if (!once)
	{
		LuaBinding::InitOnce("UIImage", func);

		once = true;
		LuaBinding::registerFunc<&UIImage::SetTexture>("SetTexture");
	}

	LuaBinding::Init(L, "CreateImage");
}

UIImage::UIImage(void)
{
}


UIImage::~UIImage(void)
{
}

/* virtual */ void UIImage::Render()
{

}

int LUA_FUNCTION UIImage::SetTexture(lua_State *L)
{
	return 0;
}
