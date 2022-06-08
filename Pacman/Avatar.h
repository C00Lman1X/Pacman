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

	Avatar(const Vector2f& aPosition, std::shared_ptr<World> world, std::shared_ptr<Drawer> drawer);
	~Avatar(void);

	void Update(float aTime);

	void SetNextMovement(const Vector2f& next) { myNextMovement = next; } 
	Vector2f GetNextMovement() { return myNextMovement; }
	MovementDirection GetMovementDirection();
	Vector2f GetMovementDirectionVec();
	Vector2f GetPreviousMovementDirectionVec();

	void Draw(std::shared_ptr<Drawer> drawer) override;

private:
	bool TryTile(int x, int y);
	bool TryTile(Vector2f tile);
	void MyMove(float dt);

private:
	Vector2f myNextMovement;
	float mySpeed = 200.f;
	std::map<MovementDirection, Sprite::Ptr> mySprites;
	MovementDirection myPreviousDirection;

	std::shared_ptr<World> myWorld;
};

#endif //AVATAR_H