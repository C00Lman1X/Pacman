#ifndef DOT_H
#define DOT_H

#include "StaticGameEntity.h"
#include "Vector2f.h"

class Dot : public StaticGameEntity
{
public:
	using Ptr = std::shared_ptr<Dot>;

	Dot(Vector2f aPosition, Sprite::Ptr entitySprite);
	~Dot(void);
};

#endif // DOT_H