#ifndef MOVABLEGAMEENTITY_H
#define MOVABLEGAMEENTITY_H

#include "GameEntity.h"
#include "Vector2f.h"

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

	MovableGameEntity(const Vector2f& aPosition, Sprite* entitySprite);
	~MovableGameEntity(void);

	void Respawn(const Vector2f& aTile);

	void SetNextTile(const Vector2f& tile);
	Vector2f GetCurrentTile() const { return myCurrentTile; }
	Vector2f GetNextTile() const { return myNextTile; }

	bool IsAtNextTile();

	void Move();

protected:
	Vector2f myCurrentTile;
	Vector2f myNextTile;
};

#endif // MOVABLEGAMEENTITY_H