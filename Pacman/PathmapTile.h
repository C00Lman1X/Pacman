#ifndef PATHMAPTILE_H
#define PATHMAPTILE_H

#include <memory>
#include <optional>

class Dot;
class BigDot;

struct PathmapTile
{
	using Ptr = std::shared_ptr<PathmapTile>;
	PathmapTile(int anX, int anY, bool aIsBlockingFlag);
	~PathmapTile(void);


	int myX;
	int myY;
	bool myIsBlockingFlag;

	std::shared_ptr<Dot> myDot = nullptr;
	std::shared_ptr<BigDot> myBigDot = nullptr;

	std::optional<std::pair<int,int>> linkedTile;
};

#endif // PATHMAPTILE_H