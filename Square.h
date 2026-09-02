#pragma once
#include "Renderable.h"
#include "Renderer.h"
#include "Math.h"

class Square : Renderable {
public:
	Square();
	Square(FVector newPos,
		FColor newColor,
		float newRadius);
	void Render(URenderer& renderer) override;
};