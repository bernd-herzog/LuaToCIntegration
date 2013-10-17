#include "stdafx.h"
#include "UITextField.h"
#include <functional>

void UITextField::Init(lua_State *L, std::function<void (UIElement *)> func)
{
	typedef LuaBinding<UITextField> LuaBinding;


	static bool once = false;
	if (!once)
	{
		once = true;
		LuaBinding::InitOnce("UITextField", func);

		LuaBinding::registerFunc<&UITextField::SetText>("SetText");
	}

	LuaBinding::Init(L, "CreateTextField");

}

UITextField::UITextField(void)
{
}

UITextField::~UITextField(void)
{
}

int LUA_FUNCTION UITextField::SetText(lua_State *L)
{
	return 0;
}

/* virtual */ void UITextField::Render()
{
	glBegin(GL_TRIANGLES);

	glVertex2f(50.0f, 50.0f);
	glVertex2f(50.0f, 150.0f);
	glVertex2f(150.0f, 50.0f);

	glEnd();
}