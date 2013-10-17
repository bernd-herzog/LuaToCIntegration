// stdafx.h : Includedatei für Standardsystem-Includedateien
// oder häufig verwendete projektspezifische Includedateien,
// die nur in unregelmäßigen Abständen geändert werden.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Selten verwendete Teile der Windows-Header nicht einbinden.
// Windows-Headerdateien:
#include <windows.h>

// C RunTime-Headerdateien
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: Hier auf zusätzliche Header, die das Programm erfordert, verweisen.
#include <setjmp.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <lua.hpp>

#define LUA_FUNCTION // markiert Funktionen die aus Lua aufgerufen werden zur besseren lesbarkeit des Codes