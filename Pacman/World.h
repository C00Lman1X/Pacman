#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <map>
#include <unordered_map>
#include "Vector2f.h"
#include "Sprite.h"

class Drawer;
class PathmapTile;
class Dot;
class BigDot;
class Cherry;
class Pacman;

struct pairIntIntHash
{
	size_t operator()(const std::pair<int, int>& p) const
	{
		// TODO: probably get rid of unordered_map, that's a poor hash for grid coords
		return std::hash<int>{}(p.first) ^ std::hash<int>{}(p.second);
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
	bool TileIsValid(int anX, int anY);
	bool TileIsValid(Vector2f tile);

	bool TryEatDotAt(int x, int y);
	bool TryEatBigDotAt(int x, int y);
	bool TryEatCherryAt(int x, int y);
	int GetDotCount();

	void Update();

	std::shared_ptr<PathmapTile> GetTileFromCoords(float x, float y);
	std::shared_ptr<PathmapTile> GetTile(int aFromX, int aFromY);

	void SwitchDebugDraw() { myDebugDraw = !myDebugDraw; }

private:
	bool InitMap(std::shared_ptr<Drawer> gameDrawer);

	std::unordered_map<std::pair<int,int>, std::shared_ptr<PathmapTile>, pairIntIntHash> myMap;
	std::list<std::shared_ptr<Dot>> myDots;
	std::list<std::shared_ptr<BigDot>> myBigDots;
	std::list<Cherry*> myCherry;

	Sprite::Ptr boardBackground;

	bool myDebugDraw = false;
	Pacman* myGame;
};

#endif // WORLD_H