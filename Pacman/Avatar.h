#ifndef AVATAR_H
#define AVATAR_H

#include "MovableGameEntity.h"
#include "Vector2f.h"

class World;

class Avatar : public MovableGameEntity
{
public:
	Avatar(const Vector2f& aPosition, Sprite* entitySprite, World* world);
	~Avatar(void);

	void Update(float aTime);

	void SetNextMovement(const Vector2f& next) { myNextMovement = next; } 
	Vector2f GetNextMovement() { return myNextMovement; }

private:
	bool TryTile(int x, int y);
	void MyMove(float dt);

private:
	Vector2f myNextMovement;
	float mySpeed = 60.f;

	World* myWorld;
};

#endif //AVATAR_H