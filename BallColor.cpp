#include "BallColor.h"

FColor getColorFromEnum(BallColors color)
{
	switch (color)
	{
	case Red:
		return FColor{ 1.0f, 0.0f, 0.0f, 1.0f };
	case Blue:
		return FColor{ 0.0f, 0.0f, 1.0f, 1.0f };
	case Green:
		return FColor{ 0.0f, 1.0f, 0.0f, 1.0f };
	case Yellow:
		return FColor{ 1.0f, 1.0f, 0.0f, 1.0f };
	case Purple:
		return FColor{ 1.0f, 0.0f, 1.0f, 1.0f };
	case EmptyColor:
		return FColor{ 0.0f, 0.0f, 0.0f, 1.0f };
	case WallColor:
		return FColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	}
}

EImage TransformBallColorsToEImage(BallColors color)
{
	switch (color)
	{
	case Red:
		return EI_BallRed;
	case Blue:
		return EI_BallBlue;
	case Green:
		return EI_BallGreen;
	case Yellow:
		return EI_BallYellow;
	case Purple:
		return EI_BallPurple;
	case WallColor:
		return EI_BallWall;
	case EmptyColor:
		return EI_BallTransparent;
	case Lightning:
		return EI_BallLightning;
	case Bomb:
		return EI_BallBomb;
	default:
		assert(false);
	}

	return EI_BallBlack;
}

