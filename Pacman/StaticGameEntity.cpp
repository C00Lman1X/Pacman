#include "StaticGameEntity.h"

StaticGameEntity::StaticGameEntity(TileCoord aTile, Sprite::Ptr entitySprite)
: GameEntity(aTile, entitySprite)
{
}

StaticGameEntity::~StaticGameEntity(void)
{
}
