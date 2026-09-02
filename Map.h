#pragma once
#include "Ball.h"
#include <stdlib.h>
#include <queue>
#include <vector>

#define GameRow 14
#define GameCol 12

using std::queue;
using std::vector;

class UMap
{
public:
	int Lines;
	bool bGameOver;
	UBall* Balls[GameRow][GameCol] = {};
public:
	UMap()
	{
		Lines = 0;
		bGameOver = false;
	}
	
	~UMap()
	{
		UMapClear();
	}

	void addBallandPop(int ix, int iy, BallColors color); // 발사한 공 구조체로 파라미터 변경해야함
	void randMapGenerator();
	void addLine();
	void UMapClear();
	void renderMap();
	int getMaxRow();
	int getLines(); // return Lines
	bool isGameOver(); // return bGameOver
	vector<UBall*> GetBalls(); // return Balls in vector
};