#include "GamePlay.h"
#include "GamePlayObjects.h"

GameManager::~GameManager()
{
}

void GameManager::setGamePlay(GamePlay* gameplay)
{
	this->gameplay = gameplay;
}

void GameManager::update()
{
	// 게임매니저가 게임오버를 감지하는 경우의 예시 코드
	// TODO
	if (false)
	{
		gameplay->requestResult(false, score);
	}
}

void Map::setGamePlay(GamePlay* gameplay)
{
	this->gameplay = gameplay;
}

void Player::setMap(Map* map)
{
	this->map = map;
}
