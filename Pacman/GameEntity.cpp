#include "GameEntity.h"
#include "Drawer.h"
#include "World.h"

GameEntity::GameEntity(const TileCoord& aTile, Sprite::Ptr entitySprite)
	: myCurrentTile(aTile)
	, myPosition(aTile * World::TILE_SIZE)
	, sprite(entitySprite)
{
}

void GameEntity::Draw(Drawer::Ptr aDrawer)
{
	sprite->Draw(aDrawer, (int)myPosition.myX + World::GAME_FIELD_X, (int)myPosition.myY + World::GAME_FIELD_Y);
}