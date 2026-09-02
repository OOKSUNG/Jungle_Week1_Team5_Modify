#pragma once
#include "Math.h"

enum BallColors
{
	Red, Blue, Green, Yellow, Purple, EmptyColor, WallColor
};

FColor getColorFromEnum(BallColors color);


struct BallData
{
	int x;
	int y;
	BallColors color;
};