#ifndef DRAWER_H
#define DRAWER_H

#include <map>
#include <string>
#include "SDL_ttf.h"

struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Rect;

class Drawer
{
public:
	static Drawer* Create(SDL_Window* aWindow, SDL_Renderer* aRenderer);
	~Drawer(void);

	SDL_Renderer* GetRenderer() { return myRenderer; }

	SDL_Texture* GetTextureResource(std::string assetPath);
	SDL_Texture* GetTextureResource(SDL_Surface* surface);
	TTF_Font* GetFontResource(std::string assetPath, int size);

	void Draw(SDL_Texture* texture, SDL_Rect frame, int aCellX, int aCellY);
	void DrawText(const char* aText, const char* aFontFile, int aX, int aY);

private:
	Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer);
	bool Init();
	
	SDL_Window* myWindow;
	SDL_Renderer* myRenderer;
	SDL_Surface* world;

	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
};

#endif // DRAWER_H