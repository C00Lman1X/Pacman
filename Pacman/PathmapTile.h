#ifndef PATHMAPTILE_H
#define PATHMAPTILE_H

#include <memory>

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
};

#endif // PATHMAPTILE_H