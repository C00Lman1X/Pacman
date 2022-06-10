#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <map>
#include <unordered_map>
#include "Vector2f.h"
#include "PathmapTile.h"
#include "Sprite.h"

class Drawer;
class Dot;
class BigDot;
class Cherry;
class Pacman;

struct tileCoordHash
{
	size_t operator()(const TileCoord& c) const
	{
		// TODO: probably get rid of unordered_map, that's a poor hash for grid coords
		return std::hash<int>{}(c.x) ^ std::hash<int>{}(c.y);
	}
};

class World
{
public:
	using Ptr = std::shared_ptr<World>;

	static constexpr int GAME_FIELD_X = 220;
	static constexpr int GAME_FIELD_Y = 60;
	static constexpr int TILE_SIZE = 22;

	World(Pacman* game);
	~World(void);

	void Init(std::shared_ptr<Drawer> gameDrawer);

	void Draw(std::shared_ptr<Drawer> aDrawer);
	bool TileIsValid(TileCoord tile);

	bool TryEatDotAt(TileCoord tile);
	bool TryEatBigDotAt(TileCoord tile);
	int GetDotCount();
	int GetEatenDotsCount() { return myEatenDots; }

	void Update();

	std::shared_ptr<PathmapTile> GetTileFromCoords(float x, float y);
	std::shared_ptr<PathmapTile> GetTile(TileCoord coord);

	void SwitchDebugDraw() { myDebugDraw = !myDebugDraw; }

private:
	bool InitMap(std::shared_ptr<Drawer> gameDrawer);

	std::unordered_map<TileCoord, std::shared_ptr<PathmapTile>, tileCoordHash> myMap;
	std::list<std::shared_ptr<Dot>> myDots;
	std::list<std::shared_ptr<BigDot>> myBigDots;

	Sprite::Ptr boardBackground;

	bool myDebugDraw = false;
	Pacman* myGame;
	int myEatenDots = 0;
};

#endif // WORLD_H