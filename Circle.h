#pragma once

#include "Math.h"
#include "Renderable.h"
#include "Renderer.h"

class Circle : public Renderable {
public:
	Circle(FVector newPos,
		FColor newColor,
		float newRadius,
		EImage image);
	void Render(URenderer& renderer) override;
};