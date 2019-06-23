#ifndef SHARED_GAMEPLAY_ENGINE
#define SHARED_GAMEPLAY_ENGINE

#include "shared/gameplay/gameplay_base.h"

// -------------------------------------------------------------------------------------
// The Engine class provides initializing of the DirectX context & device and handles the per frame rendering
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Engine
{
public:
	static LPDIRECT3D9				g_pD3D;				// Used to create the D3DDevice
	static LPDIRECT3DDEVICE9		g_pd3dDevice;		// Our rendering device
	static IDirect3DVertexBuffer9*	g_pVertexBuffer;	// Vertex buffer (size is 4, type is TLVERTEX)
	static HWND						g_hWnd;				// Handler for the game window
	static bool						g_bShowDebugOutput;

	// Custom vertex contents
	static const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	//Custom vertex structure
	struct TLVERTEX
	{
		float x;
		float y;
		float z;
		float rhw;
		D3DCOLOR colour;
		float u;
		float v;
	};

	// type of function to pass as engine render callback 
	typedef void RenderCallback();

	// Called from the main application to render one D3D frame. This function internally calls pRenderCallback.
	static void EngineRender(RenderCallback pRenderCallback, RenderCallback pDebugRenderCallback = NULL);

	// Called from the main application to initialize the D3D engine
	static HRESULT EngineInit(HWND hWnd);

	// Called from the main application to cleanup D3D engine
	static void EngineCleanup();

	static int GetUser(char *name, int len);

	static void OutputDebugText(const char *szText);

	// Command line parameters management functions
	static bool CheckStartupParameter(const std::string & param);
	static bool CheckStartupParameter(const std::string & param, int & value);
	static bool CheckStartupParameter(const std::string & param, float & value);
	static bool CheckStartupParameter(const std::string & param, bool & value);
	static bool CheckStartupParameter(const std::string & param, std::string & value);

private:
	static void SetStartupParameters();
	static std::unordered_map<std::string, std::string>	startupParameters;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE