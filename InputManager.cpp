#include "InputManager.h"

void InputManager::HandleInput()
{
	auto keyboardState = Keyboard::Get().GetState();
	auto mouseState = Mouse::Get().GetState();

	keyboardTracker.Update(keyboardState);
	mouseStateTracker.Update(mouseState);

	keyboardState;

	//char hexadecimalKey[4];

	//sprintf

	//for (size_t i = 0; i < keyboardState.OemClear; i++)
	//{
	//	if (keyboardTracker.IsKeyPressed())
	//	{

	//	}
	//}

	//if (Keyboard::)
	//{

	//}

	//if (keyboardTracker.released.A)
	//{

	//}

}

void InputManager::OnKeyPressed(Keyboard::State& key)
{
}

void InputManager::OnKeyReleased(Keyboard::State& key)
{
}

void InputManager::OnMousePressed(Mouse::State& mouse)
{
}

void InputManager::OnMouseReleased(Mouse::State& mouse)
{
}
