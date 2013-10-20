#pragma once
#include "uielement.h"
class UIImage :
	public UIElement
{
public:
	UIImage(void);
	~UIImage(void);


	static void Init(lua_State *L, std::function<void (UIElement *)> func);

	virtual void Render();

	int LUA_FUNCTION SetTexture(lua_State *L);

};

