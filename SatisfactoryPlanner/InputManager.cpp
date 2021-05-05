#include "InputManager.h"



InputCallbackHandle InputManager::AddMouseMoveCallback(MouseMoveCallback&& callback)
{
	const InputCallbackHandle handle = nextCallbackHandle++;
	mouseMoveCallbacks.emplace(handle, std::move(callback));
	return handle;
}


InputCallbackHandle InputManager::AddMouseButtonCallback(MouseButtonCallback&& callback)
{
	const InputCallbackHandle handle = nextCallbackHandle++;
	mouseButtonCallbacks.emplace(handle, std::move(callback));
	return handle;
}


InputCallbackHandle InputManager::AddMouseScrollCallback(MouseScrollCallback&& callback)
{
	const InputCallbackHandle handle = nextCallbackHandle++;
	mouseScrollCallbacks.emplace(handle, std::move(callback));
	return handle;
}


InputCallbackHandle InputManager::AddKeyCallback(KeyCallback&& callback)
{
	const InputCallbackHandle handle = nextCallbackHandle++;
	keyCallbacks.emplace(handle, std::move(callback));
	return handle;
}


void InputManager::RemoveMouseMoveCallback(const InputCallbackHandle callbackHandle) noexcept
{
	mouseMoveCallbacks.erase(callbackHandle);
}


void InputManager::RemoveMouseButtonCallback(const InputCallbackHandle callbackHandle) noexcept
{
	mouseButtonCallbacks.erase(callbackHandle);
}


void InputManager::RemoveMouseScrollCallback(const InputCallbackHandle callbackHandle) noexcept
{
	mouseScrollCallbacks.erase(callbackHandle);
}


void InputManager::RemoveKeyCallback(const InputCallbackHandle callbackHandle) noexcept
{
	keyCallbacks.erase(callbackHandle);
}


bool InputManager::IsKeyDown(const EKeyButton button) noexcept
{
	return keyButtonDownMap[button];
}


bool InputManager::IsMouseDown(const EMouseButton button) noexcept
{
	return mouseButtonDownMap[button];
}


std::pair<float, float> InputManager::GetCursorPosition() noexcept
{
	return mousePosition;
}


void InputManager::_Update()
{
	for (auto [button, isDown] : mouseButtonDownMap)
	{
		if (isDown)
		{
			for (auto& [_, callback] : mouseButtonCallbacks)
			{
				callback(button, EButtonAction::REPEAT, EKeyModifier::NONE);
			}
		}
	}

	for (auto [button, isDown] : keyButtonDownMap)
	{
		if (isDown)
		{
			for (auto& [_, callback] : keyCallbacks)
			{
				callback(button, EButtonAction::REPEAT, EKeyModifier::NONE);
			}
		}
	}
}


void InputManager::_CursorPosCallback(GLFWwindow*, const double x, const double y)
{
	static float prevX = static_cast<float>(x);
	static float prevY = static_cast<float>(y);
	
	const float newX = static_cast<float>(x);
	const float newY = static_cast<float>(y);

	mousePosition.first = newX;
	mousePosition.second = newY;

	const float xDiff = newX - prevX;
	const float yDiff = newY - prevY;

	prevX = newX;
	prevY = newY;
	
	for (auto& [_, callback] : mouseMoveCallbacks)
	{
		if (callback)
		{
			callback(xDiff, yDiff);
		}
	}
}


void InputManager::_MouseButtonCallback(GLFWwindow*, const int button, const int action, const int mod)
{
	const EMouseButton newButton = static_cast<EMouseButton>(button);
	const EButtonAction newAction = static_cast<EButtonAction>(action);
	const EKeyModifier newMod = static_cast<EKeyModifier>(mod);

	mouseButtonDownMap[newButton] = action;  // using that 0 == release, 1 == press
	
	for (auto& [_, callback] : mouseButtonCallbacks)
	{
		if (callback)
		{
			callback(newButton, newAction, newMod);
		}
	}
}


void InputManager::_MouseScrollCallback(GLFWwindow*, const double xAmount, const double yAmount)
{
	const float newX = static_cast<float>(xAmount);
	const float newY = static_cast<float>(yAmount);

	for (auto& [_, callback] : mouseScrollCallbacks)
	{
		if (callback)
		{
			callback(newX, newY);
		}
	}
}


void InputManager::_KeyCallback(GLFWwindow*, const int key, int, const int action, const int mod)
{
	const EKeyButton newKey = static_cast<EKeyButton>(key);
	const EButtonAction newAction = static_cast<EButtonAction>(action);
	const EKeyModifier newMod = static_cast<EKeyModifier>(mod);

	keyButtonDownMap[newKey] = action;  // using that 0 == release, 1 == press
	
	for (auto& [_, callback] : keyCallbacks)
	{
		if (callback)
		{
			callback(newKey, newAction, newMod);
		}
	}
}
