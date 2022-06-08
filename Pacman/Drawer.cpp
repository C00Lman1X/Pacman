#include "Drawer.h"
#include "SDL.h"
#include "SDL_rect.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

Drawer::Ptr Drawer::Create(std::shared_ptr<SDL_Window> aWindow, std::shared_ptr<SDL_Renderer> aRenderer)
{
	Drawer::Ptr drawer = std::shared_ptr<Drawer>(new Drawer{aWindow, aRenderer});

	if (!drawer->Init())
	{
		drawer = nullptr;
	}

	return drawer;
}

Drawer::Drawer(std::shared_ptr<SDL_Window> aWindow, std::shared_ptr<SDL_Renderer> aRenderer)
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

std::shared_ptr<SDL_Texture> Drawer::GetTextureResource(std::string assetPath)
{
	std::shared_ptr<SDL_Texture> texture;

	auto textureFinder = textures.find(assetPath);
	if(textureFinder != textures.end())
		texture = textureFinder->second;
	else
	{
		texture = std::shared_ptr<SDL_Texture>(
			IMG_LoadTexture(myRenderer.get(), assetPath.c_str()),
			SDL_DestroyTexture);
		if (!texture)
		{
			printf("Failed to load texture %s; SDL_error: %s", assetPath.c_str(), SDL_GetError());
		}
		textures[assetPath] = texture;
	}

	return texture;
}

std::shared_ptr<SDL_Texture> Drawer::GetTextureResource(SDL_Surface* surface)
{
	return std::shared_ptr<SDL_Texture>(
		SDL_CreateTextureFromSurface(myRenderer.get(), surface),
		SDL_DestroyTexture);
}

std::shared_ptr<TTF_Font> Drawer::GetFontResource(std::string assetPath, int size)
{
	std::shared_ptr<TTF_Font> font;

	auto fontFinder = fonts.find(assetPath);
	if (fontFinder != fonts.end())
		font = fontFinder->second;
	else
	{
		font = std::shared_ptr<TTF_Font>(
			TTF_OpenFont(assetPath.c_str(), 24),
			TTF_CloseFont);
		if (!font)
		{
			printf("Failed to load font %s; SDL_error: %s", assetPath.c_str(), SDL_GetError());
		}
		fonts[assetPath] = font;
	}

	return font;
}

void Drawer::Draw(std::shared_ptr<SDL_Texture> texture, SDL_Rect frame, int aCellX = 0, int aCellY = 0)
{
    SDL_Rect posRect;
    posRect.x = aCellX;
    posRect.y = aCellY;
	posRect.w = frame.w;
	posRect.h = frame.h;

	SDL_RenderCopy(myRenderer.get(), texture.get(), &frame, &posRect);	
}

void Drawer::DrawText(const char* aText, const char* aFontFile, int aX, int aY)
{
	TTF_Font* font=TTF_OpenFont(aFontFile, 24);

	SDL_Color fg={255,0,0,255};
	SDL_Surface* surface = TTF_RenderText_Solid(font, aText, fg);

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer.get(), surface);

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

	SDL_RenderCopy(myRenderer.get(), optimizedSurface, &sizeRect, &posRect);
	SDL_DestroyTexture(optimizedSurface);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
}
