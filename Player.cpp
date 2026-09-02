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
	if (ball) delete ball;
	if (ball) delete map;
}

void UPlayer::SetMap(UMap* newmap)
{
	map = newmap;
}

void UPlayer::GenerateNewBall()
{
	if (ball) delete ball;

	pos = BallPosition;
	vel = FVector(0.0f, 0.0f, 0.0f);



	ball = new UBall;
	BallColors newColor =static_cast<BallColors>(GenerateBallColor());
	ball->Color = newColor;
	ball->CircleRenderer->setColor(getColorFromEnum(newColor));
	
	ball->CircleRenderer->setPos(BallPosition);
	ball->CircleRenderer->setRadius(radius);
}

void UPlayer::Update()
{
	if (InputManager::GetInstance()->GetSTATUS()[SPACE] && bcanShoot)
	{
		Shoot();
		bcanShoot = false;
	}

	// 정해지는 것이 아니라 더해지게

	pos.x += vel.x;
	pos.y += vel.y;
	pos.z += vel.z;

	ball->CircleRenderer->setPos(pos);

	//ball->CircleRenderer->setPos(FVector{ 0.0f + vel.x, -0.8f + vel.y , 0.0f + vel.z});

	Collision(map->GetBalls(), map->getLines());
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
	if (MyPos.x >= 0.5f) MyPos.x = 0.5f;
	if (MyPos.x <= -0.5f) MyPos.x = -0.5f;
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

int UPlayer::GenerateBallColor()
{
	srand(static_cast<unsigned int>(time(NULL)));
	//return getColorFromEnum(static_cast<BallColors>(rand() % 5));

	return rand() % 5;
}

void UPlayer::Collision(std::vector<UBall*> balls, int lines)
{
	int lastU = 0, lastV = 0;
	FVector zeroPos = balls[0]->CircleRenderer->getPos();

	for (int i = 0; i < balls.size(); i++)
	{
		if (balls[i]->Color == EmptyColor)
		{
			continue;
		}

		FVector ballPos = balls[i]->CircleRenderer->getPos();
		float dir = GetDistance(ballPos, pos);

		if (dir <= 2 * radius)
		{
			if (balls[i]->Color == EmptyColor)
			{
				continue;
			}


			bisCollision = true;
			OutputDebugStringA("!!! Collision Detection !!!\n");

			float minDistance = 1000.0f;
			int minU = 0;
			int minV = 0;

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

					if (newU >= 13) newU = 13;
					if (newV >= 11) newU = 11;
					if (newU <= 0) newU = 0;
					if (newV <= 0) newU = 0;

					// balls[i]->CircleRenderer->getPos();
					float dis = GetDistance(pos, balls[UVToIdx(newU, newV)]->CircleRenderer->getPos());

					if (minDistance > dis && balls[UVToIdx(newU, newV)]->Color == EmptyColor)
					{
						minDistance = dis;
						minU = newU;
						minV = newV;
					}
				}

				// 공 인덱스 전달
				if (minDistance < 10.0f)
				{
					map->addBallandPop(minU, minV, ball->Color);
					GenerateNewBall();
					return;
				}


			}

			// else 충돌 라인이 12일 경우 == u가 짝수
			else
			{
				for (auto& i : v12)
				{
					int newU = u + i[0];
					int newV = v + i[1];

					if (newU >= 13) newU = 13;
					if (newV >= 11) newU = 11;
					if (newU <= 0) newU = 0;
					if (newV <= 0) newU = 0;

					float dis = GetDistance(pos, balls[UVToIdx(newU, newV)]->CircleRenderer->getPos());

					if (minDistance > dis && balls[UVToIdx(newU, newV)]->Color == EmptyColor)
					{
						minDistance = dis;
						minU = newU;
						minV = newV;
					}
				}

				// 공 인덱스 전달
				if (minDistance < 10.0f)
				{
					map->addBallandPop(minV, minU, ball->Color);
					GenerateNewBall();
					return;
				}
			}
		}
		bcanShoot = true;
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

	//u = idx / 23;
	//v = idx % 23;

	//// lines 홀수 이냐 짝수이냐 에 따라서 12 또는 11로 변경한다.

	//if (v > 11) 
	//{
	//	u++;
	//	v = u + v - 12 ;
	//}
	//else if (v <= 11)
	//{
	//	v += u;
	//}

}

int UPlayer::UVToIdx(int u, int v)
{
	return u * 12 + v;
}

float UPlayer::GetDistance(FVector v1, FVector v2)
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;

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