#include "stdafx.h"
#include "UILabel.h"
#include "CppWrapper.h"

void UILabel::Init(lua_State *L, std::function<void (UIElement *)> func)
{
	typedef LuaBinding<UILabel> LuaBinding;


	static bool once = false;
	if (!once)
	{
		LuaBinding::InitOnce("UILabel", func);

		once = true;
		LuaBinding::registerFunc<&UILabel::SetText>("SetText");
	}

	LuaBinding::Init(L, "CreateLabel");
}

UILabel::UILabel(void)
{
}


UILabel::~UILabel(void)
{
}



int UILabel::SetText(lua_State *L)
{
	return 0;
}

/* virtual */ void UILabel::Render()
{

}