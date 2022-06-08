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
	using Ptr = std::shared_ptr<Pacman>;

	static std::shared_ptr<Pacman> Create(std::shared_ptr<Drawer> aDrawer);
	~Pacman(void);

	bool Update(float aTime);
	bool Draw();

	const std::list<std::shared_ptr<Ghost>> GetGhosts() const { return ghosts; }

	const std::shared_ptr<Avatar> GetAvatar() const { return myAvatar; }

private:
	Pacman(std::shared_ptr<Drawer> aDrawer);
	bool Init();
	bool UpdateInput();
	bool CheckEndGameCondition();

	void UpdateScore(int scoreGain);
	void UpdateLives(int lives);
	void SetFPS(int fps);

	std::shared_ptr<Drawer> myDrawer;

	int myLives;
	int myScore;
	int myFps;

	std::shared_ptr<Avatar> myAvatar;
	std::list<std::shared_ptr<Ghost>> ghosts;
	std::shared_ptr<World> myWorld;

	std::shared_ptr<SpriteFont> gameplayMessage;
	std::shared_ptr<SpriteFont> scoreDisplay;
	std::shared_ptr<SpriteFont> livesDisplay;
	std::shared_ptr<SpriteFont> fpsDisplay;
};

#endif // PACMAN_H