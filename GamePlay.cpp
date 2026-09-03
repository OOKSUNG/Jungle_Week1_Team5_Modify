#include "GamePlay.h"
#include "Result.h"
#include <Windows.h>
#include "SoundManager.h"

GamePlay::GamePlay() 
 	: player()
	, map()
{
	backGround = new Square(FVector{}, FColor{ 0,0,0,0 }, 1.0f, EImage::EI_BackgroundGame, 2);

	player = new UPlayer();
	
	map = new UMap();
	map->randMapGenerator();
	map->renderMap();

	map->getGamePlay(this);

	SoundManager::GetInstance()->PlaySoundEffect(ESoundEffect::ESE_BGM);
}

GamePlay::~GamePlay()
{
	delete map;
	delete player;

	delete backGround;

	SoundManager::GetInstance()->StopSoundEffect(ESoundEffect::ESE_BGM);
}

void GamePlay::update()
{
	float DeltaTime = (float)1 / 60;
	player->SetMap(map);
	player->Update();
	map->DropBallUpdate(DeltaTime);

	if (resultRequested) { // resultRequested

		ActiveScene* activeScene = ActiveScene::getInstance();
		Result* result = new Result(returnCleared, score);
		activeScene->requestChangeScene(result);
	}
}

void GamePlay::requestResult(bool returnCleared)
{
	if (resultRequested) {
		MessageBox(
			nullptr,
			L"Multiple Results are requested",
			L"Result Request Error",
			MB_OK | MB_ICONERROR
		);

		std::exit(EXIT_FAILURE);
	}

	resultRequested = true;
	this->returnCleared = returnCleared;
}
