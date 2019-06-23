#ifndef SHARED_GAMEPLAY_BASE
#define SHARED_GAMEPLAY_BASE

#include "shared/shared_base.h"

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "shellapi.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>  
#include <crtdbg.h>

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <Xinput.h>

#include <Mmsystem.h>

#define SAFE_DELETE(ptr) delete ptr; ptr = nullptr;

BEGIN_SHARED_NAMESPACE

#define BEGIN_GAMEPLAY_NAMESPACE namespace gameplay {
#define END_GAMEPLAY_NAMESPACE }

BEGIN_GAMEPLAY_NAMESPACE

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_BASE