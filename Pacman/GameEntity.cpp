#include "GameEntity.h"
#include "Drawer.h"
#include "World.h"

GameEntity::GameEntity(const Vector2f& aPosition, Sprite* entitySprite)
:myPosition(aPosition)
,sprite(entitySprite)
,myIdMarkedForDeleteFlag(false)
{
}

GameEntity::~GameEntity(void)
{
}

void GameEntity::Draw(Drawer* aDrawer)
{
	sprite->Draw(aDrawer, (int)myPosition.myX + World::GAME_FIELD_X, (int)myPosition.myY + World::GAME_FIELD_Y);
}