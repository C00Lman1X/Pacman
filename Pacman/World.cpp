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
#include "Avatar.h"

World::World(Pacman* game)
	: myGame(game)
{
}

World::~World(void)
{
}

void World::Init(Drawer::Ptr gameDrawer)
{
	boardBackground = Sprite::Create({"playfield.png"}, gameDrawer, 1024, 768);
	InitMap(gameDrawer);
}

bool World::InitMap(Drawer::Ptr gameDrawer)
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
				PathmapTile::Ptr tile = std::make_shared<PathmapTile>(i, lineIndex, (line[i] == 'x'));
				myMap[std::pair<int,int>{i, lineIndex}] = tile;

				if (line[i] == '.')
				{
					auto newSprite = Sprite::Create({"Small_Dot_32.png"}, gameDrawer, 32, 32);
					auto dot = std::make_shared<Dot>(Vector2f(i*World::TILE_SIZE, lineIndex*World::TILE_SIZE), newSprite);
					myDots.push_back(dot);
					tile->myDot = dot;
				}
				if (line[i] == 'o')
				{
					auto newSprite = Sprite::Create({"Big_Dot_32.png"}, gameDrawer, 32, 32);
					auto dot = std::make_shared<BigDot>(Vector2f(i*World::TILE_SIZE, lineIndex*World::TILE_SIZE), newSprite);
					myBigDots.push_back(dot);
					tile->myBigDot = dot;
				}
				if (line[i] == 'a')
				{
					if (i == 0)
					{
						PathmapTile::Ptr tile = std::make_shared<PathmapTile>(-1, lineIndex, false);
						myMap[std::pair<int,int>{-1, lineIndex}] = tile;
						tile->linkedTile = std::make_pair(26, lineIndex);
					}
					else if (i == 25)
					{
						PathmapTile::Ptr tile = std::make_shared<PathmapTile>(26, lineIndex, false);
						myMap[std::pair<int,int>{26, lineIndex}] = tile;
						tile->linkedTile = std::make_pair(-1, lineIndex);
					}
				}
			}

			lineIndex++;
		}
		myfile.close();
	}

	return true;
}

void World::Draw(Drawer::Ptr aDrawer)
{
	boardBackground->Draw(aDrawer, 0, 0);

	for(Dot::Ptr dot : myDots)
	{
		dot->Draw(aDrawer);
	}

	for(BigDot::Ptr dot : myBigDots)
	{
		dot->Draw(aDrawer);
	}

	if (myDebugDraw)
	{
		for(auto& [tilePos, tile] : myMap)
		{
			SDL_Color color{255, 255, 255, 255};
			
			if (tile->myIsBlockingFlag) color = {0, 0, 0, 255};
			if (tile->myX == myGame->GetAvatar()->GetCurrentTile().myX
				&& tile->myY == myGame->GetAvatar()->GetCurrentTile().myY)
				color = {255, 255, 0, 255};
			
			SDL_SetRenderDrawColor(aDrawer->GetRenderer().get(), color.r, color.g, color.b, color.a);
			SDL_Rect rect {
				World::GAME_FIELD_X + tile->myX*World::TILE_SIZE+1,
				World::GAME_FIELD_Y + tile->myY*World::TILE_SIZE+1,
				World::TILE_SIZE-1,
				World::TILE_SIZE-1};
			SDL_RenderDrawRect(aDrawer->GetRenderer().get(), &rect);

			if (Ghost::IsHomeTile(Vector2f(tile->myX, tile->myY)))
			{
				SDL_SetRenderDrawColor(aDrawer->GetRenderer().get(), 0, 0, 255, 255);
				SDL_Rect rect {
					World::GAME_FIELD_X + tile->myX*World::TILE_SIZE,
					World::GAME_FIELD_Y + tile->myY*World::TILE_SIZE,
					World::TILE_SIZE,
					World::TILE_SIZE};
				SDL_RenderDrawRect(aDrawer->GetRenderer().get(), &rect);
			}
		}

		for(auto& ghost : myGame->GetGhosts())
		{
			SDL_Color ghostColor;
			const Vector2f& targetTile = ghost->GetTargetTile();
			if (targetTile == Vector2f{targetTile.myX, targetTile.myY})
			{
				switch (ghost->GetType())
				{
				case Ghost::Red: ghostColor = {255, 0, 0, 255}; break;
				case Ghost::Pink: ghostColor = {255, 0, 255, 255}; break;
				case Ghost::Cyan: ghostColor = {0, 255, 255, 255}; break;
				case Ghost::Orange: ghostColor = {255, 165, 0, 255}; break;
				default: break;
				}
			}
			SDL_SetRenderDrawColor(aDrawer->GetRenderer().get(), ghostColor.r, ghostColor.g, ghostColor.b, ghostColor.a);
			SDL_Rect targetRect {
				int(World::GAME_FIELD_X + targetTile.myX*World::TILE_SIZE+2),
				int(World::GAME_FIELD_Y + targetTile.myY*World::TILE_SIZE+2),
				World::TILE_SIZE-2,
				World::TILE_SIZE-2};
			SDL_RenderDrawRect(aDrawer->GetRenderer().get(), &targetRect);
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

bool World::TryEatDotAt(int x, int y)
{
	auto tileIt = myMap.find(std::make_pair(x,y));
	if (tileIt == myMap.end())
		return false;
	
	auto& [tileCoord, tile] = *tileIt;
	bool eaten = (tile->myDot != nullptr);
	if (eaten)
	{
		Vector2f tilePos{x*World::TILE_SIZE, y*World::TILE_SIZE};
		myDots.remove_if([=](Dot::Ptr dot) { return dot->GetPosition() == tilePos; });
		tile->myDot = nullptr;
		myEatenDots++;
	}
	return eaten;
}

bool World::TryEatBigDotAt(int x, int y)
{
	auto tileIt = myMap.find(std::make_pair(x,y));
	if (tileIt == myMap.end())
		return false;
	
	auto& [tileCoord, tile] = *tileIt;
	bool eaten = (tile->myBigDot != nullptr);
	if (eaten)
	{
		Vector2f tilePos{x*World::TILE_SIZE, y*World::TILE_SIZE};
		myBigDots.remove_if([=](BigDot::Ptr dot) { return dot->GetPosition() == tilePos; });
		tile->myBigDot = nullptr;
		myEatenDots++;
	}
	return eaten;
}

bool World::TryEatCherryAt(int x, int y)
{
	return false;
}

PathmapTile::Ptr World::GetTile(int aFromX, int aFromY)
{
	auto pos = std::make_pair(aFromX, aFromY);
	if (myMap.find(pos) != myMap.end())
		return myMap[pos];
	
	return nullptr;
}

PathmapTile::Ptr World::GetTileFromCoords(float x, float y)
{
	int tileX = x/World::TILE_SIZE;
	int tileY = y/World::TILE_SIZE;
	return GetTile(tileX, tileY);
}