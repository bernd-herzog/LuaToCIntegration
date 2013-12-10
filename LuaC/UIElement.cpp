#include "stdafx.h"
#include "UIElement.h"
#include "CppWrapper.h"

//MakeWrapper(LuaFunction_UIElement_SetPosition, (lua_State *L), (L), UIElement, int);
//MakeWrapper(LuaFunction_UIElement_SetSize, (lua_State *L), (L), UIElement, int);
//MakeWrapper(LuaFunction_UIElement_Show, (lua_State *L), (L), UIElement, int);
//MakeWrapper(LuaFunction_UIElement_RegisterEvent, (lua_State *L), (L), UIElement, int);


UIElement::UIElement(void)
	:_visible(false), m_luaEventHandlerId(-1), m_anchorOtherElement(nullptr)
{
}


UIElement::~UIElement(void)
{
}

void UIElement::Init(lua_State *L)
{
	typedef LuaBinding<UIElement> LuaBinding;

	static bool once = false;
	if (!once)
	{
		once = true;
		LuaBinding::registerFunc<&UIElement::lua_SetEventHandler>("SetEventHandler");
		LuaBinding::registerFunc<&UIElement::lua_RegisterEvent>("RegisterEvent");
		LuaBinding::registerFunc<&UIElement::lua_SetPosition>("SetPosition");
		LuaBinding::registerFunc<&UIElement::lua_SetSize>("SetSize");
		LuaBinding::registerFunc<&UIElement::lua_SetAnchor>("SetAnchor");
		LuaBinding::registerFunc<&UIElement::lua_Show>("Show");
		LuaBinding::registerFunc<&UIElement::lua_Hide>("Hide");
	}
}

int LUA_FUNCTION UIElement::lua_SetPosition(lua_State *L)
{
	//int n = lua_gettop(L); // number of arguments

	int x = luaL_checkint(L, 2);
	int y = luaL_checkint(L, 3);

	this->_dimSelf.x = x;
	this->_dimSelf.y = y;

	return 0;
}

int LUA_FUNCTION UIElement::lua_SetSize(lua_State *L)
{
	//int n = lua_gettop(L); // number of arguments

	int width = luaL_checkint(L, 2);
	int height = luaL_checkint(L, 3);

	this->_dimSelf.width = width;
	this->_dimSelf.height = height;

	return 0;
}

int LUA_FUNCTION UIElement::lua_Show(lua_State *L)
{
	_visible = true;
	return 0;
}

int LUA_FUNCTION UIElement::lua_Hide(lua_State *L)
{
	_visible = false;
	return 0;
}

int LUA_FUNCTION UIElement::lua_SetAnchor(lua_State *L)
{
	const char *anchorMe = luaL_checkstring(L, 2);
	const char *anchorOther = luaL_checkstring(L, 3);
	if (lua_isuserdata(L, 4))
	{
		UIElement **obj = static_cast<UIElement **>(lua_touserdata(L, 4));
		m_anchorOtherElement = *obj;
	}
	else
	{
		m_anchorOtherElement = nullptr;
	}

	if (strcmp(anchorMe, "ANCHOR_BOTTOM_LEFT") == 0)
	{
		_anchorSelf = AnchorPosition::BOTTOM_LEFT;
	}
	else if (strcmp(anchorMe, "ANCHOR_BOTTOM_RIGHT") == 0)
	{
		_anchorSelf = AnchorPosition::BOTTOM_RIGHT;
	}
	else if (strcmp(anchorMe, "ANCHOR_TOP_LEFT") == 0)
	{
		_anchorSelf = AnchorPosition::TOP_LEFT;
	}
	else if (strcmp(anchorMe, "ANCHOR_TOP_RIGHT") == 0)
	{
		_anchorSelf = AnchorPosition::TOP_RIGHT;
	}

	if (strcmp(anchorOther, "ANCHOR_BOTTOM_LEFT") == 0)
	{
		_anchorOther = AnchorPosition::BOTTOM_LEFT;
	}
	else if (strcmp(anchorOther, "ANCHOR_BOTTOM_RIGHT") == 0)
	{
		_anchorOther = AnchorPosition::BOTTOM_RIGHT;
	}
	else if (strcmp(anchorOther, "ANCHOR_TOP_LEFT") == 0)
	{
		_anchorOther = AnchorPosition::TOP_LEFT;
	}
	else if (strcmp(anchorOther, "ANCHOR_TOP_RIGHT") == 0)
	{
		_anchorOther = AnchorPosition::TOP_RIGHT;
	}

	return 0;
}

int LUA_FUNCTION UIElement::lua_SetEventHandler(lua_State *L)//UIElement, func
{
	if (lua_isfunction(L, 2) == 1)
	{
		lua_pushvalue(L, 2);
		m_luaEventHandlerId = luaL_ref(L, LUA_REGISTRYINDEX);
		return 0;
	}

	return 0;
}

void UIElement::SetRefID(int refId)
{
	this->m_luaUserDataRefId = refId;
}

int LUA_FUNCTION UIElement::lua_RegisterEvent(lua_State *L) //UIElement, string 
{
	std::string eventName = luaL_checkstring(L, 2);

	this->m_registeredEvents.insert(eventName);

	return 0;
}

void UIElement::SetParentDimensions(POINT position, SIZE size)
{
	this->_dimOther.x = position.x;
	this->_dimOther.y = position.y;
	this->_dimOther.width = size.cx;
	this->_dimOther.height = size.cy;
}



Dimension UIElement::GetAbsoluteDimensions()
{
	Dimension ret = this->_dimSelf;

	if (m_anchorOtherElement != nullptr)
	{
		Dimension other = m_anchorOtherElement->GetAbsoluteDimensions();

		this->_dimOther = other;
	}

	ret.x += this->_dimOther.x;
	ret.y += this->_dimOther.y;

	if (this->_anchorSelf == AnchorPosition::BOTTOM_RIGHT || this->_anchorSelf == AnchorPosition::TOP_RIGHT)
	{
		ret.x -= ret.width;
	}

	if (this->_anchorSelf == AnchorPosition::TOP_LEFT || this->_anchorSelf == AnchorPosition::TOP_RIGHT)
	{
		ret.y -= ret.height;
	}

	if (this->_anchorOther == AnchorPosition::BOTTOM_RIGHT || this->_anchorOther == AnchorPosition::TOP_RIGHT)
	{
		ret.x += this->_dimOther.width;
	}

	if (this->_anchorOther == AnchorPosition::TOP_LEFT || this->_anchorOther == AnchorPosition::TOP_RIGHT)
	{
		ret.y += this->_dimOther.height;
	}


	return ret;
}
