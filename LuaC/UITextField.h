#pragma once
#include "uielement.h"
#include "LuaBind.h"

class UITextField :
	public UIElement
{
private:
	std::string m_str;
	bool m_hasFocus;
	int m_cursorPosition;
	bool m_isMultiLine;

public:
	UITextField(void);
	~UITextField(void);

	int LUA_FUNCTION SetText(lua_State *L);
	int LUA_FUNCTION GetText(lua_State *L);
	int LUA_FUNCTION SetIsMultiLine(lua_State *L);

	static void Init(lua_State *L, std::function<void (UIElement *)> func);

	std::string GetText();
	void SetText(std::string str);
	
	bool GetHasFocus();
	void SetHasFocus(bool hasFocus);

	void SetCursorPositionRelative(int to);
	void SetCursorPositionRelativeVertical(int to);
	void SetCursorPosition(int to);
	int GetCursorPosition();

	void SendKey(lua_State *L, unsigned char c);

	POINT GetCusorCoordinates();
	void SetCursorByCoordiantes(int x, int y);

	virtual void Render();

};

