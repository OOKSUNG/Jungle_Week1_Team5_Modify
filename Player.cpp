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

	

	//Collision(map->GetBalls(), map->GetLines());
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

std::vector<std::vector<int>> v11 = { { -1, -1 },{ -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 } };
std::vector<std::vector<int>> v12 = { { -1, 0 },{ -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, 0 }, { 1, 1 } };

void UPlayer::Collision(std::vector<UBall*> balls, int lines)
{
	int lastU = 0, lastV = 0;
	FVector zeroPos = balls[0]->CircleRenderer->getPos();

	for (int i = 0; i < balls.size(); i++)
	{
		// 불리언 변수로 충돌 할지 말지 판단
		/*if (balls[i]->CircleRenderer.color == NullColor)
		{
			continue;
		}*/

		FVector ballPos = balls[i]->CircleRenderer->getPos();
		float dir = GetDistance(ballPos, MyPos);

		if (dir <= radius)
		{
			bisCollision = true;

			float minDistance = 0.0f;
			int minU = 0 , minV = 0;

			// 공 위치 계산 하고 맵에 전달
			int u, v;

			IdxToUV(u, v, i);

			// if 충돌 라인이 11일 경우 == u가 홀수
			if (u % 2 != 0)
			{
				for (auto& i : v11)
				{
					int newU = u + i[0];
					int newV = v + i[1];

					float dis = GetDistance(MyPos, UVToPos(zeroPos, newU, newV));

					if (minDistance > dis && balls[UVToIdx(newU, newV)].Color == EmptyColor)
					{
						minDistance = dis;
						minU = newU;
						minV = newV;
					}
				}

				// 공 인덱스 전달
				//map->addball(minU, minV);

			}

			// else 충돌 라인이 12일 경우 == u가 짝수
			else
			{
				for (auto& i : v12)
				{
					int newU = u + i[0];
					int newV = v + i[1];

					float dis = GetDistance(MyPos, UVToPos(zeroPos, newU, newV));

					if (minDistance > dis && balls[UVToIdx(newU, newV)].Color == EmptyColor)
					{
						minDistance = dis;
						minU = newU;
						minV = newV;
					}
				}

				// 공 인덱스 전달
				//map->addball(minU, minV);
			}

			
			

			
		}


	}
}

FVector UPlayer::UVToPos(FVector& zeropos, int u, int v)
{
	FVector anspos;
	anspos.x = zeropos.x + 2 * radius * v;
	anspos.y = zeropos.y + sqrt(3) * radius * u;
	return anspos;
}

void UPlayer::IdxToUV(int& u, int& v, int idx)
{
	u = idx / 12; v = idx % 12;
}

int UPlayer::UVToIdx(int u, int v)
{
	return u * 12 + v;
}

float UPlayer::GetDistance(FVector v1, FVector v2)
{
	int dx = v1.x - v2.x;
	int dy = v1.y - v2.y;

	return sqrt(dx * dx + dy * dy);
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