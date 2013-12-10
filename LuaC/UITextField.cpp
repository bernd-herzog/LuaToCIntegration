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
		LuaBinding::registerFunc<&UITextField::SetIsMultiLine>("SetIsMultiLine");
	}

	LuaBinding::Init(L, "CreateTextField");

}

UITextField::UITextField(void)
	:m_str(""), m_hasFocus(false), m_cursorPosition(0), m_isMultiLine(false)
{
}

UITextField::~UITextField(void)
{
}

int LUA_FUNCTION UITextField::SetText(lua_State *L)
{
	this->SetText(luaL_checkstring(L, 2));
	return 0;
}

int LUA_FUNCTION UITextField::GetText(lua_State *L)
{
	lua_pushstring(L, m_str.c_str());
	return 1;
}

int LUA_FUNCTION UITextField::SetIsMultiLine(lua_State *L)
{
	int i = lua_toboolean(L, 2);
	m_isMultiLine = i == 1;
	return 0;
}

bool UITextField::GetHasFocus()
{
	return this->m_hasFocus;
}
void UITextField::SetHasFocus(bool hasFocus)
{
	this->m_hasFocus = hasFocus;
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
		glRasterPos2i(dim.x + 1, dim.y + dim.height - 12);
		glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *)m_str.c_str());


		//cursor
		if (this->m_hasFocus)
		{
			//GLint rPos[4];
			//glGetIntegerv(GL_CURRENT_RASTER_POSITION, rPos);

			POINT p = GetCusorCoordinates();

			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_LINES);
			glVertex2i(p.x, p.y+11);
			glVertex2i(p.x, p.y-3);
			glEnd();
		}
		//end

		glDisable(GL_STENCIL_TEST);

	}
}

POINT UITextField::GetCusorCoordinates()
{
	POINT p;

	std::string lString = this->m_str.substr(0, this->m_cursorPosition);

	int lines = 1;
	int chars = 0;

	for(auto c : lString)
	{
		if (c == '\n')
		{
			chars = 0;
			lines++;
		}
		else
		{
			chars ++;
		}
	}

	Dimension dim = GetAbsoluteDimensions();

	p.x = dim.x + 8*chars + 1;
	p.y = dim.y + dim.height - 14*lines + 3 - 1;

	return p;
}

void UITextField::SetCursorByCoordiantes(int x, int y)
{
	Dimension dim = GetAbsoluteDimensions();

	int chars = (x - dim.x + 4)/8;
	int lines = (y - dim.y - dim.height +10)/-13;

	int line = 0;
	int charsTotal = 0;
	int charsTotalLastLine = 0;
	int charsThisLine = 0;

	while (true){
		charsTotal = this->m_str.find('\n', charsTotalLastLine);

		if (lines <= line) // this is the line
		{
			if (charsTotal == std::string::npos) // no more lines
			{
				charsThisLine = this->m_str.length() - charsTotalLastLine;
			}
			else
			{
				charsThisLine = charsTotal - charsTotalLastLine;
			}

			if (charsThisLine < chars)
			{
				this->SetCursorPosition(charsTotalLastLine + charsThisLine);
			}
			else
			{
				this->SetCursorPosition(charsTotalLastLine + chars);
			}
			return;
		}
		else // search more
		{
			if (charsTotal == std::string::npos) // but there are no more lines
			{
				this->SetCursorPosition(this->m_str.length());
				return;
			}

		}

		line++;
		charsTotalLastLine = charsTotal+1;
	}
}

std::string UITextField::GetText()
{
	return this->m_str;
}

void UITextField::SetText(std::string str)
{
	this->m_str = str;

	m_cursorPosition = this->m_str.length();
}

void UITextField::SetCursorPositionRelative(int to)
{
	SetCursorPosition(to + GetCursorPosition());
}

void UITextField::SetCursorPositionRelativeVertical(int to)
{
	int lines = 1;
	int chars = 0;

	std::string lString = this->m_str.substr(0, this->m_cursorPosition);
	for(auto c : lString)
	{
		if (c == '\n')
		{
			chars = 0;
			lines++;
		}
		else
		{
			chars++;
		}
	}

	lines += to - 1;

	int line = 0;
	int charsTotal = 0;
	int charsTotalLastLine = 0;
	int charsThisLine = 0;

	while (true){
		charsTotal = this->m_str.find('\n', charsTotalLastLine);

		if (lines <= line) // this is the line
		{
			if (charsTotal == std::string::npos) // no more lines
			{
				charsThisLine = this->m_str.length() - charsTotalLastLine;
			}
			else
			{
				charsThisLine = charsTotal - charsTotalLastLine;
			}

			if (charsThisLine < chars)
			{
				this->SetCursorPosition(charsTotalLastLine + charsThisLine);
			}
			else
			{
				this->SetCursorPosition(charsTotalLastLine + chars);
			}
			return;
		}
		else // search more
		{
			if (charsTotal == std::string::npos) // but there are no more lines
			{
				this->SetCursorPosition(this->m_str.length());
				return;
			}

		}

		line++;
		charsTotalLastLine = charsTotal+1;
	}

}

void UITextField::SetCursorPosition(int to)
{
	this->m_cursorPosition = to;
}

int UITextField::GetCursorPosition()
{
	return this->m_cursorPosition;
}

void UITextField::SendKey(lua_State *L, unsigned char c)
{

	if (c == 8) // backspace
	{
		if (this->m_cursorPosition > 0)
		{
			std::string text = this->m_str.substr(0, this->m_cursorPosition-1);
			text = text.append(this->m_str.substr(this->m_cursorPosition));

			this->m_str = text;

			this->SetCursorPositionRelative(-1);
		}
	}
	else if (c == 22) // strg + v
	{
		if (OpenClipboard(NULL)){
			char *clipText = (char *)GetClipboardData(CF_TEXT);
			CloseClipboard();
			if (clipText != nullptr)
			{
				//SetText(GetText().append(clipText));
				std::string sText = clipText;

				std::string text = this->m_str.substr(0, this->m_cursorPosition);
				text = text.append(sText);
				text = text.append(this->m_str.substr(this->m_cursorPosition));

				this->m_str = text;

				this->SetCursorPositionRelative(sText.length());

			}
		}

	}
	else
	{
		if (c == '\r')
		{
			if (m_isMultiLine)
			{
				c = '\n';
			}
			else
			{
				if (this->m_luaEventHandlerId != -1)
				{
					if (this->m_registeredEvents.find("EVENT_TEXT_SUBMIT") != this->m_registeredEvents.end())
					{
						lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_luaEventHandlerId);
						lua_rawgeti(L, LUA_REGISTRYINDEX, this->m_luaUserDataRefId);
						lua_pushstring(L, "EVENT_TEXT_SUBMIT");
						lua_call(L, 2, 0); // source, event_text
					}
				}
				return;
			}
		}

		char b[2] = { c, 0 };

		std::string text = this->m_str.substr(0, this->m_cursorPosition);
		text = text.append(b);
		text = text.append(this->m_str.substr(this->m_cursorPosition));

		this->m_str = text;

		this->SetCursorPositionRelative(1);
	}
}
