#include "BigDot.h"

BigDot::BigDot(Vector2f aPosition, Sprite* entitySprite)
: StaticGameEntity(aPosition, entitySprite)
{
	sprite->SetFrame("Big_Dot_32.png");
}

BigDot::~BigDot(void)
{
}
