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
		Scatter,
		Chase,
		Dead,
		Fear,
	};

	enum GhostType
	{
		Red,
		Pink,
		Cyan,
		Orange
	};

	Ghost(const Vector2f& aPosition, GhostType type, Drawer* drawer, Ghost* redGhost = nullptr);
	~Ghost(void);

	void Update(float aTime, World* aWorld, Avatar* avatar);

	void SetState(GhostBehavior behavior);
	bool IsDead() const { return myBehavior == GhostBehavior::Dead; }
	bool IsClaimable() const { return myBehavior == GhostBehavior::Fear; }

	const Vector2f& GetTargetTile() const { return myTargetTile; }
	GhostType GetType() const { return myType; }
	GhostBehavior GetBehavior() const { return myBehavior; }

protected:
	void UpdateSprite();

	void ChooseNextDirection(World* aWorld, Avatar* avatar);
	Vector2f BehaveScatter();
	Vector2f BehaveChase(Avatar* avatar);
	Vector2f BehaveDead();
	Vector2f BehaveFear(const std::list<Vector2f>& allowedTiles);
	Vector2f BehaveHome();
	bool IsHomeTile(const Vector2f& tile);
	bool CanLeaveHome();

	Ghost* myRedGhost = nullptr; // for cyan ghost

	GhostBehavior myBehavior;
	GhostBehavior myPreviousBehavior;
	GhostType myType;
	Vector2f myDesiredMovement{0, -1}; // intended direction after myNextTile

	Sprite* spriteNormal = nullptr;
	Sprite* spriteFrightened = nullptr;
	Sprite* spriteDead = nullptr;

	float myElapsedNormal = 0.f;
	float myElapsedFrightened = 0.f;
	std::map<GhostBehavior, float> myTimes {
		{GhostBehavior::Chase, 20.f},
		{GhostBehavior::Scatter, 10.f},
		{GhostBehavior::Fear, 10.f},
	};

	Vector2f myTargetTile; // for debug
};

#endif // GHOST_H