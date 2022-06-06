#ifndef AVATAR_H
#define AVATAR_H

#include "MovableGameEntity.h"
#include "Vector2f.h"
#include "Sprite.h"

class World;

class Avatar : public MovableGameEntity
{
public:
	Avatar(const Vector2f& aPosition, World* world, Drawer* drawer);
	~Avatar(void);

	void Update(float aTime);

	void SetNextMovement(const Vector2f& next) { myNextMovement = next; } 
	Vector2f GetNextMovement() { return myNextMovement; }

private:
	bool TryTile(int x, int y);
	void MyMove(float dt);
	MovementDirection GetMovementDirection();

private:
	Vector2f myNextMovement;
	float mySpeed = 60.f;
	std::map<MovementDirection, Sprite*> mySprites;

	World* myWorld;
};

#endif //AVATAR_H