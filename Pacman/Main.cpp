#include <iostream>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"

#include "SDLRenderer.h"
#include "SDLInput.h"

#include "Geometry.h"
#include "Timer.h"
#include "Tilemap.h"
#include "Pacman.h"
#include "Ghost.h"
#include "CollisionDetection.h"
#include "PowerUp.h"

#define SCREEN_WIDTH (32 * 19) // screen Widht
#define SCREEN_HEIGHT (32 * 23) //screen height

//folowing array of chars represnts the current level layout. 
const char* level = "\
!!!!!!!!!!!!!!!!!!!\
!########!########!\
!&!!#!!!#!#!!!#!!&!\
!#!!#!!!#!#!!!#!!#!\
!#################!\
!#!!#!#!!!!!#!#!!#!\
!####!###!###!####!\
!!!!#!!!#!#!!!#!!!!\
!!!!#!#########!!!!\
!!!!#!#!!%!!#!#!!!!\
#######!@$@!#######\
!!!!#!#!@$@!#!#!!!!\
!!!!#!#!!!!!#!#!!!!\
!!!!#!#######!#!!!!\
!!!!#!#!!!!!#!#!!!!\
!########!########!\
!#!!#!!!#!#!!!#!!#!\
!&#!###########!#&!\
!!#!#!#!!!!!#!#!#!!\
!####!###!###!####!\
!#!!!!!!#!#!!!!!!#!\
!#################!\
!!!!!!!!!!!!!!!!!!!\
";

