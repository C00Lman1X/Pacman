#include "MovableGameEntity.h"
#include "World.h"

MovableGameEntity::MovableGameEntity(const Vector2f& aPosition, Sprite* entitySprite)
: GameEntity(aPosition, entitySprite)
{
	Vector2f tile = aPosition;
	tile /= World::TILE_SIZE;
	Respawn(tile);
}

MovableGameEntity::~MovableGameEntity(void)
{
}

bool MovableGameEntity::IsAtNextTile()
{
	if (myCurrentTile == myNextTile)
	{
		return true;
	}

	return false;
}

void MovableGameEntity::SetNextTile(const Vector2f& tile)
{
	myNextTile = tile;
}

void MovableGameEntity::Respawn(const Vector2f& aTile)
{
	myCurrentTile = myNextTile = aTile;

	SetPosition(Vector2f{myCurrentTile * World::TILE_SIZE});
}

void MovableGameEntity::Move()
{

}