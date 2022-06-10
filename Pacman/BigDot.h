#ifndef BIGDOT_H
#define BIGDOT_H

#include "StaticGameEntity.h"
#include "Vector2f.h"

class BigDot : public StaticGameEntity
{
public:
	using Ptr = std::shared_ptr<BigDot>;

	BigDot(TileCoord aTile, Sprite::Ptr entitySprite);
	~BigDot(void);
};

#endif // BIGDOT_H