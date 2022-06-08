#include "SpriteFont.h"
#include "Drawer.h"
#include "SDL_image.h"
#include "SDL_rect.h"
#include "SDL.h"

std::shared_ptr<SpriteFont> SpriteFont::Create(std::string assetPath, std::string initialText, SDL_Color initialColor, int size, Drawer::Ptr drawer)
{
	std::shared_ptr<TTF_Font> font = drawer->GetFontResource(assetPath, size);
	std::shared_ptr<SpriteFont> newFont = std::shared_ptr<SpriteFont>(new SpriteFont{font, initialText, initialColor, drawer});
	return newFont;
}

SpriteFont::SpriteFont(std::shared_ptr<TTF_Font> font, std::string initialText, SDL_Color initialColor, Drawer::Ptr drawer)
	: fontResource(font),
	text(initialText),
	fontColor(initialColor),
	drawer(drawer)
{
	PrintToTexture();
}

SpriteFont::~SpriteFont(void)
{
}

void SpriteFont::SetText(std::string newTextString)
{
	text = newTextString;
	PrintToTexture();
}

void SpriteFont::SetColor(SDL_Color newColor)
{
	fontColor = newColor;
	PrintToTexture();
}

void SpriteFont::PrintToTexture()
{
	SDL_Surface* surface = TTF_RenderText_Solid(fontResource.get(), text.c_str(), fontColor);
	if (!surface)
		return;

	printedText = drawer->GetTextureResource(surface);

	frame.x = 0;
	frame.y = 0;
	frame.w = surface->w;
	frame.h = surface->h;

	SDL_FreeSurface(surface);
}

void SpriteFont::Draw(Drawer::Ptr drawer, int posX, int posY)
{
	drawer->Draw(printedText, frame, posX, posY);
}