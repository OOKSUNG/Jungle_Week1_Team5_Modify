#include "Circle.h"

// Todo: Change variable name, radius -> scale
Circle::Circle(FVector newPos, FColor newColor, float newRadius, EImage image) 
	: Renderable(newPos, newColor, newRadius, image)
{
}

void Circle::Render(URenderer& renderer) {
	renderer.UpdateConstant(pos, radius, color);
	renderer.SetTextureSRV(eImage);

	renderer.RenderPrimitive(renderer.VertexBufferSquare, renderer.NumVerticesSquare);
}