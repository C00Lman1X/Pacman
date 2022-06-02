#include "GameEntity.h"
#include "Drawer.h"

GameEntity::GameEntity(const Vector2f& aPosition, Sprite* entitySprite)
:myPosition(aPosition)
,sprite(entitySprite)
,myIdMarkedForDeleteFlag(false)
{
}

GameEntity::~GameEntity(void)
{
}

bool GameEntity::Intersect(GameEntity* aGameEntity)
{

	return false;	
}

void GameEntity::Draw(Drawer* aDrawer)
{
	sprite->Draw(aDrawer, (int)myPosition.myX + 220, (int)myPosition.myY + 60);
}