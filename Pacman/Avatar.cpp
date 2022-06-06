#include "Avatar.h"
#include "World.h"
#include "PathmapTile.h"

Avatar::Avatar(const Vector2f& aPosition, World* world, Drawer* drawer)
	: MovableGameEntity(aPosition, nullptr)
	, myWorld(world)
{
	myNextTileX = myCurrentTileX - 1;
	myNextTileY = myCurrentTileY;

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
		myNextTileX = x;
		myNextTileY = y;
		return true;
	}
	return false;
}

void Avatar::MyMove(float dt)
{
	if (myNextTileX == myCurrentTileX && myNextTileY == myCurrentTileY)
	{
		if (myNextMovement == Vector2f{0.f,0.f})
			return;
		
		if (TryTile(myCurrentTileX + myNextMovement.myX, myCurrentTileY+myNextMovement.myY))
		{
			myNextMovement = Vector2f{0.f, 0.f};
		}
		else
		{
			return;
		}
	}

	int gridDirectionX = myNextTileX - myCurrentTileX;
	int gridDirectionY = myNextTileY - myCurrentTileY;
	if ((myNextMovement.myX != 0 && myNextMovement.myX == -gridDirectionX)
		|| (myNextMovement.myY != 0 && myNextMovement.myY == -gridDirectionY))
	{
		// turning backwards
		std::swap(myCurrentTileX, myNextTileX);
		std::swap(myCurrentTileY, myNextTileY);
		myNextMovement = Vector2f{0.f, 0.f};
	}

	Vector2f nextTilePos(myNextTileX * World::TILE_SIZE, myNextTileY * World::TILE_SIZE);
	Vector2f direction = nextTilePos - myPosition;

	float distanceToMove = dt * mySpeed;
	float distanceToNextTile = direction.Length();

	if (distanceToMove > distanceToNextTile)
	{
		//steped on next tile
		myPosition = nextTilePos;
		int previousDirectionX = myNextTileX - myCurrentTileX;
		int previousDirectionY = myNextTileY - myCurrentTileY;
		myCurrentTileX = myNextTileX;
		myCurrentTileY = myNextTileY;
		distanceToMove -= distanceToNextTile;

		if (myNextMovement != Vector2f{0.f,0.f})
		{
			int desiredTileX = myCurrentTileX + myNextMovement.myX;
			int desiredTileY = myCurrentTileY + myNextMovement.myY;

			//trying to turn
			if (TryTile(desiredTileX, desiredTileY))
			{
				myNextMovement = Vector2f{0, 0};
				return;
			}
		}

		int desiredTileX = myCurrentTileX + previousDirectionX;
		int desiredTileY = myCurrentTileY + previousDirectionY;
		if (!TryTile(desiredTileX, desiredTileY))
		{
			myNextTileX = myCurrentTileX;
			myNextTileY = myCurrentTileY;
		}

	}
	else
	{
		direction.Normalize();
		myPosition += direction * distanceToMove;
	}
}

Avatar::MovementDirection Avatar::GetMovementDirection()
{
	if (myNextTileX > myCurrentTileX)
		return MovementDirection::Right;
	else if (myNextTileX < myCurrentTileX)
		return MovementDirection::Left;
	else if (myNextTileY < myCurrentTileY)
		return MovementDirection::Up;
	else // (myNextTileY >= myCurrentTileY)
		return MovementDirection::Down;
}