#include "Sprite.h"
#include "Drawer.h"
#include "SDL_image.h"
#include "SDL_rect.h"
#include "SDL.h"

Sprite* Sprite::Create(std::list<std::string> assetPaths, Drawer* drawer, int sizeX, int sizeY)
{
	std::vector<SDL_Texture*> frameCollection;
	std::list<std::string>::iterator it;

	for(it = assetPaths.begin(); it != assetPaths.end(); it++)
	{
		SDL_Texture* texture = drawer->GetTextureResource(*it);
		frameCollection.push_back(texture);
	}

	SDL_Rect sizeRect;
    sizeRect.x = 0;
    sizeRect.y = 0;
    sizeRect.w = sizeX;
    sizeRect.h = sizeY;

	Sprite* newSprite = new Sprite(frameCollection, sizeRect);
	return newSprite;
}

Sprite::Sprite(std::vector<SDL_Texture*> frameCollection, SDL_Rect sizeRect)
	: frame(sizeRect)
{
	texturesVec = frameCollection;
}

void Sprite::Draw(Drawer* drawer, int posX, int posY)
{
	drawer->Draw(texturesVec[currentFrameIdx], frame, posX, posY);
}

void Sprite::Update(float dt)
{
	if (texturesVec.size() == 1)
		return;
		
	timeSinceLastFrame += dt;
	if (timeSinceLastFrame > frameTime)
	{
		while(timeSinceLastFrame > frameTime) timeSinceLastFrame -= frameTime;

		currentFrameIdx++;
		if (currentFrameIdx >= texturesVec.size())
			currentFrameIdx = 0;
	}
}