#ifndef GHOST_H
#define GHOST_H

#include <list>

#include "MovableGameEntity.h"

class World;
struct PathmapTile;
class Avatar;

class Ghost : public MovableGameEntity
{
public:
	static constexpr float ORANGE_DISTANCE = 8.f;

	using Ptr = std::shared_ptr<Ghost>;

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

	Ghost(const TileCoord& aTile, GhostType type, std::shared_ptr<Drawer> drawer, std::shared_ptr<Ghost> redGhost = nullptr);
	~Ghost(void);

	void Update(float aTime, std::shared_ptr<World> aWorld, std::shared_ptr<Avatar> avatar);

	void SetState(GhostBehavior behavior);
	bool IsDead() const { return myBehavior == GhostBehavior::Dead; }
	bool IsClaimable() const { return myBehavior == GhostBehavior::Fear; }

	const TileCoord& GetTargetTile() const { return myTargetTile; }
	GhostType GetType() const { return myType; }
	GhostBehavior GetBehavior() const { return myBehavior; }

	static bool IsHomeTile(const TileCoord& tile);

protected:
	void UpdateSprite();

	void ChooseNextDirection(std::shared_ptr<World> aWorld, std::shared_ptr<Avatar> avatar);
	TileCoord BehaveScatter();
	TileCoord BehaveChase(std::shared_ptr<Avatar> avatar);
	TileCoord BehaveDead();
	TileCoord BehaveFear(const std::list<TileCoord>& allowedTiles);
	TileCoord BehaveHome();
	bool CanLeaveHome(std::shared_ptr<World> aWorld);

	std::shared_ptr<Ghost> myRedGhost = nullptr; // for cyan ghost

	GhostBehavior myBehavior;
	GhostBehavior myPreviousBehavior;
	GhostType myType;
	TileCoord myDesiredMovement{0, -1}; // intended direction after myNextTile

	std::shared_ptr<Sprite> spriteNormal = nullptr;
	std::shared_ptr<Sprite> spriteFrightened = nullptr;
	std::shared_ptr<Sprite> spriteDead = nullptr;

	float myElapsedNormal = 0.f;
	float myElapsedFrightened = 0.f;
	std::map<GhostBehavior, float> myTimes {
		{GhostBehavior::Chase, 20.f},
		{GhostBehavior::Scatter, 10.f},
		{GhostBehavior::Fear, 10.f},
	};

	TileCoord myTargetTile; // for debug
};

#endif // GHOST_H