#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "Vector2f.h"
#include "Sprite.h"

class Drawer;

class GameEntity
{
public:
	GameEntity(const Vector2f& aPosition, Sprite::Ptr entitySprite);
	~GameEntity(void);

	Vector2f GetPosition() const { return myPosition; }
	void SetPosition(const Vector2f& aPosition){ myPosition = aPosition; }

	bool Intersect(GameEntity* aGameEntity);
	virtual void Draw(std::shared_ptr<Drawer> aDrawer);

	void MarkForDelete() { myIdMarkedForDeleteFlag = true; }
	bool IsMarkedForDelete() const { return myIdMarkedForDeleteFlag; }

protected:

	Sprite::Ptr sprite;
	bool myIdMarkedForDeleteFlag;
	Vector2f myPosition; // relative to game field
};

#endif // GAMEENTITY_H