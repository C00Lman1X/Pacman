#include "Pacman.h"
#include "Drawer.h"
#include "SDL.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Avatar.h"
#include "World.h"
#include "Ghost.h"
#include "SpriteFont.h"

Pacman* Pacman::Create(Drawer* aDrawer)
{
	Pacman* pacman = new Pacman(aDrawer);

	if (!pacman->Init())
	{
		delete pacman;
		pacman = NULL;
	}

	return pacman;
}

Pacman::Pacman(Drawer* aDrawer)
: myDrawer(aDrawer)
, myTimeToNextUpdate(0.f)
, myScore(0)
, myFps(0)
, myLives(3)
, myGhostGhostCounter(0.f)
{
	Sprite* newSprite = NULL;
	std::list<std::string> assetPaths;

	assetPaths.clear();
	assetPaths.push_back("ghost_32_cyan.png");
	assetPaths.push_back("Ghost_Vulnerable_32.png");
	assetPaths.push_back("Ghost_Dead_32.png");
	newSprite = Sprite::Create(assetPaths, myDrawer, 32, 32);
	ghosts.push_back(new Ghost(Vector2f(13*World::TILE_SIZE,13*World::TILE_SIZE), newSprite, Ghost::GhostBehavior::Chase));

	assetPaths.clear();
	assetPaths.push_back("ghost_32_cyan.png");
	assetPaths.push_back("Ghost_Vulnerable_32.png");
	assetPaths.push_back("Ghost_Dead_32.png");
	newSprite = Sprite::Create(assetPaths, myDrawer, 32, 32);
	ghosts.push_back(new Ghost(Vector2f(13*World::TILE_SIZE,13*World::TILE_SIZE), newSprite, Ghost::GhostBehavior::Wander));

	assetPaths.clear();
	assetPaths.push_back("ghost_32_cyan.png");
	assetPaths.push_back("Ghost_Vulnerable_32.png");
	assetPaths.push_back("Ghost_Dead_32.png");
	newSprite = Sprite::Create(assetPaths, myDrawer, 32, 32);
	ghosts.push_back(new Ghost(Vector2f(13*World::TILE_SIZE,13*World::TILE_SIZE), newSprite, Ghost::GhostBehavior::Intercept));

	assetPaths.clear();
	assetPaths.push_back("ghost_32_cyan.png");
	assetPaths.push_back("Ghost_Vulnerable_32.png");
	assetPaths.push_back("Ghost_Dead_32.png");
	newSprite = Sprite::Create(assetPaths, myDrawer, 32, 32);
	ghosts.push_back(new Ghost(Vector2f(13*World::TILE_SIZE,13*World::TILE_SIZE), newSprite, Ghost::GhostBehavior::Fear));

	myWorld = new World();

	assetPaths.clear();
	assetPaths.push_back("closed_right_32.png");
	assetPaths.push_back("open_right_32.png");
	assetPaths.push_back("closed_down_32.png");
	assetPaths.push_back("open_down_32.png");
	assetPaths.push_back("closed_left_32.png");
	assetPaths.push_back("open_left_32.png");
	assetPaths.push_back("closed_up_32.png");
	assetPaths.push_back("open_up_32.png");
	newSprite = Sprite::Create(assetPaths, myDrawer, 32, 32);
	myAvatar = new Avatar(Vector2f(13*World::TILE_SIZE,22*World::TILE_SIZE), newSprite, myWorld);

	gameplayMessage = SpriteFont::Create("freefont-ttf/sfd/FreeMono.ttf", "", { 255,255,255,255 }, 24, myDrawer);
	scoreDisplay = SpriteFont::Create("freefont-ttf/sfd/FreeMono.ttf", "", { 0,255,0,255 }, 24, myDrawer);
	livesDisplay = SpriteFont::Create("freefont-ttf/sfd/FreeMono.ttf", "", { 255,255,255,255 }, 24, myDrawer);
	fpsDisplay = SpriteFont::Create("freefont-ttf/sfd/FreeMono.ttf", "", { 255,255,255,255 }, 24, myDrawer);

	UpdateLives(myLives);
	UpdateScore(0);
}

