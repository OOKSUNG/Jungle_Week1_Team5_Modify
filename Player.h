#pragma once
#include <vector>
#include "Triangle.h"
#include "Ball.h"
#include "Math.h"
//#include "Map.h"

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
	//UMap* map;

public:
	void Update();
	void Shoot();
	void AddBall();
	void Collision(std::vector<UBall*> balls);
	void GenerateNewBall();
	void WallCollision();
	FColor GenerateBallColor();

private:
	const float ballSpeed = 0.1f;
	const FVector BallPosition = { 0.0f, -0.8f , 0.0f };
	const float radius = 0.07f;
	const float leftBorder = -1.0f;
	const float rightBorder = 1.0f;
	const float topBorder = -1.0f;
	const float bottomBorder = 1.0f;
};