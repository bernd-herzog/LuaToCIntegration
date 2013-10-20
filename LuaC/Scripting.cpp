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
MakeWrapper(LuaFunctionSetGodMode, (lua_State *L), (L), Scripting, int);
MakeWrapper(LuaFunction_RegisterEvent, (lua_State *L), (L), Scripting, int);

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

	lua_pushcfunction(luaState, GetWrapper(LuaFunctionSetGodMode, &Scripting::lua_SetGodMode));
	lua_setglobal(luaState, "SetGodMode");

	lua_pushcfunction(luaState, GetWrapper(LuaFunction_RegisterEvent, &Scripting::lua_RegisterEvent));
	lua_setglobal(luaState, "RegisterEvent");

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


int Scripting::lua_SetGodMode(lua_State *L)
{
	this->SetGodeModeEvent.OnEvent(this, EventArgs<int>(0));

	return 0;
}

void Scripting::MouseEvent(int button, int state, int x, int y)
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
						return;
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
						return;
					}
				}
			}
		}
		else if (button == 3 || button == 4)
		{
			for (auto feld : this->m_uiElements)
			{
				UITextField *textField = dynamic_cast<UITextField *>(feld);

				if (textField != nullptr)
				{
					Dimension dim = textField->GetAbsoluteDimensions();

					if (x > dim.x && x < dim.x + dim.width &&
						y > dim.y && y < dim.y + dim.height) // hit test
					{
						//textField->Scroll(button == 3);
						return;
					}
				}
			}
		}
	}
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

