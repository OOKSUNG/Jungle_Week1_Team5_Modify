#pragma once
#include "Square.h"

class GamePlay;

// 임시 클래스
class Map {
private:
	GamePlay* gameplay;
public:

	void setGamePlay(GamePlay* gameplay);
};

class Player {
	Map* map;
public:
	void setMap(Map* map);
};

class GameManager {
private:
	GamePlay* gameplay;
public:
	int score = 0;

	~GameManager();
	void setGamePlay(GamePlay* gameplay);
	void update();
};