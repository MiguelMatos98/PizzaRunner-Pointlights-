#pragma once

#include <Mouse.h>
#include <Keyboard.h>
#include <map>

using namespace DirectX;

class InputManager
{
private:
	std::map<Keyboard::Keys, Keyboard::State> m_keyStates;
	Keyboard::KeyboardStateTracker keyboardTracker;
	Mouse::ButtonStateTracker mouseStateTracker;
public:
	void HandleInput();
	void OnKeyPressed(Keyboard::State& key);
	void OnKeyReleased(Keyboard::State& key);
	void OnMousePressed(Mouse::State& mouse);
	void OnMouseReleased(Mouse::State& mouse);
};

