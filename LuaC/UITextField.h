#pragma once
#include "uielement.h"
#include "LuaBind.h"

class UITextField :
	public UIElement
{
	std::string _str;

public:
	UITextField(void);
	~UITextField(void);

	int LUA_FUNCTION SetText(lua_State *L);
	int LUA_FUNCTION GetText(lua_State *L);

	static void Init(lua_State *L, std::function<void (UIElement *)> func);

	std::string GetText();
	void SetText(std::string str);
	

	virtual void Render();

};

