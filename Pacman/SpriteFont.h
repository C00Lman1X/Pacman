#ifndef SPRITE_FONT_H
#define SPRITE_FONT_H

#include <list>
#include <map>
#include <string>
#include "SDL_image.h"
#include "SDL_rect.h"
#include "SDL_ttf.h"

struct SDL_Texture;
class Drawer;

class SpriteFont
{
public:
	using Ptr = std::shared_ptr<SpriteFont>;

	static SpriteFont::Ptr Create(std::string assetPath, std::string initialText, SDL_Color color, int size, std::shared_ptr<Drawer> drawer);
	~SpriteFont(void);

	void SetText(std::string newTextString);
	void SetColor(SDL_Color newColor);
	void Draw(std::shared_ptr<Drawer> drawer, int posX, int posY);

private:
	SpriteFont(std::shared_ptr<TTF_Font> font, std::string initialText, SDL_Color initialColor, std::shared_ptr<Drawer> drawer);
	void PrintToTexture();

	std::string text;
	std::shared_ptr<TTF_Font> fontResource;
	std::shared_ptr<SDL_Texture> printedText;
	SDL_Color fontColor;
	SDL_Rect frame;
	std::shared_ptr<Drawer> drawer;
};

#endif // SPRITE_FONT_H