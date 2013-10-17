#pragma once
#include "uielement.h"
class UILabel :
	public UIElement
{
public:
	UILabel(void);
	~UILabel(void);


	int SetText(lua_State *L);

	static void Init(lua_State *L, std::function<void (UIElement *)> func);


	virtual void Render();

};

