#include "MovableGameEntity.h"
#include "World.h"

MovableGameEntity::MovableGameEntity(const Vector2f& aPosition, Sprite* entitySprite)
: GameEntity(aPosition, entitySprite)
{
	Respawn(aPosition);
}

MovableGameEntity::~MovableGameEntity(void)
{
}

bool MovableGameEntity::IsAtDestination()
{
	if (myCurrentTileX == myNextTileX && myCurrentTileY == myNextTileY)
	{


		return true;
	}

	return false;
}

void MovableGameEntity::SetNextTile(int anX, int anY)
{
	myNextTileX = anX;
	myNextTileY = anY;
}

void MovableGameEntity::Respawn(const Vector2f& aPosition)
{
	SetPosition(aPosition);
	myCurrentTileX = myNextTileX =  myPosition.myX / World::TILE_SIZE;
	myCurrentTileY = myNextTileY =  myPosition.myY / World::TILE_SIZE;
}

void MovableGameEntity::Move()
{

}