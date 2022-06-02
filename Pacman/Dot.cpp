#include "Dot.h"

Dot::Dot(Vector2f aPosition, Sprite* entitySprite)
: StaticGameEntity(aPosition, entitySprite)
{
	sprite->SetFrame("Small_Dot_32.png");
}

Dot::~Dot(void)
{
}
