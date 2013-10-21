#pragma once
#include "uielement.h"
class UILabel :
	public UIElement
{
	std::string _str;

public:
	UILabel(void);
	~UILabel(void);


	int LUA_FUNCTION SetText(lua_State *L);
	int LUA_FUNCTION GetText(lua_State *L);

	static void Init(lua_State *L, std::function<void (UIElement *)> func);

	int m_scroll;
	void Scroll(bool up);


	virtual void Render();

};

