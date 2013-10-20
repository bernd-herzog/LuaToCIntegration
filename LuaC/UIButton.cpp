#include "stdafx.h"
#include "UIButton.h"

void UIButton::Init(lua_State *L, std::function<void (UIElement *)> func)
{
	typedef LuaBinding<UIButton> LuaBinding;


	static bool once = false;
	if (!once)
	{
		LuaBinding::InitOnce("UIButton", func);

		once = true;
		LuaBinding::registerFunc<&UIButton::SetText>("SetText");
	}

	LuaBinding::Init(L, "CreateButton");
}

UIButton::UIButton(void)
{
}


UIButton::~UIButton(void)
{
}


/* virtual */ void UIButton::Render()
{
	if (_visible)
	{
		Dimension dim = GetAbsoluteDimensions();

		glColor4f(0.2f, 0.2f, 0.2f, 1.0f);

		glBegin(GL_QUADS);

		glVertex2i(dim.x, dim.y);
		glVertex2i(dim.x + dim.width, dim.y);
		glVertex2i(dim.x + dim.width, dim.y + dim.height);
		glVertex2i(dim.x, dim.y + dim.height);

		glEnd();


		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

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

		int textWidth = _str.length() * 8;
		int textHeight = 13;

		glRasterPos2i(dim.x + dim.width / 2 - textWidth / 2, dim.y + dim.height / 2 - textHeight / 2 + 13 - 3); // -3 ist ungekl�rt
		glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *)_str.c_str());

		//end
		glDisable(GL_STENCIL_TEST);
	}
}

int LUA_FUNCTION UIButton::SetText(lua_State *L)
{
	_str = luaL_checkstring(L, 2);

	return 0;
}

void UIButton::OnClick(lua_State *L)
{
	if (this->m_luaEventHandlerId != -1)
	{
		if (this->m_registeredEvents.find("EVENT_BUTTON_CLICK") != this->m_registeredEvents.end())
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_luaEventHandlerId);
			lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_luaUserDataRefId);
			lua_pushstring(L, "EVENT_BUTTON_CLICK");
			lua_call(L, 2, 0); // source, event_text
		}
	}

	//call
	//lua_rawgeti(lua, LUA_REGISTRYINDEX, m_Ref);
	//int			error = lua_pcall(lua, 0, 0, -2);

}
