#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "assert.h"
#include "Pacman.h"
#include "Drawer.h"
#include <iostream>

int main(int argc, char **argv)
{
	/* initialize SDL */
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		assert(0 && "Failed to initialize video!");
		exit(-1);
	}
	
	std::shared_ptr<SDL_Window> window = std::shared_ptr<SDL_Window>(
		SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL),
		SDL_DestroyWindow);
	std::shared_ptr<SDL_Renderer> renderer = std::shared_ptr<SDL_Renderer>(
		SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED),
		SDL_DestroyRenderer);

	if(!window)
	{
		assert(0 && "Failed to create window!");
		exit(-1);
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	if (TTF_Init() == -1)
	{
		assert(0 && "Failed to create ttf!");
		exit(-1);
	}

	Drawer::Ptr drawer = Drawer::Create(window, renderer);
	Pacman::Ptr pacman = Pacman::Create(drawer);

	float lastFrame = (float) SDL_GetTicks() * 0.001f;
	float targetFPS = 60.f;
	SDL_Event event;
	while (SDL_PollEvent(&event) >= 0)
	{
		if (event.type == SDL_KEYDOWN)
			pacman->KeyPress(event.key.keysym.scancode);

		float currentFrame = (float) SDL_GetTicks() * 0.001f;
		float elapsedTime = currentFrame - lastFrame;

		if (!pacman->Update(elapsedTime))
			break;

		SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
		SDL_RenderClear(renderer.get());

		pacman->Draw();
		
		lastFrame = currentFrame;		

		SDL_RenderPresent(renderer.get());
		SDL_Delay(1000.f/targetFPS);
	}

	TTF_Quit();
	IMG_Quit();
	SDL_Quit( );

	return 0;
}

