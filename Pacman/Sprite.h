#ifndef SPRITE_H
#define SPRITE_H

#include <list>
#include <map>
#include <string>
#include <vector>
#include "SDL_image.h"
#include "SDL_rect.h"

struct SDL_Texture;
class Drawer;

class Sprite
{
public:
	static Sprite* Create(std::list<std::string> assetPaths, Drawer* drawer, int sizeX, int sizeY);
	~Sprite(void);

	void Draw(Drawer* drawer, int posX, int posY);

	void Update(float dt);

private:
	Sprite(std::vector<SDL_Texture*> frameCollection, SDL_Rect sizeRect);
	
	std::vector<SDL_Texture*> texturesVec;
	size_t currentFrameIdx = 0;
	SDL_Rect frame;

	float frameTime = 0.25f;
	float timeSinceLastFrame = 0.f;
};

#endif // SPRITE_H