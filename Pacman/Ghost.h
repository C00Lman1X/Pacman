#ifndef GHOST_H
#define GHOST_H

#include <list>

#include "MovableGameEntity.h"

class World;
class PathmapTile;
class Avatar;

class Ghost : public MovableGameEntity
{
public:
	enum GhostBehavior
	{
		Wander,
		Chase,
		Intercept,
		Fear,
	};

	Ghost(const Vector2f& aPosition, Sprite* entitySprite, GhostBehavior behavior);
	~Ghost(void);

	void Update(float aTime, World* aWorld, Avatar* avatar);

	bool myIsClaimableFlag;
	bool myIsDeadFlag;

	void SetImage(std::string anImage);

	void Die(World* aWorld);

protected:
	void BehaveWander();
	void BehaveChase(World* aWorld, Avatar* avatar);
	void BehaveIntercept(World* aWorld, Avatar* avatar);
	void BehaveFear(World* aWorld, Avatar* avatar);
	void BehaveVulnerable();

	GhostBehavior myBehavior;
	int myDesiredMovementX;
	int myDesiredMovementY;

	std::list<PathmapTile*> myPath;

};

#endif // GHOST_H