int main(int argc, char* args[])
{

	Pacman::SDLRenderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT); //create scene
	Pacman::SDLInput input; // create user input capture

	//different characters on the screen
	auto pacman = std::make_shared<Pacman::Pacman>(9.0f, 13.0f);
	auto blinky = std::make_shared<Pacman::RedGhost>(9.0f, 8.0f, pacman);
	auto pinky = std::make_shared<Pacman::PinkGhost>(9.0f, 9.0f, pacman);
	auto inky = std::make_shared<Pacman::BlueGhost>(9.0f, 10.0f, pacman, blinky);
	auto clyde = std::make_shared<Pacman::OrangeGhost>(9.0f, 11.0f, pacman);

	std::set<std::shared_ptr< Pacman::PowerObj>> powerUps; // set of the 4 powerups. Exists for easier iterartion trought them
	std::vector<std::shared_ptr<Pacman::Ghost>> ghosts{ blinky, pinky, inky, clyde }; // vector of all the ghosts. Exists for easier iteration between them
	int coinNumber = 0; // number of total coins in level

	//different asset surfaces
	Pacman::SDLSurface pacmanSurface("Assets\\pacman.png");
	Pacman::SDLSurface redGhostSurface("Assets\\redGhost.png");
	Pacman::SDLSurface pinkGhostSurface("Assets\\pinkGhost.png");
	Pacman::SDLSurface blueGhostSurface("Assets\\blueGhost.png");
	Pacman::SDLSurface orangeGhostSurface("Assets\\orangeGhost.png");
	Pacman::SDLSurface frightenedGhostSurface("Assets\\frightGhost.png");
	Pacman::SDLSurface ghostEyesSurface("Assets\\ghostEyes.png");
	Pacman::SDLSurface powerUpSurface("Assets\\powerup.png");
	Pacman::SDLSurface endGameSurface("Assets\\win.png");

	Pacman::Tilemap tilemap(19, 23); // create a tilemap

	for (auto x = 0u; x < 19; x++) // will the tilemap with the above levelm setting tiles corresponding to symbols
	{
		for (auto y = 0u; y < 23; y++)
		{
			if (level[x + y * 19] == '#')
			{
				tilemap.Set(x, y, Pacman::Coin);
				coinNumber += 1;
			}
			else if (level[x + y * 19] == '@')
			{
				tilemap.Set(x, y, Pacman::GhostBlock);
			}
			else if (level[x + y * 19] == '%')
			{
				tilemap.Set(x, y, Pacman::Empty);
			}
			else if (level[x + y * 19] == '$')
			{
				tilemap.Set(x, y, Pacman::GhostBaseMid);
			}
			else if (level[x + y * 19] == '&')
			{
				tilemap.Set(x, y, Pacman::PowerUp);
				powerUps.emplace(std::make_shared< Pacman::PowerObj>(x, y));
			}
			else
			{
				tilemap.Set(x, y, Pacman::Wall);
			}
		}
	}
	tilemap.CalculateFlags(); // calculate the flags for the just created tilemap

	Pacman::CollisionDetection collisions; // create a collision interface for all ICollider objects

	// insert the above mentioned objects in the collision interface
	collisions.Insert(pacman);
	collisions.Insert(blinky);
	collisions.Insert(pinky);
	collisions.Insert(inky);
	collisions.Insert(clyde);
	for (auto &i : powerUps)
		collisions.Insert(i);

	std::vector<Pacman::SDLSurface> gridSurfaces; // surfaces for the grid in the tilemap
	gridSurfaces.emplace_back("Assets\\wall.png");
	gridSurfaces.emplace_back("Assets\\coin.png");

	auto tilemapSurface = std::make_shared<Pacman::SDLRenderTarget>(19 * 32, 23 * 32); // creates a pointer to a SDLRenderTarget object, to render our tilemap seperatly.
	tilemapSurface->RenderTileset(gridSurfaces, tilemap.GetGrid(), 19, 32.0f); // render the tile map

	Pacman::Timer::Initialize(); // start the timer.

	float lastTime = Pacman::Timer::GetTime();

	for (;;) //game loop
	{
		if (!input.Poll()) // polls user input
		{
			return 0;
		}

		float time = Pacman::Timer::GetTime(); // gets current time
		float deltaTime = time - lastTime;// calculates difference between current time and start of the frame. Used to make movement frame independant
		lastTime = time;

		//update our characters
		pacman->Update(input, tilemap, deltaTime, tilemapSurface, gridSurfaces);
		blinky->Update(tilemap, deltaTime);
		pinky->Update(tilemap, deltaTime);
		inky->Update(tilemap, deltaTime);
		clyde->Update(tilemap, deltaTime);

		collisions.ResolveCollisions(); //resolve collision between them

		if (pacman->GetPoints() >= coinNumber) // if all coins collected
		{
			renderer.Blit(endGameSurface, 0, 0);// apply win png 
			renderer.Show();// show win png
			SDL_Delay(8000);// wain 8 secs
			exit(0);//exit
		}

		if (pacman->GetState() == Pacman::PacState::Dead) // if ghosts caught the pacman, reset the level
		{
			pacman->Reset();
			blinky->Reset(Pacman::Direction::Left);
			pinky->Reset(Pacman::Direction::Up);
			inky->Reset(Pacman::Direction::Up);
			clyde->Reset(Pacman::Direction::Up);
		}

		renderer.Clear(); // clear surface

		renderer.Blit(*tilemapSurface, 0, 0);// apply tilemap 

		for (auto &i : powerUps)
		if (i->is_enabled())
			renderer.Blit(powerUpSurface, (size_t)(i->GetPosition().x * 32.0f), (size_t)(i->GetPosition().y * 32.0f)); // apply powerups

		for (auto &h : ghosts)
		if (pacman->GetState() == Pacman::PacState::PowerUp)
			renderer.Blit(frightenedGhostSurface, (size_t)(h->GetPosition().x * 32.0f), (size_t)(h->GetPosition().y * 32.0f)); // chage the ghost to frighened sprite

		for (auto &g : ghosts)
		if (g->GetState() == Pacman::GhostState::ReturnToBase)
			renderer.Blit(ghostEyesSurface, (size_t)(g->GetPosition().x * 32.0f), (size_t)(g->GetPosition().y * 32.0f)); // change ghost to return to base (dead) sprite

		// if each ghost is in normal chase mode or at base, apply the normal ghost sprite
		if (blinky->GetState() == Pacman::GhostState::Chase || blinky->GetState() == Pacman::GhostState::Base)
			renderer.Blit(redGhostSurface, (size_t)(blinky->GetPosition().x * 32.0f), (size_t)(blinky->GetPosition().y * 32.0f));

		if (pinky->GetState() == Pacman::GhostState::Chase || blinky->GetState() == Pacman::GhostState::Base)
			renderer.Blit(pinkGhostSurface, (size_t)(pinky->GetPosition().x * 32.0f), (size_t)(pinky->GetPosition().y * 32.0f));

		if (inky->GetState() == Pacman::GhostState::Chase || blinky->GetState() == Pacman::GhostState::Base)
			renderer.Blit(blueGhostSurface, (size_t)(inky->GetPosition().x * 32.0f), (size_t)(inky->GetPosition().y * 32.0f));

		if (clyde->GetState() == Pacman::GhostState::Chase || blinky->GetState() == Pacman::GhostState::Base)
			renderer.Blit(orangeGhostSurface, (size_t)(clyde->GetPosition().x * 32.0f), (size_t)(clyde->GetPosition().y * 32.0f));

		renderer.Blit(pacmanSurface, (size_t)(pacman->GetPosition().x * 32.0f), (size_t)(pacman->GetPosition().y * 32.0f)); // render pacman

		renderer.Show(); // update screen
	}

	return 0;
}