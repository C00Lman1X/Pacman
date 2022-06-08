#ifndef PATHMAPTILE_H
#define PATHMAPTILE_H

class Dot;
class BigDot;

struct PathmapTile
{
	PathmapTile(int anX, int anY, bool aIsBlockingFlag);
	~PathmapTile(void);

	int myX;
	int myY;
	bool myIsBlockingFlag;

	Dot* myDot = nullptr;
	BigDot* myBigDot = nullptr;
};

#endif // PATHMAPTILE_H