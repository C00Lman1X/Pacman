#include "Drawer.h"
#include "SDL.h"
#include "SDL_rect.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

Drawer* Drawer::Create(SDL_Window* aWindow, SDL_Renderer* aRenderer)
{
	Drawer* drawer = new Drawer(aWindow, aRenderer);

	if (!drawer->Init())
	{
		delete drawer;
		drawer = NULL;
	}

	return drawer;
}

Drawer::Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer)
: myWindow(aWindow)
, myRenderer(aRenderer)
{
}

Drawer::~Drawer(void)
{
}

bool Drawer::Init()
{
	if (!myWindow)
		return false;

	return true;
}

SDL_Texture* Drawer::GetTextureResource(std::string assetPath)
{
	SDL_Texture* texture = NULL;

	std::map<std::string, SDL_Texture*>::iterator textureFinder = textures.find(assetPath);
	if(textureFinder != textures.end())
		texture = textureFinder->second;
	else
	{
		texture = IMG_LoadTexture(myRenderer, assetPath.c_str());
		if (!texture)
		{
			printf("Failed to load texture %s; SDL_error: %s", assetPath.c_str(), SDL_GetError());
		}
		textures[assetPath] = texture;
	}

	return texture;
}

SDL_Texture* Drawer::GetTextureResource(SDL_Surface* surface)
{
	return SDL_CreateTextureFromSurface(myRenderer, surface);
}

TTF_Font* Drawer::GetFontResource(std::string assetPath, int size)
{
	TTF_Font* font = NULL;

	std::map<std::string, TTF_Font*>::iterator fontFinder = fonts.find(assetPath);
	if (fontFinder != fonts.end())
		font = fontFinder->second;
	else
	{
		font = TTF_OpenFont(assetPath.c_str(), 24);
		if (!font)
		{
			printf("Failed to load font %s; SDL_error: %s", assetPath.c_str(), SDL_GetError());
			font = TTF_OpenFont(assetPath.c_str(), 24);
		}
		fonts[assetPath] = font;
	}

	return font;
}

void Drawer::Draw(SDL_Texture* texture, SDL_Rect frame, int aCellX = 0, int aCellY = 0)
{
    SDL_Rect posRect;
    posRect.x = aCellX;
    posRect.y = aCellY;
	posRect.w = frame.w;
	posRect.h = frame.h;

	SDL_RenderCopy(myRenderer, texture, &frame, &posRect);	
}

void Drawer::DrawText(const char* aText, const char* aFontFile, int aX, int aY)
{
	TTF_Font* font=TTF_OpenFont(aFontFile, 24);

	SDL_Color fg={255,0,0,255};
	SDL_Surface* surface = TTF_RenderText_Solid(font, aText, fg);

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer, surface);

    SDL_Rect sizeRect;
    sizeRect.x = 0 ;
    sizeRect.y = 0 ;
    sizeRect.w = surface->w ;
    sizeRect.h = surface->h ;

    SDL_Rect posRect ;
    posRect.x = aX;
    posRect.y = aY;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(myRenderer, optimizedSurface, &sizeRect, &posRect);
	SDL_DestroyTexture(optimizedSurface);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
}
