#include "World.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "PathmapTile.h"
#include "Dot.h"
#include "BigDot.h"
#include "Drawer.h"
#include "Pacman.h"
#include "Ghost.h"

World::World(Pacman* game)
	: myGame(game)
{
}

World::~World(void)
{
}

void World::Init(Drawer* gameDrawer)
{
	boardBackground = Sprite::Create({"playfield.png"}, gameDrawer, 1024, 768);
	InitMap(gameDrawer);
}

bool World::InitMap(Drawer* gameDrawer)
{
	std::string line;
	std::ifstream myfile ("map.txt");
	if (myfile.is_open())
	{
		int lineIndex = 0;
		while (! myfile.eof() )
		{
			std::getline (myfile,line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				if (line[i] == '\r' || line[i] == '\n')
					continue;
				PathmapTile* tile = new PathmapTile(i, lineIndex, (line[i] == 'x'));
				myMap[std::pair<int,int>{i, lineIndex}] = tile;

				if (line[i] == '.')
				{
					auto newSprite = Sprite::Create({"Small_Dot_32.png"}, gameDrawer, 32, 32);
					auto dot = new Dot(Vector2f(i*World::TILE_SIZE, lineIndex*World::TILE_SIZE), newSprite);
					myDots.push_back(dot);
				}
				if (line[i] == 'o')
				{
					auto newSprite = Sprite::Create({"Big_Dot_32.png"}, gameDrawer, 32, 32);
					auto dot = new BigDot(Vector2f(i*World::TILE_SIZE, lineIndex*World::TILE_SIZE), newSprite);
					myBigDots.push_back(dot);
				}
			}

			lineIndex++;
		}
		myfile.close();
	}

	return true;
}

void World::Draw(Drawer* aDrawer)
{
	boardBackground->Draw(aDrawer, 0, 0);

	for(Dot* dot : myDots)
	{
		dot->Draw(aDrawer);
	}

	for(BigDot* dot : myBigDots)
	{
		dot->Draw(aDrawer);
	}

	if (myDebugDraw)
	{
		for(auto& [tilePos, tile] : myMap)
		{
			SDL_Color color{255, 255, 255, 255};
			
			if (tile->myIsBlockingFlag) color = {0, 0, 0, 255};

			for(auto& ghost : myGame->GetGhosts())
			{
				const Vector2f& targetTile = ghost->GetTargetTile();
				if (targetTile == Vector2f{float(tile->myX), float(tile->myY)})
				{
					switch (ghost->GetType())
					{
					case Ghost::Red: color = {255, 0, 0, 255}; break;
					case Ghost::Pink: color = {255, 0, 255, 255}; break;
					case Ghost::Cyan: color = {0, 255, 255, 255}; break;
					case Ghost::Orange: color = {255, 165, 0, 255}; break;
					default: break;
					}
				}
			}

			SDL_SetRenderDrawColor(aDrawer->GetRenderer(), color.r, color.g, color.b, color.a);
			SDL_Rect rect {
				World::GAME_FIELD_X + tile->myX*World::TILE_SIZE+1,
				World::GAME_FIELD_Y + tile->myY*World::TILE_SIZE+1,
				World::TILE_SIZE-1,
				World::TILE_SIZE-1};
			SDL_RenderDrawRect(aDrawer->GetRenderer(), &rect);
		}
	}
}

bool World::TileIsValid(int anX, int anY)
{
	auto tileCoord = std::make_pair(anX, anY);
	auto tileIt = myMap.find(tileCoord);
	if (tileIt != myMap.end())
		return !myMap[tileCoord]->myIsBlockingFlag;
	return false;
}

bool World::TileIsValid(Vector2f tile)
{
	return TileIsValid(tile.myX, tile.myY);
}

int World::GetDotCount()
{
	return myDots.size() + myBigDots.size();
}

bool World::HasIntersectedDot(const Vector2f& aPosition)
{
	for(std::list<Dot*>::iterator list_iter = myDots.begin(); list_iter != myDots.end(); list_iter++)
	{
		Dot* dot = *list_iter;
		if ((dot->GetPosition() - aPosition).Length() < 5.f)
		{
			myDots.remove(dot);
			delete dot;
			return true;
		}
	}

	return false;
}

bool World::HasIntersectedBigDot(const Vector2f& aPosition)
{
	for(std::list<BigDot*>::iterator list_iter = myBigDots.begin(); list_iter != myBigDots.end(); list_iter++)
	{
		BigDot* dot = *list_iter;
		if ((dot->GetPosition() - aPosition).Length() < 5.f)
		{
			myBigDots.remove(dot);
			delete dot;
			return true;
		}
	}

	return false;
}

bool World::HasIntersectedCherry(const Vector2f& aPosition)
{
	return true;
}

void World::GetPath(int aFromX, int aFromY, int aToX, int aToY, std::list<PathmapTile*>& aList)
{
	PathmapTile* fromTile = GetTile(aFromX, aFromY);
	PathmapTile* toTile = GetTile(aToX, aToY);

	for(auto& [tilePos, tile] : myMap)
	{
		tile->myIsVisitedFlag = false;
	}

	Pathfind(fromTile, toTile, aList);
}

PathmapTile* World::GetTile(int aFromX, int aFromY)
{
	auto pos = std::make_pair(aFromX, aFromY);
	if (myMap.find(pos) != myMap.end())
		return myMap[pos];
	
	return nullptr;
}

bool World::ListDoesNotContain(PathmapTile* aFromTile, std::list<PathmapTile*>& aList)
{
	for(std::list<PathmapTile*>::iterator list_iter = aList.begin(); list_iter != aList.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;
		if (tile == aFromTile)
		{
			return false;
		}
	}

	return true;
}

bool SortFromGhostSpawn(PathmapTile* a, PathmapTile* b)
{
	int la = abs(a->myX - 13) + abs(a->myY - 13);
	int lb = abs(b->myX - 13) + abs(b->myY - 13);

    return la < lb;
}

bool World::Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::list<PathmapTile*>& aList)
{
	aFromTile->myIsVisitedFlag = true;

	if (aFromTile->myIsBlockingFlag)
		return false;

	if (aFromTile == aToTile)
		return true;

	std::list<PathmapTile*> neighborList;

	PathmapTile* up = GetTile(aFromTile->myX, aFromTile->myY - 1);
	if (up && !up->myIsVisitedFlag && !up->myIsBlockingFlag && ListDoesNotContain(up, aList))
		neighborList.push_front(up);

	PathmapTile* down = GetTile(aFromTile->myX, aFromTile->myY + 1);
	if (down && !down->myIsVisitedFlag && !down->myIsBlockingFlag && ListDoesNotContain(down, aList))
		neighborList.push_front(down);

	PathmapTile* right = GetTile(aFromTile->myX + 1, aFromTile->myY);
	if (right && !right->myIsVisitedFlag && !right->myIsBlockingFlag && ListDoesNotContain(right, aList))
		neighborList.push_front(right);

	PathmapTile* left = GetTile(aFromTile->myX - 1, aFromTile->myY);
	if (left && !left->myIsVisitedFlag && !left->myIsBlockingFlag && ListDoesNotContain(left, aList))
		neighborList.push_front(left);

	neighborList.sort(SortFromGhostSpawn);

	for(std::list<PathmapTile*>::iterator list_iter = neighborList.begin(); list_iter != neighborList.end(); list_iter++)
	{
		PathmapTile* tile = *list_iter;

		aList.push_back(tile);

		if (Pathfind(tile, aToTile, aList))
			return true;

		aList.pop_back();
	}

	return false;
}