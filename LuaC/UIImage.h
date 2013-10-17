#pragma once
#include "uielement.h"
class UIImage :
	public UIElement
{
public:
	UIImage(void);
	~UIImage(void);


	static int lua_Index(lua_State *L);

	virtual void Render();

};

