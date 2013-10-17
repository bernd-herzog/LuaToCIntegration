#pragma once
#include "uielement.h"
class UIButton :
	public UIElement
{
public:
	UIButton(void);
	~UIButton(void);

	static int lua_Index(lua_State *L);

	virtual void Render();

};

