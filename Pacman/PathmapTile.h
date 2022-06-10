#ifndef PATHMAPTILE_H
#define PATHMAPTILE_H

#include <memory>
#include <optional>

class Dot;
class BigDot;
class Vector2f;

struct TileCoord
{
	int x = 0;
	int y = 0;

	bool operator==(const TileCoord &other) const;
	bool operator!=(const TileCoord &other) const;
	const TileCoord operator-(const TileCoord &other) const ;
	const TileCoord operator+(const TileCoord &other) const ;
	const TileCoord operator*(int aInt) const ;
	const Vector2f operator*(float aFloat) const;
};

struct PathmapTile
{
	using Ptr = std::shared_ptr<PathmapTile>;
	PathmapTile(int anX, int anY, bool aIsBlockingFlag);
	~PathmapTile(void);

	TileCoord coord;
	bool myIsBlockingFlag;

	std::shared_ptr<Dot> myDot = nullptr;
	std::shared_ptr<BigDot> myBigDot = nullptr;

	std::optional<std::pair<int,int>> linkedTile;
};

#endif // PATHMAPTILE_H