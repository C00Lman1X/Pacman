#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "Avatar.h"

Ghost::Ghost(const Vector2f& aPosition, Sprite* entitySprite, GhostBehavior behavior)
	: MovableGameEntity(aPosition, entitySprite),
	myBehavior(behavior)
{
	sprite->SetFrame("ghost_32_cyan.png");

	myIsClaimableFlag = false;
	myIsDeadFlag = false;

	myDesiredMovementX = 0;
	myDesiredMovementY = -1;
}

Ghost::~Ghost(void)
{
}

void Ghost::Die(World* aWorld)
{
	myPath.clear();
	aWorld->GetPath(myCurrentTileX, myCurrentTileY, 13, 13, myPath);
}

void Ghost::Update(float aTime, World* aWorld, Avatar* avatar)
{
	float speed = 30.f;
	int nextTileX = myCurrentTileX + myDesiredMovementX;
	int nextTileY = myCurrentTileY + myDesiredMovementY;

	if (myIsDeadFlag)
		speed = 120.f;

	if (IsAtDestination())
	{
		if (!myPath.empty())
		{
			PathmapTile* nextTile = myPath.front();
			myPath.pop_front();
			SetNextTile(nextTile->myX, nextTile->myY);
		}
		else if (aWorld->TileIsValid(nextTileX, nextTileY))
		{
			SetNextTile(nextTileX, nextTileY);
		}
		else
		{
			if (myIsClaimableFlag)
				BehaveVulnerable();
			else
			{
				switch (myBehavior)
				{
				case Chase:
					BehaveChase(aWorld, avatar);
					break;
				case Intercept:
				case Fear:
				case Wander:
				default:
					BehaveWander();
					break;
				}
			}

			myIsDeadFlag = false;
		}
	}

	int tileSize = 22;
	Vector2f destination(myNextTileX * tileSize, myNextTileY * tileSize);
	Vector2f direction = destination - myPosition;

	float distanceToMove = aTime * speed;

	if (distanceToMove > direction.Length())
	{
		myPosition = destination;
		myCurrentTileX = myNextTileX;
		myCurrentTileY = myNextTileY;
	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;
	}
}

void Ghost::SetImage(std::string anImage)
{
	sprite->SetFrame(anImage);
}

void Ghost::BehaveWander()
{
	MovementDirection nextDirection = (MovementDirection)(rand() % DirectionCount);
	switch (nextDirection)
	{
	case Up:
		myDesiredMovementX = 0;
		myDesiredMovementY = 1;
		break;
	case Down:
		myDesiredMovementX = 0;
		myDesiredMovementY = -1;
		break;
	case Left:
		myDesiredMovementX = -1;
		myDesiredMovementY = 0;
		break;
	case Right:
		myDesiredMovementX = 1;
		myDesiredMovementY = 0;
		break;
	default:
		break;
	}
}

void Ghost::BehaveChase(World* aWorld, Avatar* avatar)
{
	myPath.clear();
	aWorld->GetPath(myCurrentTileX, myCurrentTileY, avatar->GetCurrentTileX(), avatar->GetCurrentTileY(), myPath);
}

void Ghost::BehaveIntercept(World* aWorld, Avatar* avatar)
{

}

void Ghost::BehaveFear(World* aWorld, Avatar* avatar)
{

}

void Ghost::BehaveVulnerable()
{

}