#include "Triangle.h"

Triangle::Triangle(FVector newPos,
	FColor newColor,
	float newRadius,
	EImage image):Renderable(newPos, newColor, newRadius, image) {

}

void Triangle::Render(URenderer& renderer) {
	renderer.UpdateConstant(pos, radius, color);
	renderer.RenderPrimitive(renderer.VertexBufferTriangle, renderer.NumVerticesTriangle);
}