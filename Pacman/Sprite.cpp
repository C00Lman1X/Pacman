#include "Sprite.h"
#include "Drawer.h"
#include "SDL_image.h"
#include "SDL_rect.h"
#include "SDL.h"

Sprite* Sprite::Create(std::list<std::string> assetPaths, Drawer* drawer, int sizeX, int sizeY)
{
	std::map<std::string, SDL_Texture*> frameCollection;
	std::list<std::string>::iterator it;

	for(it = assetPaths.begin(); it != assetPaths.end(); it++)
	{
		SDL_Texture* texture = drawer->GetTextureResource(*it);
		frameCollection[*it] = texture;
	}

	SDL_Rect sizeRect;
    sizeRect.x = 0;
    sizeRect.y = 0;
    sizeRect.w = sizeX;
    sizeRect.h = sizeY;

	Sprite* newSprite = new Sprite(frameCollection, sizeRect);
	return newSprite;
}

void Sprite::SetFrame(std::string frameKey)
{
	if(textures[frameKey] != NULL)
		currentFrame = frameKey;
}

Sprite::Sprite(std::map<std::string, SDL_Texture*> frameCollection, SDL_Rect sizeRect)
	: frame(sizeRect)
{
	textures.insert(frameCollection.begin(), frameCollection.end());
}

void Sprite::Draw(Drawer* drawer, int posX, int posY)
{
	drawer->Draw(textures[currentFrame], frame, posX, posY);
}