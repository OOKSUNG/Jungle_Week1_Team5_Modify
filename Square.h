#pragma once

#include "Renderable.h"

class Square : public Renderable {
public:
	Square();
	Square(FVector newPos,
		FColor newColor,
		float newRadius,
		EImage image);
	Square(FVector newPos,
		FColor newColor,
		float newRadius,
		EImage image,
		int layer);
	void Render(URenderer& renderer) override;
};