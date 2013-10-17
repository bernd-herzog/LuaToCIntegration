#pragma once
#include "uielement.h"
#include "LuaBind.h"

class UITextField :
	public UIElement
{
public:
	UITextField(void);
	~UITextField(void);

	int LUA_FUNCTION SetText(lua_State *L);

	static void Init(lua_State *L, std::function<void (UIElement *)> func);


	

	virtual void Render();

};

