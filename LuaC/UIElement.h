#pragma once

#include "LuaBind.h"
#include "dimension.h"
#include <set>
#include <string>

enum class AnchorPosition
{
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};

class UIElement
{
protected:
	bool _visible;

	Dimension _dimSelf;
	Dimension _dimOther;

	UIElement *m_anchorOtherElement;
	AnchorPosition _anchorSelf;
	AnchorPosition _anchorOther;

	int m_luaUserDataRefId;
	int m_luaEventHandlerId;
	std::set<std::string> m_registeredEvents;

public:
	UIElement(void);
	~UIElement(void);

	bool IsVisible(){return this->_visible;}

	void SetRefID(int);
	void SetParentDimensions(POINT position, SIZE size);
	Dimension GetAbsoluteDimensions();

	UIElement *GetParent(){return this->m_anchorOtherElement;}

	//static int lua_Index(lua_State *L);

	int LUA_FUNCTION lua_SetPosition(lua_State *L);
	int LUA_FUNCTION lua_SetSize(lua_State *L);
	int LUA_FUNCTION lua_SetAnchor(lua_State *L);
	int LUA_FUNCTION lua_RegisterEvent(lua_State *L);
	int LUA_FUNCTION lua_SetEventHandler(lua_State *L);

	int LUA_FUNCTION lua_Show(lua_State *L);
	int LUA_FUNCTION lua_Hide(lua_State *L);

	static void Init(lua_State *L);

	typedef UIElement Base;

	virtual void Render() = 0;
};

