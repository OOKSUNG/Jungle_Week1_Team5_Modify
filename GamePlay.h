#pragma once
#include "Scene.h"
#include "Square.h"
#include "Player.h"
#include "Map.h"

class GamePlay : public Scene {
	Square* backGround = nullptr;

	UMap* map = nullptr;
	UPlayer* player = nullptr;

	bool resultRequested = false;
	bool returnCleared = false;


public:
	GamePlay();
	virtual ~GamePlay() override;

	int score = 0;

	// called by ActiveScene (every frame)
	void update() override;
	// called by objects
	void requestResult(bool returnCleared);
};