Pacman::~Pacman(void)
{
}

bool Pacman::Init()
{
	myWorld->Init(myDrawer);

	return true;
}

bool Pacman::Update(float aTime)
{
	if (!UpdateInput())
		return false;

	if (CheckEndGameCondition())
		return true;
	else if (myLives <= 0)
		return true;

	std::list<Ghost*>::iterator ghostIterator;

	myAvatar->Update(aTime);
	for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
		(*ghostIterator)->Update(aTime, myWorld, myAvatar);

	if (myWorld->HasIntersectedDot(myAvatar->GetPosition()))
	{
		UpdateScore(10);
		if(CheckEndGameCondition())
			gameplayMessage->SetText("You win!");

	}

	myGhostGhostCounter -= aTime;

	if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition()))
	{
		UpdateScore(20);

		myGhostGhostCounter = 20.f;
		for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
			(*ghostIterator)->myIsClaimableFlag = true;
	}

	if (myGhostGhostCounter <= 0)
	{
		for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
			(*ghostIterator)->myIsClaimableFlag = false;
	}

	for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
	{
		if (((*ghostIterator)->GetPosition() - myAvatar->GetPosition()).Length() < 16.f)
		{
			if (myGhostGhostCounter <= 0.f)
			{
				UpdateLives(myLives -1);

				if (myLives > 0)
				{
					myAvatar->Respawn(Vector2f(13 * World::TILE_SIZE, 22 * World::TILE_SIZE));
					(*ghostIterator)->Respawn(Vector2f(13 * World::TILE_SIZE, 13 * World::TILE_SIZE));
					break;
				}
				else
				{
					gameplayMessage->SetText("You lose!");
					break;
				}
			}
			else if ((*ghostIterator)->myIsClaimableFlag && !(*ghostIterator)->myIsDeadFlag)
			{
				UpdateScore(50);
				(*ghostIterator)->myIsDeadFlag = true;
				(*ghostIterator)->Die(myWorld);
			}
		}
	}
	
	if (aTime > 0)
		SetFPS((int) (1 / aTime));

	return true;
}

void Pacman::UpdateScore(int scoreGain)
{
	myScore += scoreGain;
	std::stringstream stream;
	stream << "Score: ";
	stream << myScore;
	scoreDisplay->SetText(stream.str());
}

void Pacman::UpdateLives(int lives)
{
	myLives = lives;
	std::stringstream stream;
	stream << "Lives: ";
	stream << myLives;
	livesDisplay->SetText(stream.str());
}

void Pacman::SetFPS(int fps)
{
	myFps = fps;
	std::stringstream stream;
	stream << "FPS: ";
	stream << myFps;
	fpsDisplay->SetText(stream.str());
}

bool Pacman::UpdateInput()
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_UP])
		myAvatar->SetNextMovement(Vector2f(0.f, -1.f));
	else if (keystate[SDL_SCANCODE_DOWN])
		myAvatar->SetNextMovement(Vector2f(0.f, 1.f));
	else if (keystate[SDL_SCANCODE_RIGHT])
		myAvatar->SetNextMovement(Vector2f(1.f, 0.f));
	else if (keystate[SDL_SCANCODE_LEFT])
		myAvatar->SetNextMovement(Vector2f(-1.f, 0.f));

	if (keystate[SDL_SCANCODE_ESCAPE])
		return false;

	return true;
}

bool Pacman::CheckEndGameCondition()
{
	return myWorld->GetDotCount() == 0;
}

bool Pacman::Draw()
{
	std::list<Ghost*>::iterator ghostIterator;
	myWorld->Draw(myDrawer);
	myAvatar->Draw(myDrawer);
	for (ghostIterator = ghosts.begin(); ghostIterator != ghosts.end(); ghostIterator++)
		(*ghostIterator)->Draw(myDrawer);

	scoreDisplay->Draw(myDrawer, 20, 50);
	livesDisplay->Draw(myDrawer, 20, 80);
	fpsDisplay->Draw(myDrawer, 880, 50);

	if (CheckEndGameCondition() || myLives <= 0)
		gameplayMessage->Draw(myDrawer, 500, 100);

	return true;
}
