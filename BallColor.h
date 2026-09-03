#pragma once

#include "Math.h"

#include "Renderer.h"
#include "EImage.h"

enum BallColors
{
	Red, Blue, Green, Yellow, Purple, EmptyColor, WallColor, Lightning, Bomb
};

struct BallData
{
	int x;
	int y;
	BallColors color;
};

FColor getColorFromEnum(BallColors color);
EImage TransformBallColorsToEImage(BallColors color);
