#include "GamePlay.h"
#include "Result.h"
#include <Windows.h>

GamePlay::GamePlay() :
	player(), map(), gameManager()
{
	backGround = new Square(FVector{}, FColor{ 0,0,0,0 }, 1.0f, EImage::EI_BackgroundGame);
	map.setGamePlay(this);
	gameManager.setGamePlay(this);
}

GamePlay::~GamePlay()
{
	delete backGround;
}

void GamePlay::update()
{
	gameManager.update();

	if (true) { // resultRequested
		ActiveScene* activeScene = ActiveScene::getInstance();
		Result* result = new Result(returnCleared, returnScore);
		activeScene->requestChangeScene(result);
	}
}

void GamePlay::requestResult(bool returnCleared, int returnScore)
{
	/*
	게임 로직 견고성 체크 구문 : 만약 여기서 에러가 발생한다면 게임 로직 재검토 필요 가능성 있음
	상세히 말하자면, 이 구문에 걸리는 경우는 한 프레임에 여러 결과창 리퀘스트가 들어오는 경우인데
	여러 결과 리퀘스트가 주는 게임클리어 여부와 결과점수가 전부 동일하면
	한 프레임에 여러 리퀘스트가 들어와도 견고하다고 판단할 수 있고, 이 구문을 없애도 무방
	만약 그렇지 못하다면 게임 로직을 더 견고하게 구축해야 함
	*/
	if (resultRequested) {
		MessageBox(
			nullptr,
			L"Multiple Results are requested",
			L"Result Request Error",
			MB_OK | MB_ICONERROR
		);

		std::exit(EXIT_FAILURE);
	}

	// 여기서부터는 함수 동작
	resultRequested = true;
	this->returnCleared = returnCleared;
	this->returnScore = returnScore;
}
