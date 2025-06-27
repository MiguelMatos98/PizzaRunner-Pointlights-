#include "GameManager.h"
#include <fstream>
#include "ColliderManager.h"

uint8_t	GameManager::playerIndex{ 0 };
vector<GameObject*> GameManager::gameObjects{};
Renderer* GameManager::gameRenderer{ nullptr };

void GameManager::Initialize(Renderer* renderer)
{
	if (renderer != nullptr)
		gameRenderer = renderer;
	else
		gameRenderer = new Renderer();

}

void GameManager::Shutdown()
{
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
		gameObjects[i] = nullptr;
	}
	gameObjects.clear();
}

void GameManager::LoadGame()
{
	// Temporary variables used for the initialization of the following objects
	json transformsFile{};
	float x{}, y{}, z{};
	Transform currentTransform{};
	ObjFileModel* currentMesh{ nullptr };

	// Reading from a file
	ifstream file("Transforms.json");
	if (!file.is_open())
	{
		std::cerr << "Could not open the file!" << std::endl;
		return;
	}

	file >> transformsFile;

	file.close();

	for (size_t i = 0; i < transformsFile.size(); i++)
	{
		// Asignment of temporary floats for clean manipulation of the json values
		x = transformsFile[i]["position"]["x"];
		y = transformsFile[i]["position"]["y"];
		z = transformsFile[i]["position"]["z"];
		currentTransform.position = XMVectorSet(x, y, z, 0.0f);

		x = transformsFile[i]["rotation"]["x"];
		y = transformsFile[i]["rotation"]["y"];
		z = transformsFile[i]["rotation"]["z"];
		currentTransform.rotation = XMVectorSet(x, y, z, 1.0f);

		x = transformsFile[i]["scale"]["x"];
		y = transformsFile[i]["scale"]["y"];
		z = transformsFile[i]["scale"]["z"];
		currentTransform.scale = XMVectorSet(x, y, z, 1.0f);

		if (i == playerIndex)
		{

			// Setting Collision presets 
			ColliderManager::SetColliderAtributes(playerIndex, currentTransform);

			/* Instantiation of player and register it as part of the
			   GameManeger(No mesh is provided because my player deosn't need to be rendered yet) */
			RegisterGameObject(new Player(currentTransform));

			// Setting camera's skybox model 
			currentMesh = new ObjFileModel((char*)"cube.obj", &gameRenderer->GetDevice(), &gameRenderer->GetDeviceContext());
			GetPlayer().GetCamera()->SetSkyboxModel(currentMesh);
		}
		else
		{
			ColliderManager::SetColliderAtributes(i, currentTransform);

			// Instantiation of the game objects and register them as part of the GameManager
			RegisterGameObject(new GameObject(currentTransform, nullptr, currentMesh));
		}
	}

	gameRenderer->pointLights[0] = { {2.5f, 1.f, 0.f}, 1.0f, {0.0f, 1.0f, 0.0f}, 0.f };
	gameRenderer->pointLights[1] = { {0.f, 1.f, 20.f}, 1.0f, {0.0f, 0.0f, 1.0f}, 0.f };

	//delete currentMesh;
	// currentMesh = nullptr;
} 
void GameManager::PlayGame()
{
	gameRenderer->RenderFrame();
}

void GameManager::RegisterGameObject(GameObject* gameObject)
{
	gameObjects.push_back(gameObject);
}

void GameManager::UnregisterGameObject(size_t index)
{
	if (index >= 0 && index < gameObjects.size())
	{
		gameObjects.erase(gameObjects.begin() + index);
	}
	else
	{
		// Throw Error
	}
}

void GameManager::UnregisterAllGameObjects()
{
	gameObjects.clear();
}

LRESULT GameManager::WindowProc(const HWND window_Handle, const UINT message, const WPARAM w_Param, const LPARAM l_Param)
{
	switch (message)
	{
	case WM_INPUT:
		Mouse::ProcessMessage(message, w_Param, l_Param);
		break;
	case WM_KEYDOWN:
		Keyboard::ProcessMessage(message, w_Param, l_Param);
		break;
	case WM_KEYUP:
		Keyboard::ProcessMessage(message, w_Param, l_Param);
		break;
	}
	return DefWindowProc(window_Handle, message, w_Param, l_Param);
}

// Should be on the player
//void GameManager::HandleInput(const HWND& window_Handle, const double delta_Time)
//{
//	// Make 2 fucntion pointers to set to point to a function that will set the movement for the 6 directions the player can move
//	//XMVECTOR forward, right, up;
//
//	
//
//
//
//	
//
//	//if (keyboardTracker.lastState.Escape)
//	//{
//	//	// Send a quit message to the app
//	//	PostQuitMessage(0);
//	//	DestroyWindow(window_Handle); // Note! Destroying the window is not the same as closing the app 
//	//}
//
//	//if (keyboardTracker.pressed.A) 
//	//{
//	//	GetPlayer().SetIsMovingHorizontly(true);
//	//}
//
//	//if (keyboardTracker.pressed.D)
//	//{
//	//	GetPlayer().SetIsMovingHorizontly(true);
//	//}
//}

vector<GameObject*>& GameManager::GetGameObjects()
{
	return gameObjects;
}

Renderer* GameManager::GetRenderer()
{
	return gameRenderer;
}

Player& GameManager::GetPlayer()
{
	// Temporary player created for method quick accessebility 
	static Player* playerPtr = dynamic_cast<Player*>(gameObjects[playerIndex]);
	return *playerPtr;
}
