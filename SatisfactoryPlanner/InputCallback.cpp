#include "InputCallback.h"



InputCallback::InputCallback(const EInputType inputType, std::function<void(float, float)>&& callback)
	: mInputType(inputType)
{
	if (inputType == EInputType::MOUSE_MOVE)
	{
		mCallbackHandle = InputManager::AddMouseMoveCallback(std::move(callback));
	}
	else if (inputType == EInputType::MOUSE_SCROLL)
	{
		mCallbackHandle = InputManager::AddMouseScrollCallback(std::move(callback));
	}
	else
	{
		throw;
	}
}


InputCallback::InputCallback(InputManager::MouseButtonCallback&& mouseButtonCallback)
	: mInputType(EInputType::MOUSE_BUTTON), mCallbackHandle(InputManager::AddMouseButtonCallback(std::move(mouseButtonCallback)))
{
}


InputCallback::InputCallback(InputManager::KeyCallback&& keyCallback)
	: mInputType(EInputType::KEY_BUTTON), mCallbackHandle(InputManager::AddKeyCallback(std::move(keyCallback)))
{
}


InputCallback::InputCallback(std::function<void()>&& callback, const EMouseButton mouseButton, const EButtonAction action, const EKeyModifier modifier)
	: mInputType(EInputType::MOUSE_BUTTON)
{
	mCallbackHandle = InputManager::AddMouseButtonCallback(
		[callback_ = std::move(callback), mouseButton, action, modifier](const EMouseButton mouseButton_, const EButtonAction action_, const EKeyModifier modifier_)
		{
			if (mouseButton_ == mouseButton &&
				action_ == action &&
				modifier_ == modifier)
			{
				callback_();
			}
		});
}


InputCallback::InputCallback(std::function<void()>&& callback, const EKeyButton keyButton, const EButtonAction action, const EKeyModifier modifier)
	: mInputType(EInputType::KEY_BUTTON)
{
	mCallbackHandle = InputManager::AddKeyCallback(
		[callback_ = std::move(callback), keyButton, action, modifier](const EKeyButton keyButton_, const EButtonAction action_, const EKeyModifier modifier_)
	{
		if (keyButton_ == keyButton &&
			action_ == action &&
			modifier_ == modifier)
		{
			callback_();
		}
	});
}


InputCallback::~InputCallback()
{
	switch (mInputType)
	{
		case EInputType::MOUSE_MOVE:
			InputManager::RemoveMouseMoveCallback(mCallbackHandle);
			break;

		case EInputType::MOUSE_BUTTON:
			InputManager::RemoveMouseButtonCallback(mCallbackHandle);
			break;

		case EInputType::MOUSE_SCROLL:
			InputManager::RemoveMouseScrollCallback(mCallbackHandle);
			break;

		case EInputType::KEY_BUTTON:
			InputManager::RemoveKeyCallback(mCallbackHandle);
			break;
	}
}
