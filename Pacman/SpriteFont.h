#ifndef SPRITE_FONT_H
#define SPRITE_FONT_H

#include <list>
#include <map>
#include "SDL_image.h"
#include "SDL_rect.h"
#include "SDL_ttf.h"

struct SDL_Texture;
class Drawer;

class SpriteFont
{
public:
	static SpriteFont* Create(std::string assetPath, std::string initialText, SDL_Color color, int size, Drawer* drawer);
	~SpriteFont(void);

	void SpriteFont::SetText(std::string newTextString);
	void SpriteFont::SetColor(SDL_Color newColor);
	void Draw(Drawer* drawer, int posX, int posY);

private:
	SpriteFont(TTF_Font* font, std::string initialText, SDL_Color initialColor, Drawer* drawer);
	void PrintToTexture();

	std::string text;
	TTF_Font* fontResource;
	SDL_Texture* printedText;
	SDL_Color fontColor;
	SDL_Rect frame;
	Drawer* drawer;
};

#endif // SPRITE_FONT_H