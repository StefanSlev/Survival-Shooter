#include "game_client_logic/gcl.h"

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool ProcessMessageLoop(double endTime)
{
	bool bQuit = false;
	MSG msg;
	LARGE_INTEGER time;
	int minProcessedMessages = 3;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		QueryPerformanceCounter(&time);
		if (msg.message == WM_QUIT)
			bQuit = true;
		else
		{
			minProcessedMessages--;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (minProcessedMessages < 0 && time.QuadPart >= endTime)
			break;
	}
	return bQuit;
}

INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Get the desktop window size
	RECT desktop;
	HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"SurvivalShooter", NULL
	};
	RegisterClassEx(&wc);

	constexpr int offsetX = shared::gameplay::WINDOW_OFFSET_X;
	constexpr int offsetY = shared::gameplay::WINDOW_OFFSET_Y;

	// Create window
	HWND hWnd = CreateWindow(L"SurvivalShooter", L"Survival Shooter",
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, offsetX, offsetY, desktop.right - 2 * offsetX, desktop.bottom - 2 * offsetY,
		NULL, NULL, wc.hInstance, NULL);

	if (SUCCEEDED(shared::gameplay::Engine::EngineInit(hWnd)) &&
		hermes::communication::Initialize())
	{
		std::string client_address;
		std::string server_address;
		int server_port;

		bool hasClientAddress = shared::gameplay::Engine::CheckStartupParameter("client_address", client_address);
		bool hasServerAddress = shared::gameplay::Engine::CheckStartupParameter("server_address", server_address);
		bool hasServerPort = shared::gameplay::Engine::CheckStartupParameter("server_port", server_port);

		if (hasClientAddress && hasServerAddress && hasServerPort)
		{
			// create game client
			yojimbo::Address clientAddress(client_address.c_str());
			hermes::communication::CommunicationAdapter communicationAdapter;

			auto gameClientLogic = std::make_unique<gcl::GameClientLogic>(clientAddress, communicationAdapter);

			uint64_t clientId = 0;
			yojimbo::random_bytes((uint8_t*)&clientId, 8);

			uint8_t privateKey[yojimbo::KeyBytes];
			memset(privateKey, 0, yojimbo::KeyBytes);

			yojimbo::Address serverAddress(server_address.c_str(), server_port);
			gameClientLogic->InsecureConnect(privateKey, clientId, serverAddress);

			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			// Enter the message loop
			LARGE_INTEGER Frequency;
			QueryPerformanceFrequency(&Frequency);

			LARGE_INTEGER StartingTime, EndingTime, TimeToFrame;
			TimeToFrame.QuadPart = 0;
			QueryPerformanceCounter(&StartingTime);

			constexpr double frameTime = shared::gameplay::FRAME_TIME;

			while (ProcessMessageLoop(StartingTime.QuadPart + frameTime * Frequency.QuadPart - TimeToFrame.QuadPart) != true)
			{
				if (TimeToFrame.QuadPart >= frameTime * Frequency.QuadPart)
				{
					// Process frame operations
					double dt = ((double)TimeToFrame.QuadPart / (double)Frequency.QuadPart);

					if (dt > 0.1) dt = 0.1; // for debug operations, we limit dt to 1/10 sec

					shared::gameplay::Input::UpdateInputState();

					// Run a game frame
					if (!gameClientLogic->RunFrame(dt))
						break;

					TimeToFrame.QuadPart = 0;
				}

				QueryPerformanceCounter(&EndingTime);
				TimeToFrame.QuadPart += (EndingTime.QuadPart - StartingTime.QuadPart);
				StartingTime.QuadPart = EndingTime.QuadPart;
			}

			gameClientLogic->Disconnect();
		}

		hermes::communication::Shutdown();
		shared::gameplay::Engine::EngineCleanup();
	}

	UnregisterClass(L"SurvivalShooter", wc.hInstance);
	return 0;
}