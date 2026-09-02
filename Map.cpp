#include "Map.h"
#include <iostream>

using namespace std;

//#include "GameManager.h"

void UMap::addBallandPop(int ix, int iy, BallColors color)  // Player가 위치 좌표 및 색상 정보를 담은 구조체를 입력하는걸로 변환 해야함
{
	int dx12[6] = { 1, -1, 0, 0, -1, -1 };
	int dy12[6] = { 0, 0, 1, -1, 1, -1 };
	int dx11[6] = { 1, -1, 0, 0, 1, 1 };
	int dy11[6] = { 0, 0, 1, -1, 1, -1 };
	int PopVisited[GameRow][GameCol] = { 0, };
	PopVisited[iy][ix] = 1;

	//addBall
	Balls[iy][ix]->Color = color;
	Balls[iy][ix]->CircleRenderer->setColor(getColorFromEnum(color));

	queue<BallData> q; // 위치 좌표 및 색상 정보를 담은 구조체로 변환 해야함
	queue<BallData> breakable; // 파괴할 공을 담는 큐 이것 또한 위치 좌표 및 색상 정보를 담은 구조체로 변환 해야함

	BallData RootBall;
	RootBall.x = ix;
	RootBall.y = iy;
	RootBall.color = color;
	q.push(RootBall); // Player가 새로 추가한 공을 root
	breakable.push(RootBall);

	//Pop
	for (int i = 0; i < 50; i++) //BFS를 통해 같은 색의 공을 탐색하고, 3개 이상이면 Pop
	{
		BallData currentBall = q.front(); //현재공 좌표 업데이트, 공에서 색 추출 필요

		for (int k = 0; k < 6; k++)
		{
			int nx = 0;
			int ny = 0;
			if ((Lines + currentBall.y) % 2 == 0) 
			{
				nx = currentBall.x + dx12[k];
				ny = currentBall.y + dy12[k];
			}
			else
			{
				nx = currentBall.x + dx11[k];
				ny = currentBall.y + dy11[k];
			}

			if (nx >= 0 && nx < GameCol && ny >= 0 && ny < GameRow)
			{
				if (Balls[ny][nx]->Color == EmptyColor)
				{
					continue;
				}
				if (Balls[ny][nx] != nullptr && Balls[ny][nx]->Color == currentBall.color && !PopVisited[ny][nx])
				{
					PopVisited[ny][nx] = 1;
					BallData newBallData;
					newBallData.x = nx;
					newBallData.y = ny;
					newBallData.color = Balls[ny][nx]->Color;
					q.push(newBallData);
					breakable.push(newBallData);
				}
			}
		}

		q.pop();

		if (q.empty())
		{
			break;
		}
	}

	if (breakable.size() >= 3) // 3개 이상이면 Pop
	{
		while (!breakable.empty())
		{
			BallData currentBall = breakable.front(); // breakable.front()에서 받은 위치 좌표에 속한 공을 Balls 배열에서 nullptr로 바꾸는 걸로 논리 수정해야함
			Balls[currentBall.y][currentBall.x]->CircleRenderer->setColor(getColorFromEnum(EmptyColor)); // Pop
			Balls[currentBall.y][currentBall.x]->Color = EmptyColor;
			breakable.pop();
		}
	}

	//gameOver
	/*
	if (getLines() + getMaxRow() >= GameRow)
	{
		bGameOver = true;
		//GameManager::GameOver()
		UMapClear();
		return;
	}
	*/

	//Drop
	queue<BallData> dq;
	int DropVisited[GameRow][GameCol] = { 0, };

	for(int i = 0; i < GameCol; i++)
	{
		if (Balls[Lines][i]->Color == EmptyColor || Balls[Lines][i] == nullptr)
		{
			continue;
		}
		
		BallData RootBall;
		RootBall.x = i;
		RootBall.y = Lines;
		RootBall.color = Balls[Lines][i]->Color;

		dq.push(RootBall);
		DropVisited[Lines][i] = 1;

		for (int k = 0; k < 50; k++)
		{
			BallData currentBall = dq.front();

			for (int j = 0; j < 6; j++)
			{
				int nx = 0;
				int ny = 0;
				if ((Lines + currentBall.y) % 2 == 0)
				{
					nx = currentBall.x + dx12[j];
					ny = currentBall.y + dy12[j];
				}
				else
				{
					nx = currentBall.x + dx11[j];
					ny = currentBall.y + dy11[j];
				}
				if (nx >= 0 && nx < GameCol && ny >= 0 && ny < GameRow)
				{
					if (Balls[ny][nx] != nullptr && Balls[ny][nx]->Color != EmptyColor && !DropVisited[ny][nx])
					{
						DropVisited[ny][nx] = 1;
						BallData newBallData;
						newBallData.x = nx;
						newBallData.y = ny;
						newBallData.color = Balls[ny][nx]->Color;
						dq.push(newBallData);
					}
				}
			}
			dq.pop();

			if (dq.empty())
			{
				break;
			}
		}
	}

	for (int i = 0; i < GameRow; i++)
	{
		for (int j = 0; j < GameCol; j++)
		{
			if (Balls[i][j] != nullptr && Balls[i][j]->Color != EmptyColor && !DropVisited[i][j])
			{
				Balls[i][j]->CircleRenderer->setColor(getColorFromEnum(EmptyColor));
				Balls[i][j]->Color = EmptyColor;
			}
		}
	}
	
	
	
	//Clear
	/*
	for (int i = 0; i < GameCol; i++)
	{
		if (Balls[0][i] != nullptr)
		{
			break;
		}
		UMapClear();
		return;
	}
	*/
}



