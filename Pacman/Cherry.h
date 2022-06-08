#ifndef CHERRY_H
#define CHERRY_H

#include "StaticGameEntity.h"
#include "Vector2f.h"

class Cherry : public StaticGameEntity
{
public:
	using Ptr = std::shared_ptr<Cherry>;

	Cherry(Vector2f aPosition, Sprite::Ptr entitySprite);
	~Cherry(void);
};

#endif // CHERRY_H