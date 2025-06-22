#include "Windows.h"
#include "Transform.h"
#include "GameObject.h"
#include "GameManager.h"
#include "ColliderManager.h"
#include <iostream>
#include <vector>
#include <chrono>

//#include <exception>
//#include <array>
//#include <d3d11.h>
//#include <DirectXColors.h>
//#include "ReadData.h"
//#include <d3d11shader.h>
//#include <d3dcompiler.h>
//#include <dxgi.h>
//#include <dxgidebug.h>
//#include <WICTextureLoader.h>
//#include "text2D.h"
//#include <Mouse.h>
//#include <Keyboard.h>
//#include "objfilemodel.h"
//#include <DDSTextureLoader.h>

//static std::vector<std::exception_ptr> exceptions = {};

//LARGE_INTEGER clockFrequency;
//
//LARGE_INTEGER clockStart;
//
//double GetTime()
//{
//	LARGE_INTEGER now_time;
//	QueryPerformanceCounter(&now_time);
//	return (double)now_time.QuadPart * clockFrequency.QuadPart;
//}

void OpenConsole()
{
	if (AllocConsole())
	{
		// -----------------------------Do this instead bellow----------------------------------- 
		// Redirecting input, output and errors to allocated console
		FILE* fp = nullptr;
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
		std::ios::sync_with_stdio(true);
		//for (size_t i = 0; i < GameManager::GetInstance().GetGameObjects().size(); i++)
		//{
		//	//std::string inSentence = static_cast<std::string>();

		//	std::cout << "{ " << DirectX::XMVectorGetX(GameManager::GetInstance().GetGameObjects()[i]->GetTransform().scale) << ", " << DirectX::XMVectorGetY(GameManager::GetInstance().GetGameObjects()[i]->GetTransform().scale) << ", " << DirectX::XMVectorGetZ(GameManager::GetInstance().GetGameObjects()[i]->GetTransform().scale) << " }" << std::endl;

		//}
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	// Declaration of time variables
	auto previousTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point currentTime;
	std::chrono::duration<double> deltaTime;

	Mouse GameMouse;
	Keyboard keyboard;
	//// Used to hold windows events message
	MSG msg = {};
	//try
	//{
	//	// Here's the code for creating objects (Such as the Prism and the Player), 
	//	// that will have to have a collider
	//}
	//catch (const std::invalid_argument& e)
	//{
	//	std::cout << "Caught an invalid_argument exception: " << e.what() << std::endl;
	//}


	GameManager::Initialize();
	
	ColliderManager::Initialize();

	if (FAILED(GameManager::GetRenderer()->InitializeWindow(hInstance, nCmdShow)))
	{
		MessageBox(NULL, L"Failed to create window", L"Critical Error!", MB_ICONERROR | MB_OK);
	}


	if (FAILED(GameManager::GetRenderer()->InitializeD3D()))
	{
		MessageBox(NULL, L"Failed to create swap chain device", L"Critical Error!", MB_ICONERROR | MB_OK);
	}

	OpenConsole();

	GameManager::LoadGame();

	GameManager::GetRenderer()->InitializeGraphics();

	Mouse::Get().SetWindow(GameManager::GetRenderer()->GetWindowHandler());
	Mouse::Get().SetMode(Mouse::MODE_RELATIVE);

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Translate certain messages into correct format, namely key presses 
			TranslateMessage(&msg);

			// Send the message to the WindowProc function
			DispatchMessage(&msg);

			// Break out of the loop if a quit message is detected
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			currentTime = std::chrono::steady_clock::now();
			deltaTime = currentTime - previousTime;

			/*GameManager::HandleInput(GameManager::GetRenderer()->GetWindowHandler(), deltaTime.count());*/
			ColliderManager::CheckForCollision();
			GameManager::GetPlayer().HandleInput(deltaTime.count());
			GameManager::GetPlayer().Update(deltaTime.count());
			GameManager::GetRenderer()->RenderFrame();

			previousTime = currentTime;
		}
	}
	GameManager::UnregisterAllGameObjects();
	GameManager::GetRenderer()->Clean3D();
	ColliderManager::Shutdown();
	GameManager::Shutdown();

	return 0;
}