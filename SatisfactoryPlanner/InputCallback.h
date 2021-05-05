#pragma once
#include "InputManager.h"


enum class EInputType
{
	MOUSE_MOVE,
	MOUSE_BUTTON,
	MOUSE_SCROLL,
	KEY_BUTTON
};


class InputCallback
{
public:
	InputCallback(EInputType inputType, std::function<void(float, float)>&& callback);  // move or scroll
	InputCallback(InputManager::MouseButtonCallback&& mouseButtonCallback);
	InputCallback(InputManager::KeyCallback&& keyCallback);
	InputCallback(std::function<void()>&& callback, EMouseButton mouseButton, EButtonAction action = EButtonAction::RELEASE, EKeyModifier modifier = EKeyModifier::NONE);
	InputCallback(std::function<void()>&& callback, EKeyButton keyButton, EButtonAction action = EButtonAction::RELEASE, EKeyModifier modifier = EKeyModifier::NONE);
	~InputCallback();

	InputCallback(const InputCallback& other) = default;
	InputCallback& operator=(const InputCallback & other) = default;

	InputCallback(InputCallback&& other) noexcept = default;
	InputCallback& operator=(InputCallback&& other) noexcept = default;


	
private:
	EInputType mInputType;
	InputCallbackHandle mCallbackHandle;
};
