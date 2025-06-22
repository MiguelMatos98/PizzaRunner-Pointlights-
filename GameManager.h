#pragma once
#include <vector>
#include <iostream>
#include "json.h"
#include "GameObject.h"
#include "Player.h"
#include "Renderer.h"

using namespace std;
using namespace nlohmann;

class GameManager // Singleton
{
private:
	static uint8_t playerIndex; // This is a bool since it'll only take 1-byte to hide a constant magic number (Which is zero in this case)
	static Renderer* gameRenderer;
	static vector<GameObject*> gameObjects;
public:
	static void Initialize(Renderer* renderer = nullptr);
	static void Shutdown();
	static void LoadGame();
	static void PlayGame();
	static void RegisterGameObject(GameObject* gameObject);
	static void UnregisterGameObject(size_t index);
	static void UnregisterAllGameObjects();
	static LRESULT CALLBACK WindowProc(const HWND hWnd, const UINT message, const WPARAM w_Param, const LPARAM l_Param);
	static vector<GameObject*>& GetGameObjects();
	static Renderer* GetRenderer();
	static Player& GetPlayer();
	// OnGameOver() // Event for when game finishes
};

