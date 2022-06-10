#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "Vector2f.h"
#include "Sprite.h"
#include "PathmapTile.h"

class Drawer;

class GameEntity
{
public:
	GameEntity(const TileCoord& aTile, Sprite::Ptr entitySprite);

	Vector2f GetPosition() const { return myPosition; }
	void SetPosition(const Vector2f& aPosition){ myPosition = aPosition; }
	const TileCoord& GetCurrentTile() const { return myCurrentTile; }

	virtual void Draw(std::shared_ptr<Drawer> aDrawer);

protected:

	Sprite::Ptr sprite;
	bool myIdMarkedForDeleteFlag;
	Vector2f myPosition; // relative to game field
	TileCoord myCurrentTile;
};

#endif // GAMEENTITY_H