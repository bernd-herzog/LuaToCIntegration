// stdafx.h : Includedatei f?r Standardsystem-Includedateien
// oder h?ufig verwendete projektspezifische Includedateien,
// die nur in unregelm??igen Abst?nden ge?ndert werden.
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


// TODO: Hier auf zus?tzliche Header, die das Programm erfordert, verweisen.
#define GL_GLEXT_PROTOTYPES

#include <setjmp.h>
//#include "GLee.h"
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>
//#include <GL/wglext.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <lua.hpp>

#define LUA_FUNCTION // markiert Funktionen die aus Lua aufgerufen werden zur besseren lesbarkeit des Codes