#include "Sprite.h"
#include "Drawer.h"
#include "SDL_image.h"
#include "SDL_rect.h"
#include "SDL.h"

Sprite::Ptr Sprite::Create(std::list<std::string> assetPaths, Drawer::Ptr drawer, int sizeX, int sizeY)
{
	std::vector<std::shared_ptr<SDL_Texture>> frameCollection;
	std::list<std::string>::iterator it;

	for(it = assetPaths.begin(); it != assetPaths.end(); it++)
	{
		std::shared_ptr<SDL_Texture> texture = drawer->GetTextureResource(*it);
		frameCollection.push_back(texture);
	}

	SDL_Rect sizeRect;
    sizeRect.x = 0;
    sizeRect.y = 0;
    sizeRect.w = sizeX;
    sizeRect.h = sizeY;

	Sprite::Ptr newSprite = std::shared_ptr<Sprite>(new Sprite{frameCollection, sizeRect});
	return newSprite;
}

Sprite::Sprite(std::vector<std::shared_ptr<SDL_Texture>> frameCollection, SDL_Rect sizeRect)
	: frame(sizeRect)
{
	texturesVec = frameCollection;
}

void Sprite::Draw(Drawer::Ptr drawer, int posX, int posY)
{
	drawer->Draw(texturesVec[currentFrameIdx], frame, posX, posY);
}

void Sprite::Update(float dt)
{
	if (texturesVec.size() <= 1)
		return;
		
	timeSinceLastFrame += dt;
	if (timeSinceLastFrame > frameTime)
	{
		while(timeSinceLastFrame > frameTime) timeSinceLastFrame -= frameTime;

		currentFrameIdx = (currentFrameIdx + 1) % texturesVec.size();
	}
}

void Sprite::SetFrame(int x, int y, int w, int h)
{
	frame.x = x;
	frame.y = y;
	frame.w = w;
	frame.h = h;
}