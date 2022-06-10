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

Pacman::Ptr Pacman::Create(Drawer::Ptr aDrawer)
{
	Pacman::Ptr pacman = std::shared_ptr<Pacman>(new Pacman{aDrawer});

	if (!pacman->Init())
	{
		pacman = nullptr;
	}

	return pacman;
}

Pacman::Pacman(Drawer::Ptr aDrawer)
: myDrawer(aDrawer)
, myScore(0)
, myFps(0)
, myLives(3)
{
	ghosts.push_back(std::make_shared<Ghost>(TileCoord{13,13}, Ghost::Red, myDrawer));
	ghosts.push_back(std::make_shared<Ghost>(TileCoord{13,13}, Ghost::Cyan, myDrawer, ghosts.back()));
	ghosts.push_back(std::make_shared<Ghost>(TileCoord{13,13}, Ghost::Pink, myDrawer));
	ghosts.push_back(std::make_shared<Ghost>(TileCoord{13,13}, Ghost::Orange, myDrawer));

	myWorld = std::make_shared<World>(this);

	myAvatar = std::make_shared<Avatar>(TileCoord{13,22}, myWorld, myDrawer);

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

	myAvatar->Update(aTime);
	for (Ghost::Ptr ghost : ghosts)
		ghost->Update(aTime, myWorld, myAvatar);

	if (myWorld->TryEatDotAt(myAvatar->GetCurrentTile()))
	{
		UpdateScore(10);
		if(CheckEndGameCondition())
			gameplayMessage->SetText("You win!");

	}

	if (myWorld->TryEatBigDotAt(myAvatar->GetCurrentTile()))
	{
		UpdateScore(20);

		for (auto& ghost : ghosts)
			if (!ghost->IsDead())
				ghost->SetState(Ghost::GhostBehavior::Fear);
	}

	for (auto& ghost : ghosts)
	{
		if (ghost->GetCurrentTile() == myAvatar->GetCurrentTile())
		{
			if (ghost->IsClaimable())
			{
				UpdateScore(50);
				ghost->SetState(Ghost::GhostBehavior::Dead);
			}
			else if (!ghost->IsDead())
			{
				UpdateLives(myLives -1);

				if (myLives > 0)
				{
					myAvatar->Respawn(TileCoord{13, 22});
					ghost->Respawn(TileCoord{13, 13});
					break;
				}
				else
				{
					gameplayMessage->SetText("You lose!");
					break;
				}
			}
		}
	}
	
	if (aTime > 0.f)
		SetFPS((int) (1.f / aTime));

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
		myAvatar->SetNextMovement(TileCoord{0, -1});
	else if (keystate[SDL_SCANCODE_DOWN])
		myAvatar->SetNextMovement(TileCoord{0, 1});
	else if (keystate[SDL_SCANCODE_RIGHT])
		myAvatar->SetNextMovement(TileCoord{1, 0});
	else if (keystate[SDL_SCANCODE_LEFT])
		myAvatar->SetNextMovement(TileCoord{-1, 0});
	
	if (keystate[SDL_SCANCODE_D])
		myWorld->SwitchDebugDraw();

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
	myWorld->Draw(myDrawer);
	myAvatar->Draw(myDrawer);
	for (auto ghost : ghosts)
		ghost->Draw(myDrawer);

	scoreDisplay->Draw(myDrawer, 20, 50);
	livesDisplay->Draw(myDrawer, 20, 80);
	fpsDisplay->Draw(myDrawer, 880, 50);

	if (CheckEndGameCondition() || myLives <= 0)
		gameplayMessage->Draw(myDrawer, 500, 100);

	return true;
}
