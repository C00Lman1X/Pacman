#include "Avatar.h"
#include "World.h"
#include "PathmapTile.h"
#include "Ghost.h"
#include "Drawer.h"

Avatar::Avatar(const TileCoord& aTile, World::Ptr world, Drawer::Ptr drawer)
	: MovableGameEntity(aTile, nullptr)
	, myWorld(world)
{
	myNextTile = myCurrentTile - TileCoord{1,0}; 

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

bool Avatar::TryToSetNextTile(TileCoord tileCoord)
{
	PathmapTile::Ptr desiredTile = myWorld->GetTile(tileCoord);
	if (desiredTile
		&& !desiredTile->myIsBlockingFlag
		&& !Ghost::IsHomeTile(tileCoord))
	{
		myNextTile = tileCoord;
		return true;
	}
	return false;
}

void Avatar::MyMove(float dt)
{
	// if we're not moving anywhere
	if (IsAtNextTile())
	{
		if (myNextMovement == TileCoord{0,0})
			return;
		
		if (TryToSetNextTile(myCurrentTile + myNextMovement))
		{
			myNextMovement = TileCoord{0,0};
		}
		else
		{
			return;
		}
	}

	TileCoord gridDirection = myNextTile - myCurrentTile;
	if ((myNextMovement.x != 0 && myNextMovement.x == -gridDirection.x)
		|| (myNextMovement.y != 0 && myNextMovement.y == -gridDirection.y))
	{
		// turning backwards
		std::swap(myCurrentTile, myNextTile);
		myNextMovement = TileCoord{0, 0};
	}

	Vector2f nextTilePos{myNextTile * World::TILE_SIZE};
	Vector2f direction = nextTilePos - myPosition;

	float distanceToMove = dt * mySpeed;
	float sqrDistanceToNextTile = direction.myX*direction.myX + direction.myY*direction.myY;

	if (distanceToMove > sqrDistanceToNextTile)
	{
		//steped on next tile
		myPreviousDirection = GetMovementDirection();
		myCurrentTile = myNextTile;

		// check for portal
		auto tile = myWorld->GetTile(myCurrentTile);
		if (tile && tile->linkedTile)
		{
			myCurrentTile = {tile->linkedTile->first, tile->linkedTile->second};
		}

		myPosition = myCurrentTile * World::TILE_SIZE;
		distanceToMove -= sqrt(sqrDistanceToNextTile);

		if (myNextMovement != TileCoord{0,0})
		{
			TileCoord desiredTile = myCurrentTile + myNextMovement;

			//trying to turn
			if (TryToSetNextTile(desiredTile))
			{
				myNextMovement = TileCoord{0, 0};
				return;
			}
		}
		
		TileCoord desiredTile = myCurrentTile + GetPreviousMovementDirectionVec();
		if (!TryToSetNextTile(desiredTile))
		{
			myNextTile = myCurrentTile;
		}
	}

	direction.Normalize();
	myPosition += direction * distanceToMove;
}

void Avatar::Draw(std::shared_ptr<Drawer> drawer)
{
	SDL_Rect frame{0, 0, 32, 32};
	/*
	auto nextTile = myWorld->GetTile(myNextTile.myX, myNextTile.myY);
	if (nextTile->linkedTile)
	{
		float percentageVisible = std::abs(myPosition.myX - nextTile->myX * World::TILE_SIZE) / World::TILE_SIZE;
		if (nextTile->myX < 0)
		{
			int leftBorder = 0;
			frame.x = leftBorder - (int)myPosition.myX;
			frame.w = 32 - frame.x;
		}
		else
		{
			frame.w = 32.f * percentageVisible;
		}
		printf("Percentage visible (%.2f) frame (%d,%d,%d,%d)\n", percentageVisible, frame.x, frame.y, frame.w, frame.h);
	}
	sprite->SetFrame(frame.x, frame.y, frame.w, frame.h);
	*/
	sprite->Draw(drawer, (int)myPosition.myX + World::GAME_FIELD_X + frame.x, (int)myPosition.myY + World::GAME_FIELD_Y);
}

Avatar::MovementDirection Avatar::GetMovementDirection()
{
	if (myNextTile.x > myCurrentTile.x)
		return MovementDirection::Right;
	else if (myNextTile.x < myCurrentTile.x)
		return MovementDirection::Left;
	else if (myNextTile.y < myCurrentTile.y)
		return MovementDirection::Up;
	else if (myNextTile.y > myCurrentTile.y)
		return MovementDirection::Down;
	else
		return myPreviousDirection;
}

TileCoord Avatar::GetMovementDirectionVec()
{
	TileCoord directionVec{0, 0};
	switch (GetMovementDirection())
	{
		case MovementDirection::Right: directionVec = {1, 0}; break;
		case MovementDirection::Left: directionVec = {-1, 0}; break;
		case MovementDirection::Up: directionVec = {0, -1}; break;
		case MovementDirection::Down: directionVec = {0, 1}; break;
		default: printf("ERROR: Wrong direction!\n"); break;
	}
	return directionVec;
}

TileCoord Avatar::GetPreviousMovementDirectionVec()
{
	TileCoord directionVec{0, 0};
	switch (myPreviousDirection)
	{
		case MovementDirection::Right: directionVec = {1, 0}; break;
		case MovementDirection::Left: directionVec = {-1, 0}; break;
		case MovementDirection::Up: directionVec = {0, -1}; break;
		case MovementDirection::Down: directionVec = {0, 1}; break;
		default: printf("ERROR: Wrong direction!\n"); break;
	}
	return directionVec;
}