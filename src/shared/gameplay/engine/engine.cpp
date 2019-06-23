#include "shared/gameplay/engine/engine.h"
#include "shared/gameplay/engine/camera.h"
#include "shared/gameplay/engine/font.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

LPDIRECT3D9										Engine::g_pD3D = NULL;
LPDIRECT3DDEVICE9								Engine::g_pd3dDevice = NULL;
IDirect3DVertexBuffer9*							Engine::g_pVertexBuffer = NULL;
HWND											Engine::g_hWnd = NULL;
bool											Engine::g_bShowDebugOutput = false;

std::unordered_map<std::string, std::string>	Engine::startupParameters;

static char				strDebugText[4096] = { 0 };


void Engine::EngineRender(Engine::RenderCallback pRenderCallback, Engine::RenderCallback pDebugRenderCallback)
{
    if( NULL == g_pd3dDevice )
        return;

	double fps = 0;
	static double avgFps = 0;

	static LARGE_INTEGER timerFrequency;
	static LARGE_INTEGER timerValue;
	static BOOL timerInitialized = FALSE;
	static BOOL timerSuccess = FALSE;

	if(!timerInitialized)
	{
		timerInitialized = TRUE;
		timerSuccess = QueryPerformanceFrequency(&timerFrequency);
		if(timerSuccess)
			QueryPerformanceCounter(&timerValue);
	}
	else if(timerSuccess)
	{
		LARGE_INTEGER newTimerValue;
		QueryPerformanceCounter(&newTimerValue);
		LONGLONG deltaTime = newTimerValue.QuadPart - timerValue.QuadPart;
		if(deltaTime > 0)
			fps = (double)timerFrequency.QuadPart / (double)deltaTime;
		timerValue = newTimerValue;
		avgFps = avgFps * 0.9 + fps * 0.1;
	}

    // Clear the backbuffer to a blue color
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0x80, 0x80, 0x80 ), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
		pRenderCallback();
		if (g_bShowDebugOutput)
		{
			if (pDebugRenderCallback != NULL)
				pDebugRenderCallback();

			char userName[256];
			GetUser(userName, 256);
			size_t len = strnlen_s(strDebugText, sizeof(strDebugText));
			sprintf_s(&strDebugText[len], sizeof(strDebugText) - len, "FPS: %.0f\nUser: %s", (float)fps, userName);
			Font::g_DefaultSmallFont->DrawText(strDebugText, { 0, 0 }, Camera2D::g_pActiveCamera->virtualScreenSize, DT_LEFT | DT_TOP);
		}
		g_pd3dDevice->EndScene();

		strDebugText[0] = 0;
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

HRESULT Engine::EngineInit(HWND hWnd)
{
	srand((unsigned int)time(0));

    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//D3DPRESENT_INTERVAL_DEFAULT;

    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }
	g_hWnd = hWnd;

    // Device state would normally be set here

	//Set vertex shader
	g_pd3dDevice->SetVertexShader(NULL);
	g_pd3dDevice->SetFVF(D3DFVF_TLVERTEX);

	//Create vertex buffer
	g_pd3dDevice->CreateVertexBuffer(sizeof(TLVERTEX) * 4, NULL,
		D3DFVF_TLVERTEX, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	g_pd3dDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(TLVERTEX));

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// Initialize the random number generator
	InitRand();

	// Fill startup parameters by parsing the command line arguments
	SetStartupParameters();

	// Create default camera
	new Camera2D();

	// Create default fonts
	Font::CreateDefaultFonts();

    return S_OK;
}

void Engine::EngineCleanup()
{
    if(g_pd3dDevice)
        g_pd3dDevice->Release();

    if( g_pD3D)
        g_pD3D->Release();

    if(g_pVertexBuffer)
        g_pVertexBuffer->Release();

	if (Camera2D::g_pActiveCamera)
		delete Camera2D::g_pActiveCamera;

	if (Font::g_DefaultSmallFont)
		delete Font::g_DefaultSmallFont;

	if (Font::g_DefaultMediumFont)
		delete Font::g_DefaultMediumFont;

	if (Font::g_DefaultLargeFont)
		delete Font::g_DefaultLargeFont;
}

int Engine::GetUser(char *name, int len)
{
	strncpy_s(name, len, "unknown", 8);
	DWORD nameLength = len;
	if (GetUserNameA(name, &nameLength) != 0)
		return nameLength;
	else
		return -1;
}

void Engine::OutputDebugText(const char * szText)
{
	strcat_s(strDebugText, sizeof(strDebugText), szText);
}

void Engine::SetStartupParameters()
{
	LPWSTR* argsList;
	int argCount;

	argsList = CommandLineToArgvW(GetCommandLine(), &argCount);

	// starting from second element, because first is app path
	for (int index = 1; index < argCount; ++index)
	{
		std::string param;
		std::string value;
		bool assignParam = true;

		LPWSTR arg = argsList[index];

		for (; *arg; ++arg)
		{
			if (assignParam)
			{
				if (*arg == '=' || *arg == ':')
				{
					assignParam = false;
					continue;
				}
				param += (char)*arg;
			}
			else
				value += (char)*arg;
		}

		startupParameters[param] = value;
	}

	LocalFree(argsList);
}

bool Engine::CheckStartupParameter(const std::string & param)
{
	auto it = startupParameters.find(param);
	return (it != startupParameters.end());
}

bool Engine::CheckStartupParameter(const std::string & param, int & value)
{
	auto it = startupParameters.find(param);
	if (it != startupParameters.end())
	{
		value = std::stoi(it->second);
		return true;
	}
	return false;
}

bool Engine::CheckStartupParameter(const std::string & param, float & value)
{
	auto it = startupParameters.find(param);
	if (it != startupParameters.end())
	{
		value = std::stof(it->second);
		return true;
	}
	return false;
}

bool Engine::CheckStartupParameter(const std::string & param, bool & value)
{
	auto it = startupParameters.find(param);
	if (it != startupParameters.end())
	{
		if (it->second.compare("true") == 0)
		{
			value = true;
			return true;
		}
		if (it->second.compare("false") == 0)
		{
			value = false;
			return true;
		}
	}
	return false;
}

bool Engine::CheckStartupParameter(const std::string & param, std::string & value)
{
	auto it = startupParameters.find(param);
	if (it != startupParameters.end())
	{
		value = it->second;
		return true;
	}
	return false;
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE