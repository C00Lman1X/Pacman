#ifndef DRAWER_H
#define DRAWER_H

#include <map>
#include <memory>
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
	using Ptr = std::shared_ptr<Drawer>;

	static Drawer::Ptr Create(std::shared_ptr<SDL_Window> aWindow, std::shared_ptr<SDL_Renderer> aRenderer);
	~Drawer(void);

	std::shared_ptr<SDL_Renderer> GetRenderer() { return myRenderer; }

	std::shared_ptr<SDL_Texture> GetTextureResource(std::string assetPath);
	std::shared_ptr<SDL_Texture> GetTextureResource(SDL_Surface* surface);
	std::shared_ptr<TTF_Font> GetFontResource(std::string assetPath, int size);

	void Draw(std::shared_ptr<SDL_Texture> texture, SDL_Rect frame, int aCellX, int aCellY);
	void DrawText(const char* aText, const char* aFontFile, int aX, int aY);

private:
	Drawer(std::shared_ptr<SDL_Window> aWindow, std::shared_ptr<SDL_Renderer> aRenderer);
	bool Init();
	
	std::shared_ptr<SDL_Window> myWindow;
	std::shared_ptr<SDL_Renderer> myRenderer;
	SDL_Surface* world;

	std::map<std::string, std::shared_ptr<SDL_Texture>> textures;
	std::map<std::string, std::shared_ptr<TTF_Font>> fonts;
};

#endif // DRAWER_H