#pragma once

#include <qopenglcontext.h>
#include <qopenglfunctions.h>
#include <qopenglextrafunctions.h>

//#include <winnt.h>
//#include <windef.h>
//#include <wingdi.h>
//#include <winapifamily.h>
//#include <GL.h>
#include "qopenglext.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

#define OPENGL_FUNCTIONS QOpenGLContext::currentContext()->functions()
#define OPENGL_EXTRA_FUNCTIONS QOpenGLContext::currentContext()->extraFunctions()
