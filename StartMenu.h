#pragma once
#include "Scene.h"
#include "Square.h"
#include "InputManager.h"

enum StartMenuState {
	BackGround,
};



class StartMenu : public Scene {
	Square* backGround = nullptr;
	Square* foreGround = nullptr;

	enum StartMenuState currentState = BackGround;
	enum EKeyStatus lastPressing = NONE;

	int frameCounter = 0;
	const int flashCycle = 30; // frames
	void BreakAllPanels();
public:
	StartMenu();
	~StartMenu();
	void LoadBackGroundState();

	// called by ActiveScene (every frame)
	void update() override;
};