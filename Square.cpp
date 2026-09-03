#include "Square.h"

Square::Square():Renderable()
{
}

// Todo: Change variable name, radius -> scale
Square::Square(FVector newPos, FColor newColor, float newRadius, EImage image):Renderable(newPos, newColor, newRadius, image)
{
}

Square::Square(FVector newPos, FColor newColor, float newRadius, EImage image, int layer):
	Renderable(newPos, newColor, newRadius, image,layer)
{
}

void Square::Render(URenderer& renderer) {
	renderer.UpdateConstant(pos, radius, color);
	renderer.SetTextureSRV(eImage);
	
	renderer.RenderPrimitive(renderer.VertexBufferSquare, renderer.NumVerticesSquare);
}
