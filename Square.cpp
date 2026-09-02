#include "Square.h"

Square::Square():Renderable()
{
}

// Todo: Change variable name, radius -> scale
Square::Square(FVector newPos, FColor newColor, float newRadius):Renderable(newPos, newColor, newRadius)
{
}

void Square::Render(URenderer& renderer) {
	renderer.UpdateConstant(pos, radius, color);
	renderer.RenderPrimitive(renderer.VertexBufferSquare, renderer.NumVerticesSquare);
}
