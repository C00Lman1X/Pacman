#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "Avatar.h"
#include <algorithm>

Ghost::Ghost(const Vector2f& aPosition, GhostType type, Drawer* drawer, Ghost* redGhost/* = nullptr*/)
	: MovableGameEntity{aPosition, nullptr}
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

void Ghost::Update(float aTime, World* aWorld, Avatar* avatar)
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
	Vector2f nextTile = myCurrentTile + myDesiredMovement;

	if (IsDead())
		speed = 120.f;

	if (IsAtNextTile())
	{
		if (aWorld->TileIsValid(nextTile))
		{
			SetNextTile(nextTile);
			if (IsDead() && nextTile == Vector2f{13.f, 13.f})
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
		myPosition = destination;
		myCurrentTile = myNextTile;
	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;
	}
}

void Ghost::ChooseNextDirection(World* aWorld, Avatar* avatar)
{
	std::list<Vector2f> allowedTiles {
		myNextTile + Vector2f{0.f, -1.f},
		myNextTile + Vector2f{0.f, 1.f},
		myNextTile + Vector2f{-1.f, 0.f},
		myNextTile + Vector2f{1.f, 0.f},
	};
	allowedTiles.remove_if([this, aWorld](const Vector2f& tile) {
		if (tile == myCurrentTile)
			return true;
		return !aWorld->TileIsValid(tile);
	});

	if (allowedTiles.empty())
	{
		printf("WHOOPS\n");
	}

	Vector2f targetTile;

	if (IsHomeTile(myCurrentTile) && !IsDead())
		targetTile = Vector2f{11.f, 10.f};
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
		[targetTile](const Vector2f& tile1, const Vector2f& tile2){
			float distance1 = (targetTile - tile1).Length();
			float distance2 = (targetTile - tile2).Length();
			return distance1 < distance2;
		});
	
	myDesiredMovement = *desiredTileIt - myNextTile;
	myTargetTile = targetTile;
}

Vector2f Ghost::BehaveScatter()
{
	Vector2f targetTile;
	switch(myType)
		{
		case GhostType::Red: targetTile = Vector2f{27.f, -1.f}; break;
		case GhostType::Pink: targetTile = Vector2f{0.f, -1.f}; break;
		case GhostType::Cyan: targetTile = Vector2f{27.f, 30.f}; break;
		case GhostType::Orange: targetTile = Vector2f{0.f, 30.f}; break;
		default: break;
		}
	return targetTile;
}

Vector2f Ghost::BehaveChase(Avatar* avatar)
{
	Vector2f targetTile;
	switch(myType)
		{
		case GhostType::Red:
		{
			targetTile = avatar->GetCurrentTile();
			break;
		}
		case GhostType::Pink:
		{
			Vector2f avatarDirection = avatar->GetNextTile() - avatar->GetCurrentTile();
			targetTile = avatar->GetCurrentTile() + avatarDirection * 4.f;
			break;
		}
		case GhostType::Cyan:
		{
			Vector2f redGhostTile = myRedGhost->GetCurrentTile();
			
			Vector2f avatarDirection = avatar->GetNextTile() - avatar->GetCurrentTile();
			Vector2f tileInFrontOfAvatar = avatar->GetCurrentTile() + avatarDirection * 2.f;
			Vector2f directionToRed = redGhostTile - tileInFrontOfAvatar;
			targetTile = tileInFrontOfAvatar + (directionToRed * -1.f);

			break;
		}
		case GhostType::Orange:
		{
			float distanceToAvatar = (avatar->GetCurrentTile() - myCurrentTile).Length();
			if (distanceToAvatar <= 8)
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

Vector2f Ghost::BehaveDead()
{
	return {13.f, 13.f};
}

Vector2f Ghost::BehaveFear(const std::list<Vector2f>& allowedTiles)
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

bool Ghost::IsHomeTile(const Vector2f& tile)
{
	if (tile.myX > 9 && tile.myX < 16)
		if (tile.myY > 10 && tile.myY < 15)
			return true;
	return false;
}