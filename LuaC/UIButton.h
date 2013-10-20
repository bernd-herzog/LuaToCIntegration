#pragma once
#include "uielement.h"
class UIButton :
	public UIElement
{
	std::string _str;
public:
	UIButton(void);
	~UIButton(void);

	int LUA_FUNCTION SetText(lua_State *L);

	virtual void Render();

	static void Init(lua_State *L, std::function<void (UIElement *)> func);

	void OnClick(lua_State *L);
};

