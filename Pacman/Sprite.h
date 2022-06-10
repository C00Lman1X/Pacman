#ifndef SPRITE_H
#define SPRITE_H

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "SDL_image.h"
#include "SDL_rect.h"

struct SDL_Texture;
class Drawer;

class Sprite
{
public:
	using Ptr = std::shared_ptr<Sprite>;

	static Sprite::Ptr Create(std::list<std::string> assetPaths, std::shared_ptr<Drawer> drawer, int sizeX, int sizeY);

	void Draw(std::shared_ptr<Drawer> drawer, int posX, int posY);

	void Update(float dt);

	void SetFrame(int x, int y, int w, int h);

private:
	Sprite(std::vector<std::shared_ptr<SDL_Texture>> frameCollection, SDL_Rect sizeRect);
	
	std::vector<std::shared_ptr<SDL_Texture>> texturesVec;
	size_t currentFrameIdx = 0;
	SDL_Rect frame;

	float frameTime = 0.25f;
	float timeSinceLastFrame = 0.f;
};

#endif // SPRITE_H