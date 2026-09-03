#pragma once

#include <vector>

#include "Renderer.h"
#include "Math.h"

#include "EImage.h"

class Renderable {
private:
	static std::vector<Renderable*> renderables[3];
	int slotNo;
protected:
	FVector pos;
	FColor color;
	float radius;
	EImage eImage;
	int layer;
public:
	Renderable();
	~Renderable();

	Renderable(FVector newPos,
		FColor newColor,
		float newRadius,
		EImage eImage);
	Renderable(FVector newPos,
		FColor newColor,
		float newRadius,
		EImage eImage,
		int layer);

	static void RenderAll(URenderer& renderer);

	void setPos(FVector newPos);
	void setColor(FColor newColor);
	void setRadius(float newRadius);
	void setImage(EImage image);

	FVector getPos();
	FColor getColor();
	virtual void Render(URenderer& renderer) = 0;
private:
	int getFirstEmpty(int layer);
};