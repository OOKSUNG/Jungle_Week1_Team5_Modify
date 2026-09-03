#pragma once

#include "Circle.h"
#include "BallColor.h"

#define BALLSPEED 0.01f

class UBall //: public UPrimitive
{
public:
	UBall(FVector pos, FColor color, float radius, EImage image)
	{
		TotalNumBalls += 1;

		Location.x = ((float)(rand() % 161 - 80) / 100.0f);
		Location.y = ((float)(rand() % 161 - 80) / 100.0f);

		Velocity.x = ((float)(rand() % 100 - 50)) * BALLSPEED;
		Velocity.y = ((float)(rand() % 100 - 50)) * BALLSPEED;

		Radius = 0.01f + ((float)rand() / RAND_MAX) * 0.1f;
		Mass = Radius * 10.0f;



		CircleRenderer = new Circle(pos, color, radius, image);
	}

	~UBall()
	{
		delete CircleRenderer;

		TotalNumBalls -= 1;
	}

	/*
	float GetRadius() override;
	float GetMass() override;
	FVector GetLocation() override;
	FVector GetVelocity() override;
	void SetLocation(FVector vector) override;
	void SetVelocity(FVector vector) override;
	void Update() override;
	bool CheckCollision(UPrimitive* targetball) override;
	void ResolveCollision(UPrimitive* targetball) override;
	*/

	float GetRadius();
	float GetMass();
	FVector GetLocation();
	FVector GetVelocity();
	void SetLocation(FVector vector);
	void SetVelocity(FVector vector);
	void Update();
	bool CheckCollision(UBall* targetball);
	void ResolveCollision(UBall* targetball);

	float GetDistance(UBall* targetball);
	void Gravity(float dt);
	float Dot(FVector v1, FVector v2);

public:
	FVector Location;
	FVector Velocity;
	float Radius;
	float Mass;
	static int TotalNumBalls;
	static bool bGravity;

	Circle* CircleRenderer;
	BallColors Color;
	int BounceCnt = 0;
};