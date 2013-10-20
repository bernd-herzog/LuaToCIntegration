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
		LuaBinding::registerFunc<&UITextField::GetText>("GetText");
	}

	LuaBinding::Init(L, "CreateTextField");

}

UITextField::UITextField(void)
	:_str("")
{
}

UITextField::~UITextField(void)
{
}

int LUA_FUNCTION UITextField::SetText(lua_State *L)
{
	_str = luaL_checkstring(L, 2);

	return 0;
}

int LUA_FUNCTION UITextField::GetText(lua_State *L)
{
	lua_pushstring(L, _str.c_str());
	return 1;
}

/* virtual */ void UITextField::Render()
{
	if (_visible)
	{
		Dimension dim = GetAbsoluteDimensions();

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		glBegin(GL_QUADS);

		glVertex2i(dim.x, dim.y);
		glVertex2i(dim.x + dim.width, dim.y);
		glVertex2i(dim.x + dim.width, dim.y + dim.height);
		glVertex2i(dim.x, dim.y + dim.height);

		glEnd();


		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		
		//stencil
		glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glColorMask(false, false, false, false);

		//draw
		glBegin(GL_QUADS);

		glVertex2i(dim.x, dim.y);
		glVertex2i(dim.x + dim.width, dim.y);
		glVertex2i(dim.x + dim.width, dim.y + dim.height);
		glVertex2i(dim.x, dim.y + dim.height);

		glEnd();

		glColorMask(true, true, true, true);
		glStencilFunc(GL_EQUAL, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		//text
		glRasterPos2i(dim.x, dim.y + 10);
		glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *)_str.c_str());

		//end
		glDisable(GL_STENCIL_TEST);


	}
}

std::string UITextField::GetText()
{
	return this->_str;
}

void UITextField::SetText(std::string str)
{
	this->_str = str;
}
