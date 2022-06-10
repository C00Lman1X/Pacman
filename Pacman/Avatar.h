#ifndef AVATAR_H
#define AVATAR_H

#include "MovableGameEntity.h"
#include "Vector2f.h"
#include "Sprite.h"

class World;

class Avatar : public MovableGameEntity
{
public:
	using Ptr = std::shared_ptr<Avatar>;

	Avatar(const TileCoord& aTile, std::shared_ptr<World> world, std::shared_ptr<Drawer> drawer);
	~Avatar(void);

	void Update(float aTime);

	void SetNextMovement(const TileCoord& next) { myNextMovement = next; } 
	TileCoord GetNextMovement() { return myNextMovement; }
	MovementDirection GetMovementDirection();
	TileCoord GetMovementDirectionVec();
	TileCoord GetPreviousMovementDirectionVec();

	void Draw(std::shared_ptr<Drawer> drawer) override;

private:
	bool TryToSetNextTile(TileCoord tileCoord);
	void MyMove(float dt);

private:
	TileCoord myNextMovement;
	float mySpeed = 60.f;
	std::map<MovementDirection, Sprite::Ptr> mySprites;
	MovementDirection myPreviousDirection;

	std::shared_ptr<World> myWorld;
};

#endif //AVATAR_H