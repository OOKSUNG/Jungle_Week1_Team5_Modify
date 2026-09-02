#pragma once
#include "Renderable.h"
#include "Renderer.h"
#include "Math.h"

class Square : public Renderable {
public:
	Square();
	Square(FVector newPos,
		FColor newColor,
		float newRadius,
		EImage image);
	void Render(URenderer& renderer) override;
};