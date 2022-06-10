#include "MovableGameEntity.h"
#include "World.h"

MovableGameEntity::MovableGameEntity(const TileCoord& aTile, Sprite::Ptr entitySprite)
: GameEntity(aTile, entitySprite)
{
	Respawn(aTile);
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

void MovableGameEntity::SetNextTile(const TileCoord& tile)
{
	myNextTile = tile;
}

void MovableGameEntity::Respawn(const TileCoord& aTile)
{
	myCurrentTile = myNextTile = aTile;

	SetPosition(Vector2f{myCurrentTile * World::TILE_SIZE});
}

void MovableGameEntity::Move()
{

}