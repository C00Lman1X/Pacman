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
	static constexpr int GAME_FIELD_X = 220;
	static constexpr int GAME_FIELD_Y = 60;
	static constexpr int TILE_SIZE = 22;

	World(void);
	~World(void);

	void Init(Drawer* gameDrawer);

	void Draw(Drawer* aDrawer);
	bool TileIsValid(int anX, int anY);

	bool HasIntersectedDot(const Vector2f& aPosition);
	bool HasIntersectedBigDot(const Vector2f& aPosition);
	bool HasIntersectedCherry(const Vector2f& aPosition);
	int GetDotCount();

	void Update();

	void GetPath(int aFromX, int aFromY, int aToX, int aToY, std::list<PathmapTile*>& aList);

	PathmapTile* GetTileFromCoords(float x, float y);
	PathmapTile* GetTile(int aFromX, int aFromY);

private:

	bool Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::list<PathmapTile*>& aList);
	bool ListDoesNotContain(PathmapTile* aFromTile, std::list<PathmapTile*>& aList);


	bool InitPathmap();
	bool InitDots(Drawer* gameDrawer);
	bool InitBigDots(Drawer* gameDrawer);

	std::unordered_map<std::pair<int,int>, PathmapTile*, pairIntIntHash> myMap;
	std::list<Dot*> myDots;
	std::list<BigDot*> myBigDots;
	std::list<Cherry*> myCherry;

	Sprite* boardBackground;
};

#endif // WORLD_H