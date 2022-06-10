#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "Avatar.h"
#include <algorithm>

Ghost::Ghost(const TileCoord& aTile, GhostType type, Drawer::Ptr drawer, Ghost::Ptr redGhost/* = nullptr*/)
	: MovableGameEntity{aTile, nullptr}
	, myBehavior{GhostBehavior::Chase}
	, myType{type}
	, myDesiredMovement{0, -1}
	, myRedGhost(redGhost)
{	
	std::string normalTexture;
	switch (myType)
	{
	case Red: normalTexture = "ghost_32_red.png"; break;
	case Pink: normalTexture = "ghost_32_pink.png"; break;
	case Cyan: normalTexture = "ghost_32_cyan.png"; break;
	case Orange: normalTexture = "ghost_32_orange.png"; break;
	}
	spriteNormal = Sprite::Create({normalTexture}, drawer, 32, 32);
	spriteFrightened = Sprite::Create({"Ghost_Vulnerable_32.png"}, drawer, 32, 32);
	spriteDead = Sprite::Create({"Ghost_Dead_32.png"}, drawer, 32, 32);

	sprite = spriteNormal;
}

Ghost::~Ghost(void)
{
}

void Ghost::Update(float aTime, World::Ptr aWorld, Avatar::Ptr avatar)
{
	if (myBehavior != GhostBehavior::Dead)
	{
		if (myBehavior == GhostBehavior::Fear)
		{
			myElapsedFrightened += aTime;
			if (myElapsedFrightened >= myTimes[GhostBehavior::Fear])
			{
				SetState(myPreviousBehavior);
				myElapsedFrightened = 0.f;
			}
		}
		else
		{
			myElapsedNormal += aTime;
			if (myElapsedNormal >= myTimes[myBehavior])
			{
				auto nextState = (myBehavior == GhostBehavior::Chase) ? GhostBehavior::Scatter : GhostBehavior::Chase;
				SetState(nextState);
				myElapsedNormal = 0.f;
			}
		}
	}

	float speed = 60.f;
	TileCoord nextTile = myCurrentTile + myDesiredMovement;

	if (IsDead())
		speed = 120.f;

	if (IsAtNextTile())
	{
		if (aWorld->TileIsValid(nextTile))
		{
			SetNextTile(nextTile);
			if (IsDead() && nextTile == TileCoord{13,13})
			{
				SetState(myPreviousBehavior);
			}
		}

		ChooseNextDirection(aWorld, avatar);
	}

	Vector2f destination{myNextTile * World::TILE_SIZE};
	Vector2f direction = destination - myPosition;

	float distanceToMove = aTime * speed;

	if (distanceToMove > direction.Length())
	{
		myCurrentTile = myNextTile;
		auto tile = aWorld->GetTile(myCurrentTile);
		if (tile->linkedTile)
		{
			// TODO: hardcoded portals
			myCurrentTile = {tile->linkedTile->first, tile->linkedTile->second};
			if (tile->coord.x < 0)
				SetNextTile(myCurrentTile + TileCoord{-1, 0});
			else
				SetNextTile(myCurrentTile + TileCoord{1, 0});
		}
		myPosition = myCurrentTile * World::TILE_SIZE;
	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;
	}
}

void Ghost::ChooseNextDirection(World::Ptr aWorld, Avatar::Ptr avatar)
{
	std::list<TileCoord> allowedTiles {
		myNextTile + TileCoord{0, -1},
		myNextTile + TileCoord{0, 1},
		myNextTile + TileCoord{-1, 0},
		myNextTile + TileCoord{1, 0},
	};
	auto nextTile = aWorld->GetTile(myNextTile);
	if (nextTile->linkedTile)
	{
		// TODO: hardcoded portals
		if (nextTile->coord.x < 0)
			allowedTiles = {TileCoord{25, 13}};
		else
			allowedTiles = {TileCoord{0, 13}};
	}
	else
	{
		allowedTiles.remove_if([this, aWorld](const TileCoord& tile) {
			if (tile == myCurrentTile)
				return true; // cannot move back
			if (IsHomeTile(myCurrentTile) && !CanLeaveHome(aWorld) && !IsHomeTile(tile))
				return true; // cannot leave home if we are there and !CanLeaveHome()
			if (!IsHomeTile(myCurrentTile) && !IsDead() && IsHomeTile(tile))
				return true; // cannot enter home if we are not dead
			
			return !aWorld->TileIsValid(tile);
		});
	}

	TileCoord targetTile;

	if (IsHomeTile(myCurrentTile) && !IsDead())
		targetTile = TileCoord{12, 10};
	else
	{
		switch (myBehavior)
		{
			case Chase: targetTile = BehaveChase(avatar); break;
			case Scatter: targetTile = BehaveScatter(); break;
			case Fear: targetTile = BehaveFear(allowedTiles); break;
			case Dead: targetTile = BehaveDead(); break;
			default: break;
		}
	}

	auto desiredTileIt = std::min_element(allowedTiles.begin(), allowedTiles.end(),
		[targetTile](const TileCoord& tile1, const TileCoord& tile2){
			Vector2f vec1{targetTile - tile1};
			Vector2f vec2{targetTile - tile2};
			return vec1.Length() < vec2.Length();
		});
	
	myDesiredMovement = *desiredTileIt - myNextTile;
	myTargetTile = targetTile;
}

