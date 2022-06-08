#include "Avatar.h"
#include "World.h"
#include "PathmapTile.h"

Avatar::Avatar(const Vector2f& aPosition, World* world, Drawer* drawer)
	: MovableGameEntity(aPosition, nullptr)
	, myWorld(world)
{
	myNextTile = {myCurrentTile.myX - 1.f, myCurrentTile.myY}; 

	mySprites[MovementDirection::Up] = Sprite::Create({"open_up_32.png", "closed_up_32.png"}, drawer, 32, 32);
	mySprites[MovementDirection::Down] = Sprite::Create({"open_down_32.png", "closed_down_32.png"}, drawer, 32, 32);
	mySprites[MovementDirection::Left] = Sprite::Create({"open_left_32.png", "closed_left_32.png"}, drawer, 32, 32);
	mySprites[MovementDirection::Right] = Sprite::Create({"open_right_32.png", "closed_right_32.png"}, drawer, 32, 32);

	sprite = mySprites[MovementDirection::Left];
}

Avatar::~Avatar(void)
{
}

void Avatar::Update(float aTime)
{
	MyMove(aTime);
	for(auto& [dir, mappedSprite] : mySprites)
		mappedSprite->Update(aTime);
	
	sprite = mySprites[GetMovementDirection()]; //TODO: change sprite only when changing direction
}

bool Avatar::TryTile(int x, int y)
{
	PathmapTile* desiredTile = myWorld->GetTile(x, y);
	if (desiredTile && !desiredTile->myIsBlockingFlag)
	{
		myNextTile = {(float)x, (float)y};
		return true;
	}
	return false;
}

bool Avatar::TryTile(Vector2f tile)
{
	return TryTile(tile.myX, tile.myY);
}

void Avatar::MyMove(float dt)
{
	// if we're not moving anywhere
	if (IsAtNextTile())
	{
		if (myNextMovement == Vector2f{0.f,0.f})
			return;
		
		if (TryTile(myCurrentTile + myNextMovement))
		{
			myNextMovement = Vector2f{0.f, 0.f};
		}
		else
		{
			return;
		}
	}

	Vector2f gridDirection = myNextTile - myCurrentTile;
	if ((myNextMovement.myX != 0 && myNextMovement.myX == -gridDirection.myX)
		|| (myNextMovement.myY != 0 && myNextMovement.myY == -gridDirection.myY))
	{
		// turning backwards
		std::swap(myCurrentTile, myNextTile);
		myNextMovement = Vector2f{0.f, 0.f};
	}

	Vector2f nextTilePos{myNextTile * World::TILE_SIZE};
	Vector2f direction = nextTilePos - myPosition;

	float distanceToMove = dt * mySpeed;
	float distanceToNextTile = direction.Length();

	if (distanceToMove > distanceToNextTile)
	{
		//steped on next tile
		myPosition = nextTilePos;
		Vector2f previousDirection = myNextTile - myCurrentTile;
		myCurrentTile = myNextTile;
		distanceToMove -= distanceToNextTile;

		if (myNextMovement != Vector2f{0.f,0.f})
		{
			Vector2f desiredTile = myCurrentTile + myNextMovement;

			//trying to turn
			if (TryTile(desiredTile))
			{
				myNextMovement = Vector2f{0, 0};
				return;
			}
		}
		
		Vector2f desiredTile = myCurrentTile + previousDirection;
		if (!TryTile(desiredTile))
		{
			myNextTile = myCurrentTile;
		}
	}

	direction.Normalize();
	myPosition += direction * distanceToMove;
}

Avatar::MovementDirection Avatar::GetMovementDirection()
{
	if (myNextTile.myX > myCurrentTile.myX)
		return MovementDirection::Right;
	else if (myNextTile.myX < myCurrentTile.myX)
		return MovementDirection::Left;
	else if (myNextTile.myY < myCurrentTile.myY)
		return MovementDirection::Up;
	else // (myNextTile.myY >= myCurrentTile.myY)
		return MovementDirection::Down;
}