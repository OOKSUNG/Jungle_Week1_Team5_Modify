#pragma once

enum EKeyStatus {
	SPACE,
	ENTER,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE,
	FIRSTFRAME,
	KEY_COUNT
};

class InputManager {
public:
	InputManager() = default;
	~InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager& operator = (const InputManager&) = delete;
	static InputManager* GetInstance() 
	{
		if (!_instance)
			_instance = new InputManager();
		return _instance;
	};

	static void DestroyInstance()
	{
		//assert(_instance != nullptr);

		delete _instance;
	};

private:
	static InputManager* _instance;
	bool bKeyStatus[KEY_COUNT] = {};

public:
	bool* GetSTATUS() { return bKeyStatus; }
	bool GetState(EKeyStatus key) { return bKeyStatus[key]; }
	void SetState(EKeyStatus key, bool value) { bKeyStatus[key] = value; }

	void OnKeyDown(EKeyStatus key) { bKeyStatus[key] = true; }
	void OnKeyUP(EKeyStatus key) { bKeyStatus[key] = false; }
};