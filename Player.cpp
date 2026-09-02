#include "Player.h"
#include <iostream>

#include "InputManager.h"
#include <random>
#include <ctime>
#include <cmath>

UPlayer::UPlayer()
{
	GenerateNewBall();

	// triangle = new Triangle(FVector{ 0.0f, -0.7f , 0.0f }, FColor{ 0.0f, 1.0f , 0.0f }, 0.1f);
}

UPlayer::~UPlayer()
{
	if (!ball) delete ball;

}

void UPlayer::GenerateNewBall()
{
	if (ball) delete ball;

	pos = BallPosition;
	vel = FVector(0.0f, 0.0f, 0.0f);

	ball = new UBall;
	ball->CircleRenderer->setColor(GenerateBallColor());
	ball->CircleRenderer->setPos(BallPosition);
	ball->CircleRenderer->setRadius(radius);
}

void UPlayer::Update()
{
	if (InputManager::GetInstance()->GetSTATUS()[SPACE])
	{
		Shoot();
	}

	// 정해지는 것이 아니라 더해지게

	pos.x += vel.x;
	pos.y += vel.y;
	pos.z += vel.z;

	ball->CircleRenderer->setPos(pos);

	//ball->CircleRenderer->setPos(FVector{ 0.0f + vel.x, -0.8f + vel.y , 0.0f + vel.z});

	

	//Collision(map->GetBalls());
	WallCollision();

	if (InputManager::GetInstance()->GetSTATUS()[LEFT])
	{
		OutputDebugStringA("!!! My Position is getting Bigger !!!");
		MyPos.x -= 0.1f;
	}
	if (InputManager::GetInstance()->GetSTATUS()[RIGHT])
	{
		OutputDebugStringA("!!! My Position is getting Smaller !!!");
		MyPos.x += 0.1f;
	}

}

void UPlayer::Shoot()
{

	OutputDebugStringA("!!! Shooter !!!");
	FVector vector;
	float dx = MyPos.x - BallPosition.x;
	float dy = MyPos.y - BallPosition.y;

	float dv = sqrt(dx * dx + dy * dy);

	vector.x = dx / dv; 
	vector.y = dy / dv;
	
	vector.x *= ballSpeed; 
	vector.y *= ballSpeed;

	vel = vector;
}

FColor UPlayer::GenerateBallColor()
{
	srand(static_cast<unsigned int>(time(NULL)));
	return color[rand() % 5];
}

void UPlayer::Collision(std::vector<UBall*> balls)
{
	int lastU = 0, lastV = 0;

	for (int i = 0; i < balls.size(); i++)
	{
		// 불리언 변수로 충돌 할지 말지 판단
		/*if (balls[i]->CircleRenderer.color == NullColor)
		{
			continue;
		}*/

		

		FVector pos = balls[i]->CircleRenderer->getPos();

		int dx = pos.x - MyPos.x;
		int dy = pos.y - MyPos.y;

		float dir = sqrt(dx * dx + dy * dy);

		if (dir <= radius)
		{
			bisCollision = true;

			

			// 공 위치 계산 하고 맵에 전달
			int u, v;
			int ansU, ansV;

			IdxToUV(u, v, i);

			// if (balls[i].bisBlink?) 비어있지 않으면
			// {
			//		// 공 파괴 다시 하나 생성
			//		GenerateNewBall();
			//		
			//		// 위치 맵에 전달
			//		//map.addBallandPop(ansU, ansV, ball->CircleRenderer.GetColor())
			// }

			// if 충돌 라인이 11이다.

					// if      + +
						
					// else if - +

					// else if - -

					// else if + -
			

			// if 충돌 라인이 12이다.
					
					// if      + +

					// else if - +

					// else if - -

					// else if + -



			lastU = u;
			lastV = v;

			


		}


	}
}
void IdxToUV(int& u, int& v, int idx)
{
	u = idx / 12; v = idx % 12;
}

int UVToIdx(int u, int v)
{
	return u * 12 + v;
}

void UPlayer::WallCollision()
{
	FVector offset = pos;

	if (offset.x < leftBorder + radius)
	{
		offset.x = leftBorder + radius;
		ball->CircleRenderer->setPos(offset);
		vel.x *= -1.0f;
	}
	if (offset.x > rightBorder - radius)
	{
		offset.x = rightBorder - radius;
		ball->CircleRenderer->setPos(offset);
		vel.x *= -1.0f;
	}
	if (offset.y < topBorder + radius)
	{
		/*offset.y = topBorder + radius;
		ball->CircleRenderer->setPos(offset);
		vel.y *= -1.0f;*/
		GenerateNewBall();
	}
	if (offset.y > bottomBorder - radius)
	{
		offset.y = bottomBorder - radius;
		ball->CircleRenderer->setPos(offset);
		vel.y *= -1.0f;
	}

	
}