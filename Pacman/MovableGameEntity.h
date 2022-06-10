#ifndef MOVABLEGAMEENTITY_H
#define MOVABLEGAMEENTITY_H

#include "GameEntity.h"
#include "Vector2f.h"
#include "PathmapTile.h"

class MovableGameEntity : public GameEntity
{
public:
	enum MovementDirection: int
	{
		Up = 0,
		Down = 1,
		Left = 2,
		Right = 3,
		DirectionCount = 4
	};

	MovableGameEntity(const TileCoord& aTile, Sprite::Ptr entitySprite);
	~MovableGameEntity(void);

	void Respawn(const TileCoord& aTile);

	void SetNextTile(const TileCoord& tile);
	const TileCoord& GetNextTile() const { return myNextTile; }

	bool IsAtNextTile();

	void Move();

protected:
	TileCoord myNextTile;
};

#endif // MOVABLEGAMEENTITY_H