TileCoord Ghost::BehaveScatter()
{
	TileCoord targetTile;
	switch(myType)
		{
		case GhostType::Red: targetTile = TileCoord{27, -1}; break;
		case GhostType::Pink: targetTile = TileCoord{0, -1}; break;
		case GhostType::Cyan: targetTile = TileCoord{27, 30}; break;
		case GhostType::Orange: targetTile = TileCoord{0, 30}; break;
		default: break;
		}
	return targetTile;
}

TileCoord Ghost::BehaveChase(Avatar::Ptr avatar)
{
	TileCoord targetTile;
	switch(myType)
		{
		case GhostType::Red:
		{
			targetTile = avatar->GetCurrentTile();
			break;
		}
		case GhostType::Pink:
		{
			TileCoord avatarDirectionVec = avatar->GetMovementDirectionVec();
			targetTile = avatar->GetCurrentTile() + avatarDirectionVec * 4;
			break;
		}
		case GhostType::Cyan:
		{
			TileCoord redGhostTile = myRedGhost->GetCurrentTile();
			
			TileCoord avatarDirectionVec = avatar->GetMovementDirectionVec();

			TileCoord tileInFrontOfAvatar = avatar->GetCurrentTile() + avatarDirectionVec * 2;
			TileCoord directionToRed = redGhostTile - tileInFrontOfAvatar;
			targetTile = tileInFrontOfAvatar + (directionToRed * -1);

			break;
		}
		case GhostType::Orange:
		{
			float distanceToAvatar = Vector2f{avatar->GetCurrentTile() - myCurrentTile}.Length();
			if (distanceToAvatar <= 8.f)
			{
				targetTile = BehaveScatter();
			}
			else
			{
				targetTile = avatar->GetCurrentTile();
			}
			break;
		}
		}
	return targetTile;
}

TileCoord Ghost::BehaveDead()
{
	return {13, 13};
}

TileCoord Ghost::BehaveFear(const std::list<TileCoord>& allowedTiles)
{
	int r = rand() % allowedTiles.size();
	auto pickedTileIt = std::next(allowedTiles.begin(), r);
	return *pickedTileIt;
}

std::string enumTypeToString(Ghost::GhostType type)
{
	switch (type)
	{
	case Ghost::Red: return "RED";
	case Ghost::Cyan: return "CYAN";
	case Ghost::Orange: return "ORANGE";
	case Ghost::Pink: return "PINK";
	}
	return "";
}
std::string enumBehaviorToString(Ghost::GhostBehavior behavior)
{
	switch (behavior)
	{
	case Ghost::Chase: return "CHASE";
	case Ghost::Scatter: return "SCATTER";
	case Ghost::Fear: return "FEAR";
	case Ghost::Dead: return "DEAD";
	}
	return "";
}

void Ghost::SetState(GhostBehavior behavior)
{
	printf("%s: setting state %s\n", enumTypeToString(myType).c_str(), enumBehaviorToString(behavior).c_str());
	if (myBehavior == Chase || myBehavior == Scatter)
		myPreviousBehavior = myBehavior;
	myBehavior = behavior;

	UpdateSprite();
}

void Ghost::UpdateSprite()
{
	switch (myBehavior)
	{
	case GhostBehavior::Fear: sprite = spriteFrightened; break;
	case GhostBehavior::Dead: sprite = spriteDead; break;
	default: sprite = spriteNormal; break;
	}
}

bool Ghost::IsHomeTile(const TileCoord& tile)
{
	if (tile.x > 9 && tile.x < 16)
		if (tile.y > 10 && tile.y < 15)
			return true;
	return false;
}

bool Ghost::CanLeaveHome(std::shared_ptr<World> aWorld)
{
	switch (myType)
	{
	case Ghost::Red: return true;
	case Ghost::Pink: return aWorld->GetEatenDotsCount() > 100;
	case Ghost::Cyan: return aWorld->GetEatenDotsCount() > 200;
	case Ghost::Orange: return aWorld->GetEatenDotsCount() > 300;
	}
}