#include "Cherry.h"

Cherry::Cherry(Vector2f aPosition, Sprite* entitySprite)
: StaticGameEntity(aPosition, entitySprite)
{
	sprite->SetFrame("dot.png");
}

Cherry::~Cherry(void)
{
}