void UMap::randMapGenerator() // 현재 3*GameCol 사이즈의 랜덤 맵만 생성하게 만들어져있음
{
	int colorCount = 5; // 색상 개수를 가정한 임시값 -> Enum 도입 후 수정 필요
	int rowCount = 3; // 랜덤으로 생성할 행 개수 -> 레벨 스테이지 도입 시 수정 필요
	
	float BasicRadius = 0.07f;
	FColor BasicColor = { 1.0f, 1.0f, 0.0f, 1.0f };

	for (int i = 0; i < GameRow-1; i++)
	{
		for (int j = 0; j < GameCol; j++)
		{
			UBall* Ball = new UBall();
			if (i < rowCount) {
				int randomColor = rand() % colorCount; // 랜덤 색상 생성
				Ball->CircleRenderer->setColor(getColorFromEnum(static_cast<BallColors>(randomColor)));
				Ball->Color = static_cast<BallColors>(randomColor);
				Ball->CircleRenderer->setRadius(BasicRadius);
			}	
			else 
			{
				Ball->CircleRenderer->setColor(getColorFromEnum(EmptyColor));
				Ball->Color = EmptyColor;
				Ball->CircleRenderer->setRadius(BasicRadius);
			}
			Balls[i][j] = Ball;
		}
	}
}
void UMap::renderMap()
{
	float BasicRadius = 0.07f;
	FVector BasicLocation = { -0.83f , 0.9f, 0.0f };
	FColor BasicColor = { 1.0f, 1.0f, 0.0f, 1.0f };

	for (int i = 0; i < GameRow -1; i++)
	{
		for (int j = 0; j < GameCol; j++)
		{
			FVector AddLocation = { BasicRadius * 2.1f * j, BasicRadius * 1.8f * i, 0.0f };
			FVector FinalLocation = { BasicLocation.x + AddLocation.x, BasicLocation.y - AddLocation.y , BasicLocation.z + AddLocation.z };
			if(Balls[i][j] != nullptr)
				Balls[i][j]->CircleRenderer->setPos(FinalLocation);
			if ((i + Lines) % 2 != 0)
			{
				if (j == GameCol - 1) // 홀수행의 마지막 공은 생성하지 않음
				{
					delete Balls[i][j];
					Balls[i][j] = nullptr;
				}
				if (Balls[i][j] != nullptr)
					Balls[i][j]->CircleRenderer->setPos(FVector{ FinalLocation.x + BasicRadius, FinalLocation.y, FinalLocation.z });
			}
		}
	}
}
void UMap::addLine() 
{
	Lines++;
	float BasicRadius = 0.07f;
	
	for (int i = 0; i < GameCol; i++) // 초반에 게임오버 여부 판단하고 라인추가 실행 후 게임오버 전달
	{
		if (Balls[GameRow - 2][i] != nullptr && Balls[GameRow - 2][i]->Color != EmptyColor)
		{
			bGameOver = true; 
		}
	}

	for (int i = 0; i < GameCol; i++)
	{
		if(Balls[GameRow - 2][i] != nullptr)
		{
			delete Balls[GameRow-2][i];
			Balls[GameRow-2][i] = nullptr;
		}
	}
	for (int i = GameRow - 2; i > 0; i--)
	{
		for (int j = 0; j < GameCol; j++)
		{
			{
				Balls[i][j] = Balls[i - 1][j];
			}
		}
	}

	for (int i = 0; i < GameCol; i++)
	{
		if (Balls[0][i] != nullptr)
		{
			Balls[0][i] = nullptr;
		}
		UBall* Ball = new UBall();
		Ball->CircleRenderer->setColor(getColorFromEnum(WallColor));
		Ball->CircleRenderer->setRadius(BasicRadius);
		Ball->Color = WallColor;
		Balls[0][i] = Ball;
	}
	
	/*
	if (Lines % 2 == 0)
	{
		for (int i = 0; i < GameCol; i++)
		{
			UBall* Ball = new UBall();
			Ball->CircleRenderer->setColor(getColorFromEnum(WallColor));
			Ball->CircleRenderer->setRadius(BasicRadius);
			Ball->Color = WallColor;
			Balls[0][i] = Ball;
		}
	}
	else
	{
		for (int i = 0; i < GameCol; i++)
		{
			UBall* Ball = new UBall();
			Ball->CircleRenderer->setColor(getColorFromEnum(WallColor));
			Ball->CircleRenderer->setRadius(BasicRadius);
			Ball->Color = WallColor;
			Balls[0][i] = Ball;
		}
	}
	*/
	if (bGameOver)
	{
		//GameManager::GameOver
	}
}
void UMap::UMapClear()
{
	for (int i = 0; i < GameRow; i++)
	{
		for (int j = 0; j < GameCol; j++)
		{
			if (Balls[i][j] != nullptr)
			{
				delete Balls[i][j];
				Balls[i][j] = nullptr;
			}
		}
	}
}
int UMap::getLines() // 필요 없어짐
{
	return Lines;
}
int UMap::getMaxRow() // 필요 없어짐 
{
	return 3;
}
bool UMap::isGameOver() //필요 없어짐 
{
	return bGameOver;
}
vector<UBall*> UMap::GetBalls()
{
	vector<UBall*> ballVector;
	for (int i = 0; i < GameRow; i++) //플레이어 라인 제외
	{
		for (int j = 0; j < GameCol; j++)
		{
			if (Balls[i][j] != nullptr)
			{
				ballVector.push_back(Balls[i][j]);
			}
		}
	}
	return ballVector;
}