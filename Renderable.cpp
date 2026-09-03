#include "Renderable.h"

std::vector<Renderable*> Renderable::renderables[3];

Renderable::Renderable() {
	layer = 1;
	int slot = getFirstEmpty(1);
	if (slot == -1) {
		slotNo = renderables[1].size();
		renderables[1].push_back(this);
	}
	else {
		slotNo = slot;
		renderables[1][slot] = this;
	}
}

Renderable::Renderable(FVector newPos,
	FColor newColor,
	float newRadius,
	EImage image)
	: eImage(image)
{
	pos.x = newPos.x;
	pos.y = newPos.y;
	pos.z = newPos.z;
	color.R = newColor.R;
	color.G = newColor.G;
	color.B = newColor.B;
	radius = newRadius;
	layer = 1;
	int slot = getFirstEmpty(1);
	if (slot == -1) {
		slotNo = renderables[1].size();
		renderables[1].push_back(this);
	}
	else {
		slotNo = slot;
		renderables[1][slot] = this;
	}
}

Renderable::Renderable(FVector newPos, FColor newColor, float newRadius, EImage eImage, int layer)
	:pos(newPos), color(newColor), radius(newRadius), eImage(eImage), layer(layer)
{
	int slot = getFirstEmpty(layer);
	if (slot == -1) {
		slotNo = renderables[layer].size();
		renderables[layer].push_back(this);
	}
	else {
		slotNo = slot;
		renderables[layer][slot] = this;
	}
}

Renderable::~Renderable() {
	renderables[layer][slotNo] = nullptr;
}

int Renderable::getFirstEmpty(int layer) {
	for (int i = 0; i < renderables[layer].size(); i++) {
		if (renderables[layer][i] == nullptr)
			return i;
	}
	return -1;
}

void Renderable::RenderAll(URenderer& renderer) {
	for (int j = 2; j >= 0; j--)
	for (int i = 0; i < renderables[j].size(); i++) {
		if (renderables[j][i] != nullptr)
		{
			renderables[j][i]->Render(renderer);
		}
	}
}

void Renderable::setPos(FVector newPos) {
	pos.x = newPos.x;
	pos.y = newPos.y;
	pos.z = newPos.z;
}

void Renderable::setColor(FColor newColor) {
	color.R = newColor.R;
	color.G = newColor.G;
	color.B = newColor.B;
}

void Renderable::setRadius(float newRadius) {
	radius = newRadius;
}

void Renderable::setImage(EImage image)
{
	eImage = image;
}


FVector Renderable::getPos() {
	return pos;
}

FColor Renderable::getColor() {
	return color;
}