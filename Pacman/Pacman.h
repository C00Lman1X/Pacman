#ifndef PACMAN_H
#define PACMAN_H

#include "Vector2f.h"
#include <list>

struct SDL_Surface;
class Drawer;
class Avatar;
class World;
class Ghost;
class SpriteFont;

class Pacman
{
public:
	static Pacman* Create(Drawer* aDrawer);
	~Pacman(void);

	bool Update(float aTime);
	bool Draw();

private:
	Pacman(Drawer* aDrawer);
	bool Init();
	bool UpdateInput();
	bool CheckEndGameCondition();

	void UpdateScore(int scoreGain);
	void UpdateLives(int lives);
	void SetFPS(int fps);

	Drawer* myDrawer;

	float myTimeToNextUpdate;
	float myGhostGhostCounter;

	int myLives;
	int myScore;
	int myFps;

	Avatar* myAvatar;
	std::list<Ghost*> ghosts;
	World* myWorld;

	SpriteFont* gameplayMessage;
	SpriteFont* scoreDisplay;
	SpriteFont* livesDisplay;
	SpriteFont* fpsDisplay;
};

#endif // PACMAN_H