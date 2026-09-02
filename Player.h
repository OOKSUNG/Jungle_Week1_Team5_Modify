#pragma once
#include <vector>
#include "Triangle.h"
#include "Ball.h"
#include "Math.h"
#include "Map.h"

const std::vector<FColor> color = {
{1.0f, 0.0f, 0.0f, 1.0f},
{0.0f, 1.0f, 0.0f, 1.0f},
{0.0f, 0.0f, 1.0f, 1.0f},
{1.0f, 1.0f, 0.0f, 1.0f},
{1.0f, 0.0f, 1.0f, 1.0f}
};

class UPlayer {
public:
	UPlayer();
	~UPlayer();

private:
	UBall* ball;		// FVector Pos
	Triangle* triangle;  // FVector Pos
	bool bisCollision = false;
	FVector vel = { 0.0, 0.0f, 0.0f };
	FVector MyPos = { 0.0f, -0.5f , 0.0f };
	FVector pos = { 0.0f, -0.8f, 0.0f };
	UMap* map;
	bool bcanShoot = true;

public:
	void SetMap(UMap* newmap);
	void Update();
	void Shoot();
	void Collision(std::vector<UBall*> balls, int lines);
	void GenerateNewBall();
	void WallCollision();
	int GenerateBallColor();
	FVector UVToPos(FVector& zeropos, int u, int v);
	void IdxToUV(int& u, int& v, int idx);
	int UVToIdx(int u, int v);
	float GetDistance(FVector v1, FVector v2);

private:
	const float ballSpeed = 0.07f;
	const FVector BallPosition = { 0.0f, -0.8f , 0.0f };
	const float radius = 0.07f;
	const float leftBorder = -0.9f;
	const float rightBorder = 0.9f;
	const float topBorder = -1.0f;
	const float bottomBorder = 1.0f;
	const std::vector<std::vector<int>> v11 = { { 1, 1 },{ -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 } };
	const std::vector<std::vector<int>> v12 = { { -1, 0 },{ -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, 0 }, { -1, -1 } };
};