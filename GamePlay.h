#pragma once
#include "Scene.h"
#include "Square.h"
#include "GamePlayObjects.h"


class GamePlay : public Scene {
	Square* backGround;

	Map map;
	Player player;
	GameManager gameManager;

	bool resultRequested = false;
	bool returnCleared = false;
	int returnScore = 0;

public:
	GamePlay();
	~GamePlay();

	// called by ActiveScene (every frame)
	void update() override;
	// called by objects
	void requestResult(bool returnCleared, int returnScore);
};