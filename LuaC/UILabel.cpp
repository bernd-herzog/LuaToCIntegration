#include "stdafx.h"
#include "UILabel.h"

void UILabel::Init(lua_State *L, std::function<void (UIElement *)> func)
{
	typedef LuaBinding<UILabel> LuaBinding;


	static bool once = false;
	if (!once)
	{
		LuaBinding::InitOnce("UILabel", func);

		once = true;
		LuaBinding::registerFunc<&UILabel::SetText>("SetText");
		LuaBinding::registerFunc<&UILabel::GetText>("GetText");
	}

	LuaBinding::Init(L, "CreateLabel");
}

UILabel::UILabel(void)
{
}


UILabel::~UILabel(void)
{
}

int LUA_FUNCTION UILabel::SetText(lua_State *L)
{
	_str = luaL_checkstring(L, 2);

	int lines = 1;
	for (auto c : _str)
	{
		if (c == '\n')
			lines++;
	}

	if (_str.rbegin() != _str.rend() && *_str.rbegin() == '\n')
		lines--;

	int max = lines * 14 - this->_dimSelf.height;
	m_scroll = max;

	if (m_scroll < 0)
		m_scroll = 0;


	return 0;
}

int LUA_FUNCTION UILabel::GetText(lua_State *L)
{
	lua_pushstring(L, _str.c_str());
	return 1;
}

/* virtual */ void UILabel::Render()
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
		glBitmap(0, 0, 0, 0, 0, m_scroll, NULL);
		glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *)_str.c_str());

		//end
		glDisable(GL_STENCIL_TEST);


	}
}

void UILabel::Scroll(bool up)
{
	m_scroll -= up ? 14 : -14;


	int lines = 1;
	for (auto c : _str)
	{
		if (c == '\n')
			lines++;
	}

	if (_str.rbegin() != _str.rend() && *_str.rbegin() == '\n')
		lines--;

	int max = lines * 14 - this->_dimSelf.height;
	if (m_scroll > max)
	{
		m_scroll = max;
	}

	if (m_scroll < 0)
		m_scroll = 0;

}

