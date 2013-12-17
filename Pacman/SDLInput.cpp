#include "SDL_image.h"
#include "SDL.h"

#include "SDLInput.h"

namespace Pacman
{

	bool SDLInput::Poll()
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			// X the window
			if (event.type == SDL_QUIT)
			{
				return false;
			}
			//key press
			if (event.type == SDL_KEYDOWN)
			{
				//sets a key flag to true if key was pressed.
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					m_Keys.reset();
					m_Keys[Key::Up] = true;
					break;
				case SDLK_DOWN:
					m_Keys.reset();
					m_Keys[Key::Down] = true;
					break;
				case SDLK_LEFT:
					m_Keys.reset();
					m_Keys[Key::Left] = true;
					break;
				case SDLK_RIGHT:
					m_Keys.reset();
					m_Keys[Key::Right] = true;
					break;
				}
			}
			//set key flag to false if key is released 
			if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					m_Keys.reset();
					m_Keys[Key::Up] = false;
					break;
				case SDLK_DOWN:
					m_Keys.reset();
					m_Keys[Key::Down] = false;
					break;
				case SDLK_LEFT:
					m_Keys.reset();
					m_Keys[Key::Left] = false;
					break;
				case SDLK_RIGHT:
					m_Keys.reset();
					m_Keys[Key::Right] = false;
					break;
				}
			}
		}
		return true;
	}

}