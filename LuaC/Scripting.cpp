#include "stdafx.h"
#include "Scripting.h"
#include "CppWrapper.h"

#include "UIButton.h"
#include "UILabel.h"
#include "UIImage.h"
#include "UITextField.h"

#include "LuaBind.h"
#include <functional>

MakeWrapper(LuaErrorWrapper, (lua_State *L), (L), Scripting, int);
MakeWrapper(LuaFunction_RegisterEvent, (lua_State *L), (L), Scripting, int);

MakeWrapper(LuaFunctionInstallVertexShader, (lua_State *L), (L), Scripting, int);
MakeWrapper(LuaFunctionInstallFragmentShader, (lua_State *L), (L), Scripting, int);

Scripting::Scripting(void)
:_uiSize({ 0, 0 })
{
}


Scripting::~Scripting(void)
{
}


void Scripting::SetUiSize(SIZE size)
{
	this->_uiSize = size;
}


void Scripting::Init(void)
{
	//TODO: alten Luastate und alte objekte aus dem speicher freigeben
	m_uiElements.clear();

	luaState = luaL_newstate();
	luaL_openlibs(luaState);

	lua_pushcfunction(luaState, GetWrapper(LuaFunction_RegisterEvent, &Scripting::lua_RegisterEvent));
	lua_setglobal(luaState, "RegisterEvent");

	lua_pushcfunction(luaState, GetWrapper(LuaFunctionInstallFragmentShader, &Scripting::lua_InstallFragmentShader));
	lua_setglobal(luaState, "InstallFragmentShader");
	lua_pushcfunction(luaState, GetWrapper(LuaFunctionInstallVertexShader, &Scripting::lua_InstallVertexShader));
	lua_setglobal(luaState, "InstallVertexShader");


	std::function<void(UIElement *)> func = [this](UIElement *field){
		field->SetParentDimensions({ 0, 0 }, { this->_uiSize.cx, this->_uiSize.cy });
		m_uiElements.push_back(field);
	};

	UIElement::Init(luaState);

	UITextField::Init(luaState, func);
	UILabel::Init(luaState, func);
	UIButton::Init(luaState, func);
	UIImage::Init(luaState, func);
}

int Scripting::lua_atPanicFunctuon(lua_State *L)
{
	luaError = lua_tostring(L, -1);
	lua_pop(L, 1);
	longjmp(jumpBuffer, 1);
	return 1;
}

void Scripting::RunScripts(void)
{
	lua_atpanic(luaState, GetWrapper(LuaErrorWrapper, &Scripting::lua_atPanicFunctuon));

	int r = setjmp(jumpBuffer);

	if (r == 0)
	{
		if (luaL_loadfile(luaState, "../LuaC/script.lua")) {
			luaError = lua_tostring(luaState, -1);
			lua_pop(luaState, 1);
		}
		else{
			luaError = NULL;
			lua_call(luaState, 0, LUA_MULTRET);
		}
	}
}

bool Scripting::HasError()
{
	return luaError != NULL;
}

const char *Scripting::GetError()
{
	return luaError;
}

void Scripting::RenderHUD()
{
	if (HasError())
	{
		//glRasterPos4f(-1.0f, -1.0f, -5.0f, 1.0f);
		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
		glRasterPos2f(20.0f, 20.0f);

		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char *)GetError());
	}

	for (UIElement * var : this->m_uiElements)
	{
		var->Render();
	}
}

int LUA_FUNCTION Scripting::lua_InstallFragmentShader(lua_State *L)
{
	char *source = const_cast<char *>(luaL_checkstring(L, 1));
	InstallFragmentShader.OnEvent(this, EventArgs<char *>(source));
	return 0;
}

int LUA_FUNCTION Scripting::lua_InstallVertexShader(lua_State *L)
{
	char *source = const_cast<char *>(luaL_checkstring(L, 1));
	InstallVertexShader.OnEvent(this, EventArgs<char *>(source));
	return 0;
}


bool Scripting::MouseEvent(int button, int state, int x, int y)
{
	lua_atpanic(luaState, GetWrapper(LuaErrorWrapper, &Scripting::lua_atPanicFunctuon));

	int r = setjmp(jumpBuffer);

	if (r == 0)
	{
		// wenn hier in TextField ist Focus setzen
		// wenn hier ein button ist Click event senden
		// sonst focus löschen
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			m_focus = nullptr;

			for (auto feld : this->m_uiElements)
			{
				UITextField *textField = dynamic_cast<UITextField *>(feld);

				if (textField != nullptr)
				{
					Dimension dim = textField->GetAbsoluteDimensions();

					if (x > dim.x && x < dim.x + dim.width &&
						y > dim.y && y < dim.y + dim.height) // hit test
					{
						m_focus = textField;
						return true;
					}
				}

				UIButton *button = dynamic_cast<UIButton *>(feld);

				if (button != nullptr)
				{
					Dimension dim = button->GetAbsoluteDimensions();

					if (x > dim.x && x < dim.x + dim.width &&
						y > dim.y && y < dim.y + dim.height) // hit test
					{
						button->OnClick(luaState);
						return true;
					}
				}
			}
		}
		else if ((button == 3 || button == 4) && state == GLUT_UP)
		{
			for (auto feld : this->m_uiElements)
			{
				UILabel *textField = dynamic_cast<UILabel *>(feld);

				if (textField != nullptr)
				{
					Dimension dim = textField->GetAbsoluteDimensions();

					if (x > dim.x && x < dim.x + dim.width &&
						y > dim.y && y < dim.y + dim.height) // hit test
					{
						textField->Scroll(button == 3);
						return true;
					}
				}
			}
		}
	}
	return false;
}

void Scripting::KeyboardEvent(unsigned char c, int p1, int p2)
{
	// wenn TextField focus hat ihm die taste senden
	if (this->m_focus != nullptr)
	{
		if (c == 8) // backspace
		{
			std::string text = m_focus->GetText();

			text = text.substr(0, text.length() - 1);

			m_focus->SetText(text);
		}
		else if (c == 22) // strg + v
		{
			if (OpenClipboard(NULL)){
				char *text = (char *)GetClipboardData(CF_TEXT);
				CloseClipboard();
				if (text != nullptr)
				{
					m_focus->SetText(m_focus->GetText().append(text));
				}
			}
			
		}
		else
		{
			if (c == '\r')
				c = '\n';

			char b[2] = { c, 0 };
			std::string text = m_focus->GetText();

			text = text.append(b);

			m_focus->SetText(text);
		}
	}
}

int Scripting::lua_RegisterEvent(lua_State *L)
{
	return 0;
}

