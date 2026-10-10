#include "GraphicsObject.h"

namespace CMPUT350 {

void GraphicsObject::RenderBackground(GameContext *contextrender) { return; }
void GraphicsObject::RenderForeground(GameContext *contextrender) { return; }
Point2D GraphicsObject::GetLocation() const { return Point2D(0, 0); };
float GraphicsObject::GetRotation() const { return 0.0f; };

}  // namespace CMPUT350
