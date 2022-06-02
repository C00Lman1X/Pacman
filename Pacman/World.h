#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <map>
#include "Vector2f.h"
#include "Sprite.h"

class Drawer;
class PathmapTile;
class Dot;
class BigDot;
class Cherry;

class World
{
public:
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

private:

	PathmapTile* GetTile(int aFromX, int aFromY);
	bool Pathfind(PathmapTile* aFromTile, PathmapTile* aToTile, std::list<PathmapTile*>& aList);
	bool ListDoesNotContain(PathmapTile* aFromTile, std::list<PathmapTile*>& aList);


	bool InitPathmap();
	bool InitDots(Drawer* gameDrawer);
	bool InitBigDots(Drawer* gameDrawer);

	std::list<PathmapTile*> myPathmapTiles;
	std::list<Dot*> myDots;
	std::list<BigDot*> myBigDots;
	std::list<Cherry*> myCherry;

	Sprite* boardBackground;
};

#endif // WORLD_H